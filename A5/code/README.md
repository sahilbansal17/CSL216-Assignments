# Assignment 4 - ARM Simulator with Multicycle Instructions

It enhances the instruction set simulator developed in Assignment 3, to also incorporate delays.

## Running the ARM Simulator :

#### Normal execution mode (all instructions at once)
`make execute input="input_file_name.extension"`

eg. : `make execute input=testCases/test_A4_1.s`


#### Debug mode (run instructions cycle by cycle, hit Enter after each cycle)
`make debug input="input_file_name.extension"`

eg. : `make debug input=testCases/test_A4_1.s`

#### Clean (removes executable)
`make clean`

## Assumptions and About the Simulator

Assuming a file **'latency.txt'** will be given that indicates the information of how many clock cycles each instruction requires.

The latency of all the supported instructions must be provided in it otherwise an error will be reported.

The program supports the following instructions:
- LDR/STR
  - *op{type}{cond} Rt, [Rn {, #offset}]* ; immediate offset
  - *op{type}{cond} Rt, [Rn, #offset]!* ; pre-indexed
  - *op{type}{cond} Rt, [Rn], #offset* ; post-indexed
  - *LDR{condition} register,=[expression | label-expression]*
- ADD/SUB
  - *op {Rd}, Rn, Operand2*
  - *op {Rd}, Rn, #imm12* ; 32-bit Thumb ADD and SUB only
- MUL
  - MUL {Rd}, Rn, Rm
- CMP
  - CMP{cond} Rn, Operand2
- CMN
- For Branches: *op1{cond} label*
  - BNE
  - BGE
  - B
  - BL
  - BEQ
  - BLT
- MOV
  - *MOV{S}{cond} Rd, Operand2*
  - *MOV{cond} Rd, #imm16*

The only assemble directive supported is **.space**. To use it, it must follow the label in the same line.

For labels, the instruction must start from the next line.

It is assumed that the size of data memory is 100 words (400 bytes) and start address is 1000.
Memory can currently be only allocated at multiples of 4 (since an integer array is used to simulate the memory).

Currently, comments are not supported in the input file.

### Test Cases
Some test cases are provided in the **testCases** folder which have been extensively tested during the making of the simulator at each step.

1. `testCases/test_A4_1.s` is for verifying that all the kinds of load instructions are working properly.

2. `testCases/fact.s` is for verifying that function calls `bl` work properly.

3. `testCases/sum_1_to_n.s` is the code given in **Assignment 1** without comments and modified to fit within memory limits.

4. Other test cases are for verifying all other basic instructions.
