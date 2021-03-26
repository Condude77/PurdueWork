/* wakeup.c - wakeup */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wakeup  -  Called by clock interrupt handler to awaken processes
 *------------------------------------------------------------------------
 */
void	wakeup(void)
{
	/* Awaken all processes that have no more time to sleep */

	resched_cntl(DEFER_START);
	while (nonempty(sleepq) && (firstkey(sleepq) <= 0)) {
		/* Process to set a waking process priority to the lowest CPU usage for Lab 3 part 4.3	*/
		pid32 pid = firstid(sleepq);		/* Get pid of first entry in sleep queue	*/
		struct	procent	*prptr;			/* Get process table pointer	*/
		prptr = &proctab[pid];
		/* Find min CPU usage in ready list	*/		
		pid32 minid;
		struct	procent	*minptr;
		if(currpid != 0 && (firstid(readylist) != 0)){		
			minid = firstid(readylist);		/* Get first entry which is the smallest prvgrosscpu	*/
			minptr = &proctab[minid];
			prptr->prvgrosscpu = minptr->prvgrosscpu;
		}else{
			/* Null is either running or is the only entry in the ready list	*/	
			prptr->prvgrosscpu = 0;		/* We want our new process to run next so we initialize to 0	*/
		}
		ready(dequeue(sleepq));
	}

	resched_cntl(DEFER_STOP);
	return;
}
