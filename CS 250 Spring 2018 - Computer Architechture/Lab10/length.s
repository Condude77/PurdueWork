.data
fmt:		.asciz "%s"		//format for reading
printfmt:	.asciz "%d\n"	//format for printing
str:		.word 0			//where length is stored
.text
.global main

main:

	push {r4-r9, fp, lr}
	
	ldr r0, =fmt	//loads read format
	ldr r1, =str	//loads length
	bl scanf
	ldr r0, =str	//loads string in r0

	mov r5, #0		//sets counter to 0

loop:
	ldr r3, [r0]	//loads first char
	cmp r3, #0		//null check
	beq end			//breaks if null
	add r5, r5, #1	//increments pointer
	add r0, r0, #1	//increments counter
	b loop			

end:
	ldr r0, =printfmt	//loads message
	mov r1, r5			//loads length
	bl printf

	pop {r4-r9, fp, pc}
