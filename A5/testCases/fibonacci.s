mov r0, #0
mov r1, #1
add r2, r0, r1

mov r5, #30

calc_r5_th_FibNumber_in_r2:
    mov r0, r1
    mov r1, r2
    add r2, r0, r1
    sub r5, r5, #1
    cmp r5, #3
    bne calc_r5_th_FibNumber_in_r2
    
mov r0, #0
mov r1, #0
mov r5, #0