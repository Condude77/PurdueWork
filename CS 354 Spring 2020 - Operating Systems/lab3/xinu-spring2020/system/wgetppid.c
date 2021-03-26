/* wgetppid.c - wgetppid */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wgetppid  -  Return the ID of the given process's parent
 *------------------------------------------------------------------------
 */
pid32	wgetppid(pid32 pid)
{
	pid32 ppid;			// Parent process id to return
	asm ("movl $6, %%eax\n\t"	// Set eax to 6
		"movl %1, %%ebx\n\t"	// Set ebx to the pid
		"int $33\n\t"		// Jump to _Xint33 in intr.S
		"movl %%eax, %0"	// Set ppid to value saved in eax
		: "=r" (ppid)
		: "r" (pid)
		: "eax", "ebx");
	return ppid;
}
