@ give symbolic names to numeric constants
.equ SWI_PrChr, 0x00
.equ SWI_DpStr, 0x02
.equ SWI_Exit, 0x11
.equ SWI_MeAlloc, 0x12
.equ SWI_DAlloc, 0x13
.equ SWI_Open, 0x66
.equ SWI_Close, 0x68
.equ SWI_PrStr, 0x69
.equ SWI_RdStr, 0x6a
.equ SWI_PrInt, 0x6b
.equ SWI_RdInt, 0x6c
.equ SWI_Timer, 0x6d

.text /* includes all the instructions */
openInputFile:
	ldr r0,=InFileName	; set name for the input file
	mov r1,#0 					; mode of opening the file is input mode
	swi SWI_Open				; open the file and return the handle to r0 if successful or set the carry bit if error occured
	bcs InFileError				; branch if carry is set (i.e. error)
	ldr r1,=InFileHandle	; load input file handle's address
	str r0, [r1]					; save the input file handle, r0 => InFileHandle

ldr r3,=start_state  ; load the address of start_state in r3

readFromFile:
	ldr r0,=InFileHandle 	; load the input file handle address
	ldr r0,[r0]						; load the value of input file handle
	mov r1,r3							;	destination address in r1
	mov r2,#4							; no of bytes to read
	swi SWI_RdStr					; read string from file, r0 already contains file handle
	bcs closeInputFile		@ the string is now stored at start_state, r0 now contains the bytes stored
	; ldr r0,=start_state 	; r0 contains the start_state's address
	; swi SWI_DpStr					; display start_state's value on stdout
	; mov r0,#1
	; mov r1,
	; swi SWI_PrChr

; readIntFromFile:
; 	ldr r0,=InFileHandle
; 	ldr r0,[r0]
; 	swi SWI_RdInt
; 	bcs closeInputFile
; 	ldr r1,=start_state
; 	str r0,[r1]

closeInputFile:
	ldr r0,=InFileHandle
	ldr r0,[r0]
	swi SWI_Close

openOutputFile:
	ldr r0,=OutFileName
	mov r1,#1						; mode of opening the file is output mode
	swi SWI_Open
	bcs OutFileError
	ldr r1,=OutFileHandle
	str r0,[r1]

@ function to convert from hexadecimal input to integer
mov r5,#0	; bit number, goes from 0 to 3 (16 bit binary number so 4 bits in hexa)
mov r6,#0	; final result will be stored in r6
mov r7,#1	; to be multipied with hexa value to convert to power of 16
mov r9,#0 ; temp, for doing left shift using ORR
ORR r7,r9,r7,LSL #12; store 2^12 in r7, i.e. 16^4
convertHexaToInteger:
	ldrb r1,[r3]	; read one byte from address at r3 to r1
	add r3,r3,#1 	; increment the address at r3
	; swi SWI_PrInt ; print string to the file
	; @ print \n
	; ldr r1,=end_of_line
	; swi SWI_PrStr
	@ r1 contains the ascii value, convert it to 0-15
	cmp r1, #97
	bge alphabet
	number:
	@ if a number, subtract 48 from the ascii value in r1
	sub r1,r1,#48
	b next
	alphabet:
	@ if an alphabet A-F , subtract 87 from the ascii value in r1
	sub r1,r1,#87
	next:
		mul r8,r1,r7 ; multiply with appropriate power of 16
		ORR r7,r9,r7,LSR #4  ; right shift by 4 to get previous power of 16
		add r6,r6,r8 ; final integer value
		add r5,r5,#1
		cmp r5,#4	; need to take 4 characters (16 bit hexadecimal number)
		bge lfsr_optimal
		b convertHexaToInteger
; r2,r7,r8,r9 are unused
lfsr_optimal:
; r6 now contains the value of start_state, remains unchanged
mov r5,#0			; r5 contains the current period value
mov r3,r6			; r3 is used to calculate bit value, initialization
mov r4,r6 		; r4 is used to generate next value of lfsr
mov r7,#0
getCounts:
	mov r1,#1 ; mask
	mov r8,#0 ; to store count of 1
	AND r2,r3,r1 ; r2 = r3 & 1 , get bit 0
	cmp r2,#0
	ADDNE r8,r8,#1 ; if r3 not equal to 1 then add 1 to r8
	mov r1,r1,LSL #2 ; r1 = 2^2 = 4
	AND r2,r3,r1 ; r2 = r3 & 2^2 , get bit 2
	cmp r2,#0
	ADDNE r8,r8,#1
	mov r1,r1,LSL #1 ; r1 = 2^3 = 8
	AND r2,r3,r1 ; r2 = r3 & 2^3 , get bit 2
	cmp r2,#0
	ADDNE r8,r8,#1
	mov r1,r1,LSL #2 ; r1 = 2^5 = 32
	AND r2,r3,r1 ; r2 = r3 & 2^5 , get bit 2
	cmp r2,#0
	ADDNE r8,r8,#1
