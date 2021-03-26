/* getppid.c - getppid */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getppid  -  Return the ID of the given process's parent
 *------------------------------------------------------------------------
 */
pid32	getppid(pid32 pid)
{
	pid32 ppid;					// Parent process id to return
	intmask mask;					// Saved interrupt mask

	mask = disable();				// Disable interrupts
	if (isbadpid(pid)) {				// Check for bad process id
		restore(mask);
		return (pid32) SYSERR;
	}
	
	struct procent myProcent = proctab[pid];	// Get process table
	ppid = myProcent.prparent;			// Get parent id stored in process table

	restore(mask);					// Enable interrupts
	return ppid;
}
