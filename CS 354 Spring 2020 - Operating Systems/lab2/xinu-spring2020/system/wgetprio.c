/* wgetprio.c - wgetprio */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wgetprio  -  Return the scheduling priority of a process
 *------------------------------------------------------------------------
 */
syscall	wgetprio(
	  pid32		pid		/* Process ID			*/
	)
{
	uint32	prio;			/* Priority to return		*/
	asm ("movl $5, %%eax\n\t"	// Set eax to 5
		"movl %1, %%ebx\n\t"	// Set ebx to the pid
		"int $33\n\t"		// Jump to _Xint33 in intr.S
		"movl %%eax, %0"	// Set prio to value saved in eax
		: "=r" (prio)
		: "r" (pid)
		: "eax", "ebx");
	return prio;
}
