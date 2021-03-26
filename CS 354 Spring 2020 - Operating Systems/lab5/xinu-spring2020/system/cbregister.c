/* cbregister.c - cbregister */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  cbregister  -  Register a callback function for Lab5 part 3.2
 *------------------------------------------------------------------------
 */
syscall cbregister(void (* fp)(void), umsg32 *mbufptr)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	mask = disable();		/* Disable Interrupts	*/
	/* Check if fp is not in text segment and check if mbufptr points inside text	*/	
	if((fp < text || fp > etext) || (mbufptr >= text && mbufptr <= etext)){
		restore(mask);
		return SYSERR;
	}
	prptr = &proctab[currpid];
	prptr->prcbvalid = 1;		/* We have valid callback function	*/
	prptr->prmbufptr = mbufptr;	/* Set pointer to message buffer	*/
	prptr->prcbptr = fp;		/* Set pointer to callback function	*/

	restore(mask);			/* Enable Interrupts	*/
	return OK;
}
