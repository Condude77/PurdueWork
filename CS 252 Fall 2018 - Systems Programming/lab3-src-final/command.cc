/*
 * CS252: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 * DO NOT PUT THIS PROJECT IN A PUBLIC REPOSITORY LIKE GIT. IF YOU WANT 
 * TO MAKE IT PUBLICALLY AVAILABLE YOU NEED TO REMOVE ANY SKELETON CODE 
 * AND REWRITE YOUR PROJECT SO IT IMPLEMENTS FUNCTIONALITY DIFFERENT THAN
 * WHAT IS SPECIFIED IN THE HANDOUT. WE OFTEN REUSE PART OF THE PROJECTS FROM  
 * SEMESTER TO SEMESTER AND PUTTING YOUR CODE IN A PUBLIC REPOSITORY
 * MAY FACILITATE ACADEMIC DISHONESTY.
 */

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>

#include "command.hh"
#include "shell.hh"

extern char ** environ;

Command::Command() {
    // Initialize a new vector of Simple Commands
    _simpleCommands = std::vector<SimpleCommand *>();

    _outFile = NULL;
    _inFile = NULL;
    _errFile = NULL;
    _background = false;
    _append = 0;
    _inCount = 0;
    _outCount = 0;
}

void Command::insertSimpleCommand( SimpleCommand * simpleCommand ) {
    // add the simple command to the vector
    _simpleCommands.push_back(simpleCommand);
}

void Command::clear() {
    // deallocate all the simple commands in the command vector
    for (auto simpleCommand : _simpleCommands) {
        delete simpleCommand;
    }

    // remove all references to the simple commands we've deallocated
    // (basically just sets the size to 0)
    _simpleCommands.clear();

    if ( _outFile == _errFile ) {
        delete _outFile;
    }else{
	if ( _outFile ) {
	    delete _outFile;
	}
	if ( _errFile ) {
	    delete _errFile;
	}
    }
    _outFile = NULL;
    _errFile = NULL;

    if ( _inFile ) {
        delete _inFile;
    }
    _inFile = NULL;

    _background = false;

    _append = 0;
    
    _inCount = 0;
    
    _outCount = 0;
}

void Command::print() {
    printf("\n\n");
    printf("              COMMAND TABLE                \n");
    printf("\n");
    printf("  #   Simple Commands\n");
    printf("  --- ----------------------------------------------------------\n");

    int i = 0;
    // iterate over the simple commands and print them nicely
    for ( auto & simpleCommand : _simpleCommands ) {
        printf("  %-3d ", i++ );
        simpleCommand->print();
    }

    printf( "\n\n" );
    printf( "  Output       Input        Error        Background\n" );
    printf( "  ------------ ------------ ------------ ------------\n" );
    printf( "  %-12s %-12s %-12s %-12s\n",
            _outFile?_outFile->c_str():"default",
            _inFile?_inFile->c_str():"default",
            _errFile?_errFile->c_str():"default",
            _background?"YES":"NO");
    printf( "\n\n" );
}

int Command::builtInCommand(int cmd){
    //check if a built in command is called in the shell
    std::vector<char *> bic;
    bic.reserve(_simpleCommands[cmd]->_arguments.size());
    for(auto& s: _simpleCommands[cmd]->_arguments){
        bic.push_back(&s->front());
    }
    bic.push_back(NULL);
    //check for setenv
    if(!strcmp(bic[0], "setenv")){
	if(setenv(bic[1], bic[2], 1)){
	  perror("setenv");
	}
	clear();
	Shell::prompt();
	return 1;
    }
    //check for unsetenv
    if(!strcmp(bic[0], "unsetenv")){
	if(unsetenv(bic[1])){
	  perror("unsetenv");
	}
	clear();
	Shell::prompt();
	return 1;
    }
    //check for cd
    if(!strcmp(bic[0], "cd")){
	int err;
	if(bic.size() == 2){
	  err = chdir(getenv("HOME"));
	}else{
	  err = chdir(bic[1]);
	}
	if(err < 0){
	  std::cerr << "cd: can't cd to " << bic[1] << '\n';
	}
	clear();
	Shell::prompt();
	return 1;
    }
    //no built in commands
    return 0;
}

void Command::runSource(char* file){
    //open the file
    FILE * fp = fopen(file, "r");
    char src[1024];
    fgets(src, 1023, fp);
    fclose(fp);
    
    int tempin = dup(0);
    int tempout = dup(1);

    int fdpipein[2];
    pipe(fdpipein);
    write(fdpipein[1], src, strlen(src));
    write(fdpipein[1], "\n", 1);
    close(fdpipein[1]);
    dup2(fdpipein[0], 0);
    close(fdpipein[0]);

    int fdpipeout[2];
    pipe(fdpipeout);
    dup2(fdpipeout[1], 1);
    close(fdpipeout[1]);
 
    int ret = fork();
    if(ret == 0){
      execvp("/proc/self/exe", NULL);
      perror("execvp");
      _exit(1);
    }else if(ret < 0){
      perror("fork");
      exit(1);
    }
    
    dup2(tempin, 0);
    dup2(tempout, 1);
    close(tempin);
    close(tempout);

    char ch;
    char* buffer = (char *) malloc (100);
    int i = 0;
    while(read(fdpipeout[0], &ch, 1)){
      if(ch != '\n'){
	buffer[i++] = ch;
      }
    }
    buffer[i] = '\0';
    printf("%s\n", buffer);
}

