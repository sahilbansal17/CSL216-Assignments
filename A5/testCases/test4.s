mov r1, #10
mov r0, #0
loop:
add r0, r0, #1
cmp r0, r1
bne loop
add r3, r0, r1
add r4, r3, r0