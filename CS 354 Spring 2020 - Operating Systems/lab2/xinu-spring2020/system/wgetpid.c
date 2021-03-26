/* wgetpid.c - wgetpid */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wgetpid  -  Return the ID of the currently executing process
 *------------------------------------------------------------------------
 */
pid32	wgetpid(void)
{
	pid32 out;			// Output to return
	asm ("movl $4, %%eax\n\t"	// Set eax to 4
		"int $33\n\t"		// Jump to _Xint33 in intr.S
		"movl %%eax, %0"	// set out to value saved in eax
		: "=r" (out)
		: /* no input registers */
		: /* no collision */ );
	return out;
}
