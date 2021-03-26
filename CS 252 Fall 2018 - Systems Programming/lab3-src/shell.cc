#include <cstdio>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "shell.hh"

void yyrestart(FILE * file);
int yyparse(void);

void Shell::prompt() {
  if (isatty(0)) {
    printf("myshell> ");
    fflush(stdout);
  }
}

extern "C" void controlC(int sig){
  printf("\n");
  Shell::prompt();
}

extern "C" void zombieP(int sig){
  int pid = wait3(0, 0, NULL);
  while(waitpid(-1, NULL, WNOHANG) > 0);
  if(pid != -1){
    //printf("[%d] exited.\n", pid);
  }
}

int main() {
  //check for controlC
  struct sigaction sigact1;
  sigact1.sa_handler = controlC;
  sigemptyset(&sigact1.sa_mask);
  sigact1.sa_flags = SA_RESTART;
  if(sigaction(SIGINT, &sigact1, NULL)){
    perror("sigaction");
    exit(2);
  }

  //check for zombie process
  struct sigaction sigact2;
  sigact2.sa_handler = zombieP;
  sigemptyset(&sigact2.sa_mask);
  sigact2.sa_flags = SA_RESTART;
  if(sigaction(SIGCHLD, &sigact2, NULL)){
    perror("sigaction");
    exit(2);
  }

  //load the shellrc
  /*FILE* fd = fopen("./u/data/u87/brow1325/cs252/lab3-src/shellrc", "r");
  if(fd){
    yyrestart(fd);
    yyparse();
    yyrestart(stdin);
    fclose(fd);
  }else{*/
    Shell::prompt();
  //}
  yyparse();
}

Command Shell::_currentCommand;
