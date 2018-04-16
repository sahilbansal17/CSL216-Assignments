	mov r1, #100
	mov r2, #1
	ldr r3, =AA

StoreIntegers:
	str r2, [r3]
	add r3, r3, #4
	add r2, r2, #1
	sub r1, r1, #1
	cmp r1, #0
	bne StoreIntegers

	mov r1, #100
	mov r4, #0
	ldr r3, =AA

LoadAddIntegers:
	ldr r2, [r3]
	add r4, r4, r2
	add r3, r3, #4
	sub r1, r1, #1
	cmp r1, #0
	bne LoadAddIntegers

	b exit

AA:	.space 400
exit:
