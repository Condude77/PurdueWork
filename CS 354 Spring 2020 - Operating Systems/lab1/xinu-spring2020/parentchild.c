#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/time.h>
#include <sys/resource.h>
void forkexample() 
{ 
    int which = PRIO_PROCESS;
    id_t pid;
    int ret;
    // child process because return value zero 
    if (fork() == 0){ 
	pid = getpid();
	ret = getpriority(which, pid);        
	write(1, "Child\n", 6); 
	printf("%d\n", ret);
  
    // parent process because return value non-zero. 
    } else {
	pid = getpid();
	ret = getpriority(which, pid);         
	write(1, "Parent\n", 7);
	printf("%d\n", ret);
    }
} 
int main() 
{ 
    forkexample(); 
    return 0; 
} 
