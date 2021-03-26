/*  main.c  - main, sndch*/

#include <xinu.h>

void sndch(char);
extern void testcpu(void);
extern void testio(void);

process	main(void)
{
    
    	/*kprintf("\nHello World!\n");
    	kprintf("\nI'm the first XINU app and running function main() in system/main.c.\n");
    	kprintf("\nI was created by nulluser() in system/initialize.c using create().\n");
    	kprintf("\nMy creator will turn itself into the do-nothing null process.\n");
    	kprintf("\nI will create a second XINU app that runs shell() in shell/shell.c as an example.\n");
    	kprintf("\nYou can do something else, or do nothing; it's completely up to you.\n");
    	kprintf("\n...creating a shell\n"); */

	//kprintf("%d\n", 12/0);	//divide-by-zero test for Lab 1 part 3.4

	//Lab 2 part 3.2
	//resume(create(sndch, 1024, 20, "send A", 1, 'A'));
	//resume(create(sndch, 1024, 10, "send B", 1, 'B'));
		
	resume(create(testcpu, 1024, 10, "CPU 1", 0));
	sleepms(500);
	resume(create(testcpu, 1024, 10, "CPU 2", 0));
	sleepms(500);
	resume(create(testcpu, 1024, 10, "CPU 3", 0));
	sleepms(500);
	resume(create(testcpu, 1024, 10, "CPU 4", 0));
	//resume(create(testio, 1024, 10, "I/O 1", 0));
	//resume(create(testio, 1024, 10, "I/O 2", 0));
	//resume(create(testio, 1024, 10, "I/O 3", 0));
	//resume(create(testio, 1024, 10, "I/O 4", 0));
	/* Run the Xinu shell */

	//recvclr();
	//resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */
    /*
	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    */
}

//Process added for Lab 2 part 3.2
/*------------------------------------------------------------------------
* sndch - Output a character on a serial device indefinitely
*------------------------------------------------------------------------
*/
void sndch(char ch) /* The character to emit continuously */
{
	while(1){
		//kprintf("pid: %d     %d\n", wgetpid(), getpid());
		uint32 i = procgrosscpu(currpid);
		uint32 j = procgrosscpumicro(currpid);		
		kprintf("cpu%c: milli:%d  micro:%d\n", ch, i, j); //kputc(ch);
	}
}
