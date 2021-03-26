#ifndef simplcommand_hh
#define simplecommand_hh

#include <string>
#include <vector>

struct SimpleCommand {

  // Simple command is simply a vector of strings
  std::vector<std::string *> _arguments;

  SimpleCommand();
  ~SimpleCommand();
  char * environmentVariableCheck(char * argument);
  char * tildeCheck(char * argument);
  void insertArgument( std::string * argument );
  void print();
};

#endif
