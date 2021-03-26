
/*
 * CS-252
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */

%code requires 
{
#include <string>

#if __cplusplus > 199711L
#define register      // Deprecated in C++11 so remove the keyword
#endif
}

%union
{
  char        *string_val;
  // Example of using a c++ type in yacc
  std::string *cpp_string;
}

%token <cpp_string> WORD
%token NOTOKEN GREAT NEWLINE LESS PIPE GREATGREAT GREATAMPERSAND GREATGREATAMPERSAND AMPERSAND TWOGREAT

%{
//#define yylex yylex
#include <cstdio>
#include <regex.h>
#include <string.h>
#include <dirent.h>
#include "shell.hh"

void yyerror(const char * s);
int yylex();

void expandWildCardsIfNecessary(std::string * arg);
void expandWildCards(char * prefix, char * arg);
int cmpfunc(const void * file1, const void * file2);

%}

%%

goal:
  commands
  ;

commands:
  command
  | commands command
  ;

command: simple_command
       ;

simple_command:	
  pipe_list iomodifier_list background NEWLINE {
    /* printf("   Yacc: Execute command\n"); */
    Shell::_currentCommand.execute();
  }
  | NEWLINE {
  }
  | error NEWLINE { yyerrok; }
  ;

command_and_args:
  command_word argument_list {
    Shell::_currentCommand.
    insertSimpleCommand( Command::_currentSimpleCommand );
  }
  ;

argument_list:
  argument_list argument
  | /* can be empty */
  ;

argument:
  WORD {
    /* printf("   Yacc: insert argument \"%s\"\n", $1->c_str()); */
    /* expandWildCardsIfNecessary( $1 ); */
    Command::_currentSimpleCommand->insertArgument($1);
  }
  ;

command_word:
  WORD {
    /* printf("   Yacc: insert command \"%s\"\n", $1->c_str()); */
    Command::_currentSimpleCommand = new SimpleCommand();
    Command::_currentSimpleCommand->insertArgument( $1 );
  }
  ;

iomodifier_opt:
  GREAT WORD {
    /* printf("   Yacc: insert output \"%s\"\n", $2->c_str()); */
    Shell::_currentCommand._outFile = $2;
    Shell::_currentCommand._outCount++;
  } /* Part 1A */
  | LESS WORD{
	/* printf("   Yacc: insert input \"%s\"\n", $2->c_str()); */
	Shell::_currentCommand._inFile = $2;
	/* Shell::_currentCommand._inCount++; */
  }
  | GREATGREAT WORD {
	/* printf("   Yacc: insert output \"%s\"\n", $2->c_str()); */
	Shell::_currentCommand._outFile = $2;
	Shell::_currentCommand._append = 1;
	Shell::_currentCommand._outCount++;
  }
  | GREATAMPERSAND WORD {
	/* printf("   Yacc: insert output \"%s\"\n", $2->c_str()); */
	Shell::_currentCommand._outFile = $2;
	Shell::_currentCommand._errFile = $2;
	Shell::_currentCommand._outCount++;
  }
  | GREATGREATAMPERSAND WORD {
	/* printf("   Yacc: insert output \"%s\"\n", $2->c_str()); */
	Shell::_currentCommand._outFile = $2;
	Shell::_currentCommand._errFile = $2;
	Shell::_currentCommand._append = 1;
	Shell::_currentCommand._outCount++;
  }
  | TWOGREAT WORD {
	Shell::_currentCommand._errFile = $2;
  }
  ;

pipe_list:
  pipe_list PIPE command_and_args
  | command_and_args
  ;

background:
  AMPERSAND {
    Shell::_currentCommand._background = true;
  }
  |
  ;

iomodifier_list:
  iomodifier_list iomodifier_opt
  | iomodifier_opt
  |
  ;

%%

int maxEntries = 20;
int currentEntries = 0;
char ** entries;

