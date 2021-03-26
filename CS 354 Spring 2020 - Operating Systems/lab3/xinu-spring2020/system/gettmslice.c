/* gettmslice.c - gettmslice*/

#include <xinu.h>

/*-----------------------------------------------------------------------------------------
 *  gettmslice  -  Return how much time in msec remains in the current process time slice
 *-----------------------------------------------------------------------------------------
 */
uint32	gettmslice(void)
{
	uint32 tmleft;					// Time left to be returned
	intmask mask;					// Saved interrupt mask

	mask = disable();				// Disable interrupts
	
	tmleft = preempt;				// Get value of preempt

	restore(mask);					// Enable interrupts
	return tmleft;
}
