/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
	prptr->prstate = PR_READY;
	//rinsert(pid, readylist, prptr->prprio);
	//kprintf("process pid: %d inserted in ready.c\n", pid);	
	rinsert(pid, readylist, prptr->prvgrosscpu);	/* Changed to rinsert for lab 3 part 4.2 and changed prprio to prvgrosscpu for lab 3 part 4.3 */
	resched();

	return OK;
}
