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
	bcs InFileError			; branch if carry is set (i.e. error)
	ldr r1,=InFileHandle	; load input file handle
	str r0, [r1]					; save the input file handle

readFromFile:
	swi SWI_RdInt		; read int from file, r0 already contains file handle
	bcs ReadError		@ the integer is now stored in r0
	mov r1, r0			; move the value read to r1
	mov r0, #1			; to print the int to stdout
	swi SWI_PrInt		; prints value in r1 to stdout

closeInputFile:
	ldr r0,=InFileHandle
	ldr r0,[r0]
	swi SWI_Close

@ Stops the program
swi SWI_Exit

InFileError:
	ldr r0,=InFileErrorMsg
	swi SWI_DpStr

ReadError:
	ldr r0,=ReadErrorMsg
	swi SWI_DpStr

.data	/* data used by the program including read only variables/constants */
InFileName: .asciz "in.txt"	@used to initialize string
InFileHandle: .word 0	@used to initialize integer variables
OutFileName: .asciz "out.txt"
OutFileHandle: .word 0
FileTest:	.asciz "The file was properly accessed.\n"
InFileErrorMsg: .asciz "Unable to open input file\n"
OutFileErrorMsg: .asciz "Unable to open output file\n"
ReadErrorMsg: .asciz "Unable to read from the input file\n"
