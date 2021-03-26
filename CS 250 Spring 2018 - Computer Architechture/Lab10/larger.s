.data
printfmt:	.asciz "%d\n\000"	//format for printing
fmt:	.asciz "%d\000"	//format for numbers to be read
num1:	.word 1			//storage for number 1
num2:	.word 2			//storage for number 2
.text

.global main

main:

	push {ip, lr}

	ldr r0, =fmt		//sets r0 to proper format
	ldr r1, =num1		//sets num1 to r1
	bl scanf

	ldr r0, =fmt		//sets r0 to proper format
	ldr r1, =num2		//sets num2 to r1
	bl scanf

	ldr r0, =num1		//sets r0 to num1
	ldr r0, [r0]		//sets r0 to r0 address
	ldr r1, =num2		//sets r1 to num2
	ldr r1, [r1]		//sets r1 to address r1

	cmp r0, r1			//compares r0 and r1
	movgt r1, r0		// if (r0 > r1) r1 = r0
	ldr r0, =printfmt	//sets r0 to print format
	bl printf

pop {ip, pc}
