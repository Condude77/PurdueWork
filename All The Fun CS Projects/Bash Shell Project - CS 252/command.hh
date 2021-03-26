#ifndef command_hh
#define command_hh

#include "simpleCommand.hh"

// Command Data Structure

struct Command {
  std::vector<SimpleCommand *> _simpleCommands;
  std::string * _outFile;
  std::string * _inFile;
  std::string * _errFile;
  bool _background;
  int _append;
  int _inCount;
  int _outCount;

  Command();
  void insertSimpleCommand( SimpleCommand * simpleCommand );

  void clear();
  void print();
  void execute();

  int builtInCommand(int cmd);
  void runSource(char* file);

  static SimpleCommand *_currentSimpleCommand;
};

#endif
