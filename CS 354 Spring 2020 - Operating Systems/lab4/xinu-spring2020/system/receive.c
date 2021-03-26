/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/
	struct procent * prptrS;
	pid32 reschedPID;
	pid32 senderPID;
	int isValid = 1;

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/

	/* Added code for lab 4 part 3.2 */
	if(!isempty(prptr->prblockedsenders)){ /* Check that queue is not empty*/
		while(isValid){	//check for valid entry in queue
			if(!isempty(prptr->prblockedsenders)){
				senderPID = dequeue(prptr->prblockedsenders);
				prptrS = &proctab[senderPID];
			
				if(prptrS->prsenderflag == 0){	//The current sender is not valid
					isValid = 1;
				}else{				//The current sender is valid
					reschedPID = senderPID;
					isValid = 0;
				}
			}else{	//The list is empty
				restore(mask);
				return msg;
			}		

		}
		prptr->prmsg = prptrS->prsndmsg;	//Set receiving message buffer to new message
		prptr->prhasmsg == TRUE;		//Set its flag to true because it has a message still
		prptrS->prsenderflag = 0;		//The sender no longer has a message to send
		ready(reschedPID);			//reschedule the sending process
		
	}

	restore(mask);
	return msg;
}
