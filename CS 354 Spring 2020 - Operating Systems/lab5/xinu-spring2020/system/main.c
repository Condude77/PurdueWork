/*  main.c  - main */

#include <xinu.h>

void memtest();

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
	return OK;*/
	
	struct procent *prptr = &proctab[currpid];
	kprintf("Memory List Before: %d\n", memlist.mlength);
	kprintf("MemList b4: %d\n", prptr->prheaphd->memlen);
	resume(create(memtest, 1024, 20, "Mem Test", 0));
	kprintf("Memory List After: %d\n", memlist.mlength);
	kprintf("MemList after: %d\n", prptr->prheaphd->memlen);
    
}

void memtest(){
	void *test1 = getmem(112);
	void *test2 = getmem(1);
	if(freemem(test1, 112) == SYSERR){
		kprintf("Free mem not working correct\n");
	}
}
