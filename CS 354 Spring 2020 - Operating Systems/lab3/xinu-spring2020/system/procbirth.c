/* procbirth.c - procbirth */

#include <xinu.h>


/*------------------------------------------------------------------------
 *  procbirth  -  returns the birthday of a process for lab 3 part 3.2
 *------------------------------------------------------------------------
 */
uint32 procbirth(pid32 pid)
{
	uint32 birth;					// value to be returned
	intmask mask;					// interrupt mask

	mask = disable();				// disable interrupts

	if (isbadpid(pid)) {				// Check for bad process id
		restore(mask);
		return (pid32) SYSERR;
	}
	
	birth = proctab[pid].prbirth;			// Get process table entry for birth time

	restore(mask);					// restore interrupts
	return birth;
}
