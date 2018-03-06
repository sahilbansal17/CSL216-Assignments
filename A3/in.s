mov r0,#1004
mov r1,#786
str r1,[r0,#4]
ldr sp,[r0,#4]
mov r1, pc   
STR sp, [SP, #4]
LDR r0, [SP, #4]