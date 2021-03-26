
/*
 *
 * CS-252 
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */

%{

#include <cstring>
#include "y.tab.hh"

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

[^ \t\n][^ \t\n]*  {
  /* Assume that file names have only alpha chars */
  yylval.cpp_string = new std::string(yytext);
  return WORD;
}
