#include "arm.h"
#include <stdio.h>
using namespace std;

ARM myArm ; 
// executing the instructions 
void execute(instructions i){

    // get the instruction parameters 
    string op = i.getOp();
    int rd = i.getRd();
    int rn = i.getRn();
    int operand2 = i.getOp2();
    bool imm = i.getImm();

    if(op == "add"){
        myArm.add(rd, rn, operand2, imm); //call to add function
    }
    else if(op == "sub"){
        myArm.sub(rd, rn, operand2, imm); //call to sub function
    }
    else if(op == "mul"){
        myArm.mul(rd, rn, operand2); //call to mul function
    }
    else if(op == "mov"){
        myArm.mov(rd, operand2, imm); //call to mov function
    }
    else if(op == "ldr"){
        myArm.ldr(rd, operand2);
    }
    else if(op == "str"){
        myArm.str(rd, operand2);
    }
}

// display the contents of the register file
void display(){
    for(int j = 0; j < 10; j++){
        printf("|r%d=%5d",j,myArm.getR_atIndex(j)) ;
    }
    cout << "|\n";
}