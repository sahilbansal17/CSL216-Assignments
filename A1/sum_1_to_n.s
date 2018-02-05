	.equ SWI_Exit, 0x11
	.text

@ StoreIntegers - Stores integers (in this case, 1 to 1000) at a destination Address
@ r3 			- Destination Address at which the integers are stored
@ r2 			- Integers (in this case, starting from 1 to 1000)
@ r1 			- Loop counter (in this case, going from 1000 to 1)
@str instruction - store register to memory
@ldr instruction - load register from memory
@latency of the instructions is in cycles and energy is in pJ (pico joules), frequency of the processor = 1GHz

	mov r1, #1000		@Moves constant 1000 to register r1, latency = 1, energy = 100
	mov r2, #1
	ldr r3, =AA		@Loads address of label AA in register r3, latency = 1, energy = 110

StoreIntegers:
	str r2, [r3]		@ Stores the value of register r2 to address stored in register r3, latency = 20, energy = 2000
	add r3, r3, #4 		@ Update Destination Address(4 bytes), latency = 1, energy = 100
	add r2, r2, #1 		@ Update Integer value (add 1), latency = 1, energy = 100
	sub r1, r1, #1 		@ Update Loop counter (sub 1), latency = 1, energy = 100
	cmp r1, #0 	        @ Compare Loop counter with 0, latency = 1, energy = 100
	bne StoreIntegers	@ If Loop counter != 0, keep looping , latency = 2, , energy = 180

@LoadAddIntegers- Loads integers (in this case, 1 to 1000) from a source Address and sums them up.
@ r4 			- Sum register
@ r3 			- Source Address at which the integers are loaded from
@ r2 			- Integers (in this case, starting from 1 to 1000)
@ r1 			- Loop counter

	mov r1, #1000
	mov r4, #0
	ldr r3, =AA
LoadAddIntegers:
	ldr r2, [r3]     	@ Loads integers from address stored in register r3 to register r2, latency = 20, energy = 2000
	add r4, r4, r2          @ Update Sum (= Previous sum + current value)
	add r3, r3, #4   	@ Update Source Address (4 bytes)
	sub r1, r1, #1    	@ Update Loop counter (sub 1)
	cmp r1, #0        	@ Compare Loop counter with 0
	bne LoadAddIntegers	@ If Loop counter != 0, keep looping

	swi SWI_Exit		@ Stops the program, latency = 100, energy = 10000

	.data				@ Do a memory allocation for 4000 words (each 4 bytes)
AA:	.space 4000
	.end
