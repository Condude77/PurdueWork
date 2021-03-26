/* meminsert.c - meminsert */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  meminsert  -  insert memory block into list for Lab 5 part 4.2
 *------------------------------------------------------------------------
 */
void meminsert(struct inusememblk *hdptr, char *memptr){
	memblockkflag = 1;

	struct procent *prptr = &proctab[currpid];

	hdptr->memblockptr = memptr;
	hdptr->mnext = prptr->prheaphd->mnext;
	hdptr->memlen = (uint32)roundmb(memptr);
	prptr->prheaphd->mnext = hdptr;
	prptr->prheaphd->memlen = (uint32)roundmb(memptr);
	
}
