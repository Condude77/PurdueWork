#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <limits.h>

#include "simpleCommand.hh"

SimpleCommand::SimpleCommand() {
  _arguments = std::vector<std::string *>();
}

SimpleCommand::~SimpleCommand() {
  // iterate over all the arguments and delete them
  for (auto & arg : _arguments) {
    delete arg;
  }
}

char * SimpleCommand::environmentVariableCheck(char * argument) {
  char * arg = strdup(argument);
  //check for a dollar sign and brace
  char * dollar = strchr(arg, '$');
  char * brace = strchr(arg, '{');
  
  char * expansion = (char *) malloc (sizeof(argument) + 400);
  char * temp = expansion;

  if(dollar && brace){
    //get to where we need to expand
    while(*arg != '$'){
      *temp = *arg;
      arg++;
      temp++;
    }
    *temp = '\0';

    while(dollar){
      if(dollar[1] == '{' && dollar[2] != '}'){
	char * end = dollar + 2;
	char * var = (char *) malloc (25);
	char * varTemp = var;

	//get the variable we need to expand
	while(*end != '}'){
	  *varTemp = *end;
	  end++;
	  varTemp++;
	}
	*varTemp = '\0';

	//check for special expansions
	if(!strcmp(var, "$")){
	  //get the PID of the shell process
	  char * pid = (char *) getpid();
	  strcat(expansion, pid);
	}else if(!strcmp(var, "?")){
	  //get the return code of the last executed simple command
	  int code;
	  wait(&code);
	  if(WIFEXITED(code)){
	    strcat(expansion, (const char *) WEXITSTATUS(code));
	  }else{
	    
	  }
	}else if(!strcmp(var,"!")){
	  //get the PID of last background process
	}else if(!strcmp(var,"_")){
	  //get the last argument int the fully expanded previous command
	}else if(!strcmp(var,"SHELL")){
	  //get the path of the shell executable
	  char * path = "/u/data/u87/brow1325/cs252/lab3-src/shell";
	  char actualpath[PATH_MAX + 1];
	  char * ptr = realpath(path, actualpath);
	  strcat(expansion, ptr);
	}else{
	  //insert the environment into the arg
	  char * environment = getenv(var);
	  strcat(expansion, environment);
	}

	while(*(arg - 1) != '}'){
	  arg++;
	}
	
	//get the stuff after the insertion
	char * extra = (char *) malloc (25);
	char * extraTemp = extra;
	while(*arg && *arg != '$'){
	  *extraTemp = *arg;
	  arg++;
	  extraTemp++;
	}
	*extraTemp = '\0';
	strcat(expansion, extra);
      }
      dollar++;
      dollar = strchr(dollar, '$');
    }
    argument = strdup(expansion);
    return argument;
  }
  return NULL;
}

char * SimpleCommand::tildeCheck(char * argument) {
  char * arg = strdup(argument);
  if(arg[0] == '~'){
    if(strlen(arg) == 1){
      //list the home directory
      arg = strdup(getenv("HOME"));
      return arg;
    }else{
      if(arg[1] == '/'){
	//list home and add after slash
	arg++;
	char * home = strdup(getenv("HOME"));
	arg = strcat(home, arg);
	return arg;
      }
      
      char * dir = (char *) malloc (strlen(arg) + 20);
      char * username = (char *) malloc (40);
      char * user = username;
      char * temp = arg;
      temp++;

      while(*temp != '/' && *temp) {
	//get the username up to the slash
	*username = *temp;
	*(username++);
	*(temp++);
      }
      *username = '\0';

      if(*temp){
	//get directory after slash
	dir = strcat(getpwnam(user)->pw_dir, temp);
	arg = strdup(dir);
	return arg;
      }else{
	//get user directory
	arg = strdup(getpwnam(user)->pw_dir);
	return arg;
      }
    }
  }
  return NULL;
}

void SimpleCommand::insertArgument( std::string * argument ) {
  //check for environment variable expansion
  char * temp = new char[strlen(argument->c_str())];
  strcpy(temp, argument->c_str());
  //printf("argument: %s\n", argument);
  //printf("temp: %s\n", temp);
  char * variable = environmentVariableCheck(temp);
  //printf("variable: %s\n", variable);
  if(variable){
    *argument = variable;
  }

  //check for tilde
  char * tilde = tildeCheck(temp);
  if(tilde){
    *argument = tilde;
  }

  _arguments.push_back(argument);
}

// Print out the simple command
void SimpleCommand::print() {
  for (auto & arg : _arguments) {
    std::cout << "\"" << arg << "\" \t";
  }
  // effectively the same as printf("\n\n");
  std::cout << std::endl;
}
