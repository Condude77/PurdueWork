/* testcpu.c - testcpu */

#include <xinu.h>

/*------------------------------------------------------------------------
 * testcpu - a CPU-bound program to test Complete Fair Scheduling for lab 3 part 5.1
 *------------------------------------------------------------------------
 */
void testcpu(void){
	int x, y;
	x = 0;
	y = clktimemilli;
	while(clktimemilli - y < 8000){ // until wall clock time of 8 sec is reached do
		x++;
	}
	kprintf("cpu: %d %d %d %d\n", currpid, x, proctab[currpid].prgrosscpu, procgrosscpumicro(currpid));
}
