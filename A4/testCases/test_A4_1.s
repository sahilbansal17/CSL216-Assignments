mov r2, #1

ldr_pseudo:
ldr r3, =AA

str r2, [r3]
add r3, r3, #4
mov r2, #2
str r2, [r3]

ldr_ldrImm:
ldr r1, [r3, #-4]

preIndexed_Load_ldrPre:
ldr r1, [r3, #-4]!

postIndexedLoad_ldrPost:
ldr r2, [r3], #4

b exit

AA:	.space 400
exit:
