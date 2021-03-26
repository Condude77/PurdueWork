/* proclifetime.c - proclifetime */

#include <xinu.h>


/*------------------------------------------------------------------------
 *  proclifetime  -  returns the lifetime of a process for lab 3 part 3.2
 *------------------------------------------------------------------------
 */
uint32 proclifetime(pid32 pid)
{
	uint32 birth;					// birthtime of process
	uint32 lifetime;				// lifetime to be returned
	intmask mask;					// interrupt mask

	mask = disable();				// disable interrupts

	if (isbadpid(pid)) {				// Check for bad process id
		restore(mask);
		return (pid32) SYSERR;
	}
	
	birth = proctab[pid].prbirth;			// Get process table entry for birth time
	lifetime = clktimemilli - birth;

	restore(mask);					// restore interrupts
	return lifetime;
}
