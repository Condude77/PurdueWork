/* rinsert.c - rinsert */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  rinsert  -  Insert a process into a queue in ascending key order for Lab 3 part 4.2
 *------------------------------------------------------------------------
 */
status	rinsert(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q,		/* ID of queue to use		*/
	  int32		key		/* Key for the inserted process	*/
	)
{
	qid16	curr;			/* Runs through items in a queue*/
	qid16	prev;			/* Holds previous node index	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	curr = firstid(q);
	//kprintf("rinsert- curr: %d queuekey: %d ourkey: %d\n", curr, queuetab[curr].qkey, key);
	while (queuetab[curr].qkey <= key) {	// loop through queue until the next entry is higher
		curr = queuetab[curr].qnext;	// than the priority of the process we want to insert
		//kprintf("in loop- curr: %d queuekey: %d ourkey: %d\n", curr, queuetab[curr].qkey, key);
	}

	/* Insert process between curr node and previous node */

	prev = queuetab[curr].qprev;	/* Get index of previous node	*/
	queuetab[pid].qnext = curr;
	queuetab[pid].qprev = prev;
	queuetab[pid].qkey = key;
	queuetab[prev].qnext = pid;
	queuetab[curr].qprev = pid;
	return OK;
}
