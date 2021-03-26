.section .data
.section .text

u_divide_by_10:

	ldr r1, .Lu_magic_number_10				//sets format for hex value
	umull r1, r2, r1, r0					//sets r1 with lower 32 bits and r2 with upper 32 bits of hex value
	mov r0, r2, LSR #3						//shift r2 by 3 and store in r0
	b ihatethis
	.align 4
	.Lu_magic_number_10:.word 0xcccccccd	//format of hex

.global printx
.global printd

printx:
	push {fp, lr}
	mov r3, r0				//set r3 to r0
	mov r5, #0				//set counter to 0

how:
	lsl r0, r0, #28			//clears all but LSB
	lsr r0, r0, #28			//puts back to original value
	cmp r0, #10				//compares if r0 to 10
	blt does				//if r0 < 10 branch
	add r0, r0, #87			//add 87 to r0 to get ascii value
	b one					//branch

does:
	add r0, r0, #48			//add 48 to r0 to get ascii value

one:
	push {r0}				//push r0 to stack
	add r5, r5, #1			//increment counter
	lsr r3, r3, #4			//shift r3 right 4 bits
	mov r0, r3				//sets r0 to r3
	cmp r3, #0				//compare r3 to 0
	bne how					//branch if r3 does not equal 0
	b printthings			//branch

printthings:
	pop {r0}				//pop r0 from stack
	sub r5, r5, #1			//decrement counter
	bl putchar				//print value
	cmp r5, #0				//check for end
	bne printthings			//branch if not end
	
	pop {fp, lr}

printd:
	push {fp, lr}

start:
	mov r3, r0				//sets r3 to r0
	lsr r3, r3, #31			//shift right to the most significant bit
	cmp r3, #1				//compare r3 to 1
	bne sequel				//branch if r3 does not equal 1
	mov r10, #0xFFFFFFFF	//sets r10 to max hex value
	sub r0, r10, r0			//subtracts r0 from r10
	add r0, r0, #1			//add 1 to r0
	mov r10, #100			//sets r10 to 100
	b sequel				//branch

sequel:
	mov r4, r0				//sets r4 to r0
	mov r5, #0				//sets r5 to 0
	mov r9, #10				//sets r9 to 10

what:
	b u_divide_by_10		//branch

ihatethis:
	mov r3, r0				//set r3 to r0
	mul r8, r0, r9			//multiply r0 with r9
	sub r7, r4, r8			//subtract r4 from r8
	push {r7}				//stores r7 on stack
	add r5, r5, #1			//increments counter
	cmp r3, #0				//checks for empty number
	mov r4, r3				//sets r4 to r3
	mov r0, r3				//sets r0 to r3
	bne what				//branch if r3 is not 0
	cmp r10, #100			//check if r10 is 100
	bne do					//branch if r10 is not 100
	mov r10, #-3			//set r10 to -3
	push {r10}				//push r10 to stack
	add r5, r5, #1			//increment counter

do:
	pop {r0}				//pop r0 from stack
	sub r5, r5, #1			//decrement counter
	add r0, r0, #48			//add 48 to r0 to get ascii value
	bl putchar				//print
	cmp r5, #0				//check for end of number
	bne do					//branch if not end

	pop {fp, pc}
