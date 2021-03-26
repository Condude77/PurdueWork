/*  main.c  - main, sndch*/

#include <xinu.h>

void sndch(char);

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
	resume(create(sndch, 1024, 20, "send A", 1, 'A'));
	resume(create(sndch, 1024, 10, "send B", 1, 'B'));

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
	//while(1){
		//kprintf("pid: %d     %d\n", wgetpid(), getpid());
		int i = wgetppid(currpid);		
		kprintf("prio: %d\n", i); //kputc(ch);
	//}
}
