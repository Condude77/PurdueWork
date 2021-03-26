/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;
uint32 currentgrosscpu = 0;	/* variable to track gross cpu time consumed for Lab 3 part 3.3	*/
uint64 currentgrosscputick = 0;	/* variable to track gross cpu time in cpu ticks consumed for Lab 3 part 3.5	*/

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	//TODO: add check for null process
	//kprintf("pnew: %d\n", currpid);
	if(currpid != 0){
		/* Determine gross cpu usage for a process for lab 3 part 3.3	*/
		if(clktimemilli == currentgrosscpu){	/* Check if current process has used less than one msec	*/
			ptold->prgrosscpu++;	/* Round up to 1 msec	*/
			ptold->prvgrosscpu++;
		}else{
			ptold->prgrosscpu += clktimemilli - currentgrosscpu;	/* Else add difference between clktimemilli and current gross cpu	*/
			ptold->prvgrosscpu += clktimemilli - currentgrosscpu;
		}

	}
	//kprintf("pid: %d  prv: %d\n", currpid, ptold->prvgrosscpu);
	/* Determine gross cpu usage in cpu ticks for a process for lab 3 part 3.5	*/
	uint64 timetick = 0;
	asm volatile ( "rdtsc" : "=A"(timetick) );
	ptold->prgrosscputick += timetick - currentgrosscputick;

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		//kprintf("ptold: %d    firstkey: %d\n", ptold->prvgrosscpu, firstkey(readylist));
		//if (ptold->prprio > firstkey(readylist)) {
		if(ptold->prvgrosscpu < firstkey(readylist)){		/* Changed to check for prvgrosscpu instead of prprio for lab 3 part 4.3 */
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		//rinsert(currpid, readylist, ptold->prprio);
		//kprintf("process currpid: %d inserted in resched.c\n", currpid);
		rinsert(currpid, readylist, ptold->prvgrosscpu);	/* Changed to rinsert for lab 3 part 4.2 and changed prprio to prvgrosscpu for lab 3 part 4.3*/
		//kprintf("after rinsert\n");
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
	//kprintf("after dequeue: %d\n", currpid);
	
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr); /* Context switch to new process	*/

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}

/*------------------------------------------------------------------------
 *  resched_dummy  -  Dummy function for updating grosscputick for lab 3 part 3.3 and 3.5
 *------------------------------------------------------------------------
 */
void resched_dummy()
{
	currentgrosscpu = clktimemilli;
	asm volatile ( "rdtsc" : "=A"(currentgrosscputick) );
	return;
}