void Command::execute() {
    // Don't do anything if there are no simple commands
    if ( _simpleCommands.size() == 0 ) {
        Shell::prompt();
        return;
    }

    //check for ambiguous output redirect
    if ( _outCount > 1){
	//printf("in count: %d\n", _inCount);
	//printf("out count: %d\n", _outCount);
	printf("Ambiguous output redirect.\n");
	clear();
	Shell::prompt();
	return;
    }

    //check for exit command
    std::vector<char *> tempstr;
    tempstr.reserve(_simpleCommands[0]->_arguments.size());
    for(auto& s: _simpleCommands[0]->_arguments){
	tempstr.push_back(&s->front());
    }
    tempstr.push_back(NULL);

    if (!strcmp(tempstr[0], "exit") || !strcmp(tempstr[0], "sexit")) {
        if(!strcmp(tempstr[0], "exit")){
	  printf("\nGood bye!!\n\n");
	}
	_exit(1);
    }

    // Print contents of Command data structure
    //print();

    // Add execution here
    // For every simple command fork a new process
    int tempin = dup(0);
    int tempout = dup(1);
    int temperr = dup(2);

    int fdin;
    int fdout;
    int fderr;

    //open _inFile
    if (_inFile) {
	//if there is a file
	//convert _inFile to c-string
	char * cin = new char [_inFile->length() + 1];
	std::strcpy (cin, _inFile->c_str());
	fdin = open(cin, O_RDONLY);
    }else{
	//if no file then use standard in
	fdin = dup(tempin);
    }

    //open _errFile
    if (_errFile) {
	//if there is a file
	//convert _errFile to c-string
	char * cerr = new char [_errFile->length() + 1];
	std::strcpy (cerr, _errFile->c_str());
	if (_append) {
	  //if there the file is appended
	  fderr = open(cerr, O_WRONLY | O_CREAT | O_APPEND, 0600);
	}else{
	  //use truncated file
	  fderr = open(cerr, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	}
    }else{
	//use standard error
	fderr = dup(temperr);
    }

    //redirect error and then close it
    dup2(fderr, 2);
    close(fderr);
    
    int ret;
    for ( unsigned int i = 0; i < _simpleCommands.size(); i++ ) {
	//redirect for input and then close it
	dup2(fdin, 0);
	close(fdin);

	//check for built in commands
	if(builtInCommand(i)){
	  return;
	}
	
	//check for last simple command
	if (i == (_simpleCommands.size() - 1)) {
	  //open _outFile
	  if (_outFile) {
	    //if there is a file
	    //convert _outFile to c-string
            char * cout = new char [_outFile->length() + 1];
            std::strcpy (cout, _outFile->c_str());
	    if (_append) {
		//if the file is appended
		fdout = open(cout, O_WRONLY | O_CREAT | O_APPEND, 0600);
	    }else{
		//else use truncated file
		fdout = open(cout, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	    }
	  }else{
	    //use standard out if no file was given
	    fdout = dup(tempout);
	  }
	}else{
	  //pipe if not last command
	  int fdpipe[2];
	  pipe(fdpipe);
	
	  //set fdin and fdout to the pipe in and pipe out
	  fdin = fdpipe[0];
	  fdout = fdpipe[1];
	}

	//redirect output then close it
	dup2(fdout, 1);
	close(fdout);
	
	//execute commands
	ret = fork();
	if (ret == 0) {
	  //ret == 0 means child process which is copy of parent
	  //convert arguments to char * vector
	  //first get number of arguments
	  std::vector<char *> cargs;
	  cargs.reserve(_simpleCommands[i]->_arguments.size());
	  //copy args
	  for(auto& s: _simpleCommands[i]->_arguments){
	    cargs.push_back(&s->front());
	  }
	  cargs.push_back(NULL);

	  //check for printenv
	  if(!strcmp(cargs[0], "printenv")){
	    char** p = environ;
	    while(*p){
		printf("%s\n", *p);
		p++;
	    }
	    exit(0);
	  }
	  
	  //check for source
	  if(!strcmp(cargs[0], "source")){
	    runSource(cargs[1]);
	  }else{
	    execvp(cargs[0], cargs.data());
	    perror("execvp");
	    _exit(1);
          }
	}else if (ret < 0) {
	  //ret < 0 means there was an error in the fork
	  perror("fork");
	  return;
	}
	//ret > 0 means it is a parent
    }
    if (!_background) {
	//wait for last process
	waitpid(ret, NULL, 0);
    }
    
    //restore defaults
    dup2(tempin, 0);
    dup2(tempout, 1);
    dup2(temperr, 2);
    close(tempin);
    close(tempout);
    close(temperr);

    // Clear to prepare for next command
    clear();

    // Print new prompt
    Shell::prompt();
}

SimpleCommand * Command::_currentSimpleCommand;
