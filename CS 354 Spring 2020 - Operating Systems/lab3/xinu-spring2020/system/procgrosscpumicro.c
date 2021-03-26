/* procgrosscpumicro.c - procgrosscpumicro */

#include <xinu.h>


/*------------------------------------------------------------------------
 *  procgrosscpumicro -  returns the gross CPU usage of a process in microseconds for lab 3 part 3.5
 *------------------------------------------------------------------------
 */
uint32 procgrosscpumicro(pid32 pid)
{
	uint32 ticks;					// value of ticks for process
	uint32 nano;					// value in nanoseconds
	uint32 micro;					// value to be returned in microseconds
	intmask mask;					// interrupt mask

	mask = disable();				// disable interrupts

	if (isbadpid(pid)) {				// Check for bad process id
		restore(mask);
		return (pid32) SYSERR;
	}
	
	
	ticks = proctab[pid].prgrosscputick;		// Get process table entry for gross cpu tick usage
	nano = ticks * 2.5;				// Convert cpu ticks to nanoseconds
	micro = (uint32) (nano / 1000);			// Convert nanoseconds to microseconds

	restore(mask);					// restore interrupts
	return micro;
}
