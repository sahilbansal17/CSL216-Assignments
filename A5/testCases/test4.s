mov r2, #1
mov r1, #10
mov r0, #0
loop:
add r0 ,r0 ,#1
cmp r2, #0
bne notzero
add r2, r2, #2
notzero:
sub r2, r2, #1
cmp r0, r1
bne loop