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

; readFromFile:
; 	ldr r0,=InFileHandle 	; load the input file handle address
; 	ldr r0,[r0]						; load the value of input file handle
; 	mov r1,r3							;	destination address in r1
; 	; add r3,r3,#1 					; increment address by 1
; 	mov r2,#4						; maximum no of bytes to store into memory
; 	swi SWI_RdStr					; read string from file, r0 already contains file handle
; 	bcs closeInputFile		@ the string is now stored at start_state, r0 contains the bytes stored
; 	ldr r0,=start_state 	; r0 contains the start_state's address
; 	swi SWI_DpStr					; display start_state's value on stdout
; 	; mov r0,#1
; 	; mov r1,
; 	; swi SWI_PrChr
; 	bal readFromFile

readIntFromFile:
	ldr r0,=InFileHandle
	ldr r0,[r0]
	swi SWI_RdInt
	bcs closeInputFile
	ldr r1,=start_state
	str r0,[r1]

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

; printToFile:
; 	ldr r1,=start_state	;
; 	mov r2,#1
; 	swi SWI_PrStr ; print string to the file
; 	bcs closeOutputFile

; printIntToFile:
; 	ldr r1,=start_state ;
; 	ldr r1,[r1]
; 	swi SWI_PrInt
; 	@ print \n
; 	ldr r1,=end_of_line
; 	swi SWI_PrStr

ldr r1,=start_state ;
ldr r1,[r1]

mov r6,r1			; r6 now contains the value of start_state, remains unchanged
mov r5,#0			; r5 contains the current period value
mov r3,r6			; r3 is used to calculate bit value, initialization
mov r4,r6 		; r4 is used to generate next value of lfsr
mov r7,#0
lfsr:
	EOR r3,r3,r4,LSR #2
	EOR r3,r3,r4,LSR #3
	EOR r3,r3,r4,LSR #5
	@ now r3 contains the value of lfsr ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)
	AND r3,r3,#1 ; now r3 contains the value of bit as in c program
	ORR r4,r7,r4,LSR #1 ; r4 contains lfsr >> 1
	ORR r4,r4,r3,LSL #15; r4 contains r4 | (bit << 15)
	@ now r4 contains the next pseudo random no
	mov r3,r4			; r3 is used to calculate bit value
	add r5,r5,#1 ; increment the period
	cmp r5,#5	; compare period with 5
	bgt NotPrintLfsr ; if less or equal, need to print the lfsr value
	printLFSR:
		@ print lfsr =
		ldr r1,=lfsr_str
		swi SWI_PrStr
		@ print the value of lfsr
		mov r1, r4
		swi SWI_PrInt
		@ print \n
		ldr r1,=end_of_line
		swi SWI_PrStr

	NotPrintLfsr:
	cmp r4,r6 ; compare with the start_state
	bne lfsr ; if not equal continue in the loop

printPostTermination:
	@ print lfsr =
	ldr r1,=lfsr_end
	swi SWI_PrStr
	@ print the value of lfsr
	mov r1, r4
	swi SWI_PrInt

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
InFileName: .asciz "in.txt"	@used to initialize string
InFileHandle: .word 0	@used to initialize integer variables
OutFileName: .asciz "out.txt"
OutFileHandle: .word 0
InFileErrorMsg: .asciz "Unable to open input file\n"
OutFileErrorMsg: .asciz "Unable to open output file\n"
ReadErrorMsg: .asciz "Unable to read from the input file\n"
WriteErrorMsg: .asciz "Unable to write to the output file\n"
end_of_line: .asciz "\n"
lfsr_str: .asciz "lsfr="
lfsr_end: .asciz "Post termination lfsr="
