/*  main.c  - main */

#include <xinu.h>

extern void victimA(void);
extern void attackerA(int);
extern void attackerB(int);

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

	/* Testing for lab 4 part 3.3	*/

/*	pid32 receiver  = create(receiveMessage, 1000, 20, "receiver 1", 1, 7);
	pid32 receiver2  = create(receiveMessage, 1000, 10, "receiver 2", 1, 7); 

	pid32 sender1   = create(sendMessage, 1000, 20, "sender 1", 2, receiver, '1'); 
	pid32 sender2   = create(sendMessage, 1000, 20, "sender 2", 2, receiver, '2'); 
	pid32 sender3   = create(sendMessage, 1000, 20, "sender 3", 2, receiver, '3'); 
	pid32 sender4   = create(sendMessage, 1000, 20, "sender 4", 2, receiver2, '4'); 
	pid32 sender5   = create(sendMessage, 1000, 20, "sender 5", 2, receiver2, '5'); 
	pid32 sender6   = create(sendMessage, 1000, 20, "sender 6", 2, receiver2, '6');

	resume(sender1);
	resume(sender2);
	resume(sender3);
	resume(sender4);
	resume(sender5);
	resume(sender6);

	resume(receiver);
	resume(receiver2); */

	resume(create(victimA, 1000, 20, "Victim 1", 0));
	resume(create(victimA, 1000, 20, "Victim 2", 0));
	resume(create(victimA, 1000, 20, "Victim 3", 0));

	resume(create(attackerA, 1000, 20, "Attacker 1", 1, 1));
	/* Wait for shell to exit and recreate it */
/*
	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}*/
	return OK;
    
}

void sendMessage(pid32 rPID, char msg) {
	bsend(rPID, msg);
}

void receiveMessage(int number) {
	char msg = ' ';
	int  i = 0;

	for (i = 0; i < number; i++) {
		msg = receive();
		kprintf("Message received from sender %d: \"%c\"\n", i+1, msg);	
	}
}

