/* procgrosscpu.c - procgrosscpu */

#include <xinu.h>


/*------------------------------------------------------------------------
 *  procgrosscpu -  returns the gross CPU usage of a process for lab 3 part 3.3
 *------------------------------------------------------------------------
 */
uint32 procgrosscpu(pid32 pid)
{
	uint32 usage;					// value to be returned
	intmask mask;					// interrupt mask

	mask = disable();				// disable interrupts

	if (isbadpid(pid)) {				// Check for bad process id
		restore(mask);
		return (pid32) SYSERR;
	}
	
	usage = proctab[pid].prgrosscpu;		// Get process table entry for gross cpu usage

	restore(mask);					// restore interrupts
	return usage;
}
