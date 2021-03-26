.data
fmt:		.asciz "%d"
printfmt:	.asciz "%d\n"
num:		.word 0
.text
.global main

main:

	push {r4-r9, fp, lr}

	mov r2, #0
	mov r5, #0	//sets sum to 0

loop:

	ldr r0, =fmt
	ldr r1, =num
	bl scanf

	ldr r9, =num
	ldr r1, [r9]
	ldr r7, r1

	cmp r1, #0

//this code is unfinished and does not work
