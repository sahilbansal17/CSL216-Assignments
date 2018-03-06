main:
mov r0, #4
bl fact
b exit  
fact:
SUB SP, SP, #8
STR LR, [SP, #4]
STR r0, [SP, #0]
CMP r0, #1
BGE L1
MOV r0, #1
ADD SP, SP, #8 
MOV pc, lr
L1 : SUB r0, r0, #1
BL fact
MOV r12, r0 
LDR r0, [SP, #0]
LDR LR, [SP, #4]
ADD SP, SP, #8
mov r1, r0
MUL r0, r1, r12
MOV pc, lr 
exit:
