/* bsend.c - bsend */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  bsend  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	bsend(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptrR;	/* Ptr to receiving process's table entry	*/
	struct 	procent *prptrS;	/* Ptr to sending process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptrR = &proctab[pid];
	prptrS = &proctab[currpid];
	
	if (prptrR->prhasmsg) {		/* Check for blocked message	*/
		prptrS->prstate = PR_SND;		
		prptrS->prsndmsg = msg;
		prptrS->prsenderflag = 1;
		
		enqueue(currpid, prptrR->prblockedsenders);	/* Insert sender into queue	*/
		resched();
	}
	prptrR->prmsg = msg;		/* Deliver message		*/
	prptrR->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptrR->prstate == PR_RECV) {
		ready(pid);
	} else if (prptrR->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}