lfsr:
	ORR r4,r7,r4,LSR #1 ; r4 contains lfsr >> 1
	; get the correct value of bit in r3
	AND r8,r8,#1 ; r8 = r8 & 1, get last bit, check whether odd or even
	mov r3,#0 ; if even, i.e. r8 contains 0
	cmp r8,#0
	MOVNE r3,#1 ; if odd, i.e. r8 contains 1
	ORR r4,r4,r3,LSL #15; r4 contains r4 | (bit << 15)
	@ now r4 contains the next pseudo random no
	mov r3,r4
	add r5,r5,#1 ; increment the period
	cmp r5,#5	; compare period with 5
	bgt NotPrintLfsr ; if less or equal, need to print the lfsr value
	printLFSR:
		@ print lfsr =
		ldr r1,=lfsr_str
		swi SWI_PrStr
		@ convert the value of lfsr to hexa
		; mov r1,#0 ; this will contain the result
		ldr r1,=result
		add r1,r1,#3 ; since we will store the result from the last byte
		mov r9,r4 ; used to get 4 bits
		; we will right shift r9 by 4 and then use it to mask with 15 to get 4 bits
		mov r8,#0 ; loop counter
		convertIntegerToHexa:
			AND r2,r9,#15 ; r2 = r4 & r9
			cmp r2,#10
			blt number1
			alphabet1:
				@if alphabet, add 87 to it
				add r2,r2,#87
				b proceed
			number1:
				@ if number add 48 to it
				add r2,r2,#48
			proceed:
			@ need to store this in 1 byte at 'result'
			strb r2,[r1],#-1 ; post-indexed storing in memory
			MOV r9,r9,LSR #4 ; right shift
			add r8,r8,#1 ; increment the loop counter
			cmp r8, #4
			blt convertIntegerToHexa
		@ print the value of lfsr
		ldr r1,=result ; result address in memory stores the hexa form
		swi SWI_PrStr
		@ print \n
		ldr r1,=end_of_line
		swi SWI_PrStr
		NotPrintLfsr:
		cmp r4,r6 ; compare with the start_state
		bne getCounts ; if not equal continue in the loop

printPostTermination:
	@ print lfsr =
	ldr r1,=lfsr_end
	swi SWI_PrStr
	@ print the value of lfsr
	ldr r1,=start_state ; from verification, it comes out to be the same
	swi SWI_PrStr

closeOutputFile:
	ldr r0,=OutFileHandle
	ldr r0,[r0]
	swi SWI_Close

@ Stops the program
Exit:
swi SWI_Exit

@Error handling
InFileError:
	ldr r0,=InFileErrorMsg
	swi SWI_DpStr
	b Exit

OutFileError:
	ldr r0,=OutFileErrorMsg
	swi SWI_DpStr
	b Exit

ReadError:
	ldr r0,=ReadErrorMsg
	swi SWI_DpStr
	b Exit

WriteError:
	ldr r0,=WriteErrorMsg
	swi SWI_DpStr
	b Exit

.data	/* data used by the program including read only variables/constants */
start_state:	.skip 5	@5 bytes allocated since input would be atmost 4 char long
result:	.skip 5
InFileName: .asciz "in.txt"	@used to initialize string
InFileHandle: .word 0	@used to initialize integer variables
OutFileName: .asciz "out.txt"
OutFileHandle: .word 0
InFileErrorMsg: .asciz "Unable to open input file\n"
OutFileErrorMsg: .asciz "Unable to open output file\n"
ReadErrorMsg: .asciz "Unable to read from the input file\n"
WriteErrorMsg: .asciz "Unable to write to the output file\n"
end_of_line: .asciz "\n"
lfsr_str: .asciz "lfsr="
lfsr_end: .asciz "Post termination lfsr="
