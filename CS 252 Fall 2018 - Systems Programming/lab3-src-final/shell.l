
/*
 *
 * CS-252 
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */

%{

#include <cstring>
#include <unistd.h>
#include "y.tab.hh"
#include "command.hh"

extern "C" char * read_line();

int mygetc(FILE * f) {
 static char *p;
 char ch;

 if (!isatty(0)) {
   return getc(f);
 }

 if (p==NULL || *p == 0) {
   char * s = read_line();
   p = s;
 }

 ch = *p;
 p++;

 return ch;
}

#undef getc
#define getc(f) mygetc(f)

static  void yyunput (int c,char *buf_ptr  );

void myunputc(int c) {
  unput(c);
}

%}

%%

\n {
  return NEWLINE;
}

[ \t] {
  /* Discard spaces and tabs */
}

">" {
  return GREAT;
}

  /* Part 1A */

  /* Input */
"<" {
  return LESS;
}
  /* Pipe */
"|" {
  return PIPE;
}
  /* Append Out */
">>" {
  return GREATGREAT;
}
  /* Error Out */
">&" {
  return GREATAMPERSAND;
}
  /* Append Error*/
">>&" {
  return GREATGREATAMPERSAND;
}
  /* Background */
"&" {
  return AMPERSAND;
}
"2>" {
  return TWOGREAT;
}

  /* Check for subshell */
[$(][^\n]*[)] {
  char * cmd = strdup(yytext + 2);
  cmd[strlen(cmd) - 1] = '\0';
  int tempin = dup(0);
  int tempout = dup(1);

  int fdpipein[2];
  pipe(fdpipein);
  write(fdpipein[1], cmd, strlen(cmd));
  write(fdpipein[1], "\nsexit\n", 7);
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
  char * buffer = (char *) malloc (4000);
  int i = 0;
  while(read(fdpipeout[0], &ch, 1)){
    if(ch == '\n'){
      buffer[i++] = ' ';
    }else{
      buffer[i++] = ch;
    }
  }
  buffer[i] = '\0';
  for(i = strlen(buffer) - 1; i >= 0; i--){
    unput(buffer[i]);
  }
}
  /* Check for quotations in argument */
\"[^\n\"]*\" {
  yytext[strlen(yytext + 1)] = '\0';
  yylval.cpp_string = new std::string(yytext + 1);
  return WORD;
}
  /* Check for escape characters */
[^ \t\n|><]*\\[^ \t\n]* {
  int i = 0;
  char * newstr = (char *) malloc (400);
  char * temp = yytext;
  while (*temp) {
    if (*temp == '\\') {
      temp++;
      newstr[i] = *(temp);
    } else {
      newstr[i] = *temp;
    }
    i++;
    temp++;
  }
  newstr[i] = '\0';
  yylval.cpp_string = new std::string(newstr);
  return WORD;
}
[^ \t\n|><][^ \t\n|><]* {
  /* Assume that file names have only alpha chars */
  yylval.cpp_string = new std::string(yytext);
  return WORD;
}
