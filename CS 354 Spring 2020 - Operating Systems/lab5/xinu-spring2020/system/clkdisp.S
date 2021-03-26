/* clkdisp.s - clkdisp (x86) */

/*------------------------------------------------------------------------
 * clkdisp  -  Interrupt dispatcher for clock interrupts (x86 version)
 *------------------------------------------------------------------------
 */
#include <icu.s>
		.text
		.globl	clkdisp		# Clock interrupt dispatcher
clkdisp:
		pushal			# Save registers
		cli			# Disable further interrupts
		movb	$EOI,%al	# Reset interrupt
		outb	%al,$OCW1_2

		call	clkhandler	# Call high level handler

					# Copy prtmpbuf to msg
					# Reset prtmpvalid to 0
					# Shift EFLAGS by 4 bytes
					# Shift CS by 4 bytes
					# Shift EIP 4 bytes
					# Copy EIP into original EFLAGS location at top of stack
					# Update ESP to to reflect stack growth
					# Replace Address at top of stack with callback address

		sti			# Restore interrupt status
		popal			# Restore registers
		iret			# Return from interrupt