void expandWildCardsIfNecessary(std::string * arg)
{
  maxEntries = 20;
  currentEntries = 0;
  entries = (char **) malloc (maxEntries * sizeof(char *));
  char * copy = new char[strlen(arg->c_str())];
  strcpy(copy, arg->c_str());

  if(strchr(copy, '*') || strchr(copy, '?')){
    expandWildCards(NULL, copy);
    qsort(entries, currentEntries, sizeof(char *), cmpfunc);
    std::string * ent;
    for(int i = 0; i < maxEntries; i++){
      Command::_currentSimpleCommand->insertArgument((std::string *)entries[i]);
    }
  }else{
    Command::_currentSimpleCommand->insertArgument(arg);
  }
  return;
}

void expandWildCards(char * prefix, char * arg)
{
  char * temp = arg;
  char * exp = (char *) malloc (strlen(arg) + 10);
  char * dir = exp;

  if(temp[0] == '/'){
    *exp = *temp;
    exp++;
    temp++;
  }
  *exp = '\0';

  if(strchr(dir, '*') || strchr(dir, '?')){
    if(!prefix && arg[0] == '/'){
      prefix = strdup("/");
      dir++;
    }

    /* convert wildcard to regular expression */
    char * reg = (char *) malloc (2 * strlen(arg) + 10);
    char * a = dir;
    char * r = reg;
    *(r++) = '^';
    while(*a){
      if(*a == '*'){
	*r = '.';
	r++;
	*r = '*';
	r++;
      }else if(*a == '?'){
	*r = '.';
	r++;
      }else if(*a == '.'){
	*r = '\\';
	r++;
	*r = '.';
	r++;
      }else{
	*r = *a;
	r++;
      }
      a++;
    }
    *r = '$';
    r++;
    *r = '\0';

    /* compile regular expression */
    regex_t re;
    int expbuf = regcomp(&re, reg, REG_EXTENDED|REG_NOSUB);
    if(!expbuf){
      perror("regcomp");
      return;
    }
    
    /* list directory and add as arguments the entries */
    char * path = strdup((prefix)?prefix:".");
    DIR * d = opendir(path);
    if(d == NULL){
      perror("opendir");
      return;
    }
    struct dirent * ent;
    regmatch_t match;
    while((ent = readdir(d)) != NULL){
      /* check if name matches */
      if(!regexec(&re, ent->d_name, 1, &match, 0)){
	if(*temp){
	  if(ent->d_type == DT_DIR){
	    char * newPrefix = (char *) malloc (200);
	    if(!strcmp(path, ".")){
	      newPrefix = strdup(ent->d_name);
	    }else if(!strcmp(path, "/")){
	      sprintf(newPrefix, "%s%s", path, ent->d_name);
	    }else{
	      sprintf(newPrefix, "%s%s", path, ent->d_name);
	    }
	    expandWildCards(newPrefix, (*temp == '/')?++temp:temp);
	  }
	}else{
	  /* increase amount of entries if necessary */
	  if(currentEntries == maxEntries) {
	    maxEntries *= 2;
	    entries = (char **) realloc (entries, maxEntries * sizeof(char *));
	  }
	  char * newArg = (char *) malloc (100);
	  newArg[0] = '\0';
	  if(prefix){
	    sprintf(newArg, "%s%s", prefix, ent->d_name);
	  }
	  if(ent->d_name[0] == '.'){
	    if(arg[0] == '.'){
	      entries[currentEntries] = (newArg[0] != '\0')?strdup(newArg):strdup(ent->d_name);
	      currentEntries++;
	    }
	  }else{
	    entries[currentEntries] = (newArg[0] != '\0')?strdup(newArg):strdup(ent->d_name);
	    currentEntries++;
	  }
	}
      }
    }
    closedir(d);	
  }else{
    char * newPrefix = (char *) malloc (100);
    if(prefix){
      sprintf(newPrefix, "%s%s", prefix, dir);
    }else{
      newPrefix = strdup(dir);
    }
    if(*temp){
      expandWildCards(newPrefix, ++temp);
    }
  }
}

int cmpfunc(const void * file1, const void * file2)
{
  const char *_file1 = *(const char **)file1;
  const char *_file2 = *(const char **)file2;
  return strcmp(_file1, _file2);
}

void
yyerror(const char * s)
{
  fprintf(stderr,"%s", s);
}

#if 0
main()
{
  yyparse();
}
#endif
