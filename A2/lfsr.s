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

@ display character A on stdout
displayA:
  mov r0,#'A      @ moves character A to register r0
  swi SWI_PrChr   @ print the char on stdout
  mov r0,#'\n     @ moves null character to register r0
  swi SWI_PrChr   @ prints the null char on stdout

@ display string on stdout
displayMyString:
  ldr r0,=MyString  @ load the address of label MyString to register r0
  swi SWI_DpStr     @ display the string on stdout

@ allocate block of memory on heap and write some value on it
allocateMem:
  mov r0, #28         @ moves constant 28 to register r0
  swi SWI_MeAlloc     @ allocate memory of 28 bytes and store its address in register r0
  ldr r1,=MyAddress   @ load the address of label MyAddress to register r1
  str r0,[r1]         @ store the values of register r0 (address of 28 byte memory) to address stored in register r1
  mov r2, #8
  str r2,[r0]

@ deallocate memory
deallocateMem:
  swi SWI_DAlloc

@ labels, symbol table
MyString: .asciz "Hello There\n"
MyAddress: .word 34

@ Stops the program
swi SWI_Exit
