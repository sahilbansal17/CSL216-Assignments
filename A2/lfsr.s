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
	ldr r1,=start_state	;	destination address in r1
	mov r2,#4				; maximum no of bytes to store into memory
	swi SWI_RdStr		; read string from file, r0 already contains file handle
	bcs ReadError		@ the string is now stored at start_state, r0 contains the bytes stored
	ldr r0,=start_state ; r0 contains the start_state's address
	swi SWI_DpStr		; display start_state's value on stdout

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

printToFile:
	ldr r1,=start_state	;
	swi SWI_PrStr ; print string to the file
	bcs WriteError

closeOutputFile:
	ldr r0,=OutFileHandle
	ldr r0,[r0]
	swi SWI_Close

@ Stops the program
Exit:
swi SWI_Exit

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
