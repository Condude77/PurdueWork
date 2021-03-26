/* cbunregister.c - cbunregister */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  cbunregister  -  Unregister a callback function for Lab5 part 3.2
 *------------------------------------------------------------------------
 */
syscall cbunregister(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	mask = disable();		/* Disable Interrupts	*/
	prptr = &proctab[currpid];
	if(prptr->prcbvalid == 0){	/* Check if there was no registered cb function	*/
		restore(mask);
		return SYSERR;
	}
	
	prptr->prcbvalid = 0;		/* Set flag to invalid	*/
	restore(mask);
	return OK;
}
