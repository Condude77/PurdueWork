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
	  //execvp(_simpleCommands[i]->_arguments[i], _simpleCommands[i]->_arguments);
	  execvp(cargs[0], cargs.data());
	  perror("execvp");
	  _exit(1);
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
