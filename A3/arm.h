#include <stdio.h>
using namespace std;

class ARM{
private:
	int r[16]; // for storing the registers
	int memory[100]; // for memory, assumed to be 100 words (100 * 32 bit)
	int startAddress; // start address of the memory
	int N; // negative Flag
	int Z; // zero Flag
	int C; // carry Flag
public:
	ARM(){
		for(int i = 0 ; i < 15 ; i ++){
			r[i] = 0;
		}
		r[15] = 1000; // instruction addr starts with 1000	
		for(int i = 0 ; i < 100 ; i ++){
			memory[i] = 0;
		}
		startAddress = 1000;
		N = 0; Z = 0; C = 0;
	}
	// function to perform add instruction
	void add(int rd, int rn, int operand2, bool i){
		if(i == true){
			r[rd] = r[rn] + operand2;
		}
		else{
			r[rd] = r[rn] + r[operand2];
		}
	}
	// function to perform sub instruction
	void sub(int rd, int rn, int operand2, bool i){
		if(i == true){
			r[rd] = r[rn] - operand2;
		}
		else{
			r[rd] = r[rn] - r[operand2];
		}
	}
	// function to perform mul instruction
	void mul(int rd, int rn, int operand2){
		r[rd] = r[rn] * r[operand2];
	}
	// function to perform mov instruction
	void mov(int rd, int operand2, bool i){
		if(i == true){
			r[rd] = operand2;
		}
		else{
			r[rd] = r[operand2];
		}
	}
	// function to perform ldr instruction  ( in progress )
	void ldr(int rd, int operand2){
		r[rd] = memory[operand2];
	}

	// function to perform ldr offset instruction  ( in progress )
	void ldr1(int rd, int rn, int offset){
		r[rd] = memory[rn]+offset;
	}

	// function to perform str instruction ( in progress )
	void str(int rd, int operand2){
		memory[operand2] = r[rd];
	}

	// function to get register value of a particular reg 
	int getR_atIndex(int i){
		return r[i];
	}

	// function to perform cmp instruction
	void cmp(int r1, int r2){
		if((r[r1] > 0 && r[r2] > 0)||(r[r1] < 0 && r[r2] < 0)){
			if(r[r1] - r[r2]<0){
				N = 1;
			}
			else if(r[r1] - r[r2] == 0){
				Z = 1;
				C = 1;
			}
		}
		else cmn(r1, r2);
	}

	// function to perform cmn instruction
	void cmn(int r1, int r2){
		if((r[r1] > 0 && r[r2] > 0)||(r[r1] < 0 && r[r2] < 0)){
			cmp(r1, r2);
		}
		else if(r[r1] > 0 && r[r2] < 0){
			if(r[r1] + r[r2] < 0){
				N = 1;
			}
			else if(r[r1] + r[r2] == 0){
				Z = 1;
			}
		}
		else{
			if(r[r1] + r[r2] > 0){
				N = 1;
			}
			else if(r[r1] + r[r2] == 0){
				Z = 1;
			}        
		}
	}

	// function to perform bge instruction
	void bge(int label){
		if(N == 0 && Z == 0){
			r[15] = label;
		}
	}

	// function to perform b instruction
	void b(int label){
		r[15] = label;
	}

	// function to perform beq instruction
	void beq(int label){
		if(Z == 1){
			r[15] = label;
		}
	}

	// function to perform bl instruction
	void bl(int label){
		r[14] = r[15] + 4;	// pointing LR to net instruction of pc
		r[15] = label;		// updating pc to label
	}

	// function to perform blt instruction
	void blt(int label){
		if(N == 1){
			r[15] = label;
		}
	}

	// function to perform bne instruction
	void bne(int label){
		if(Z != 1){
			r[15] = label;
		}
	}

	// function to perform offset
	void offset(int r1, int offset){
		r[r1]=r[r1]+offset;
	}

	// executing the instructions 
	void execute(instructions i){
		// get the instruction parameters 
		string op = i.getOp();
		int rd = i.getRd();
		int rn = i.getRn();
		int operand2 = i.getOp2();
		bool imm = i.getImm();

		if(op == "add"){
			add(rd, rn, operand2, imm); //call to add function
		}
		else if(op == "sub"){
			sub(rd, rn, operand2, imm); //call to sub function
		}
		else if(op == "mul"){
			mul(rd, rn, operand2); //call to mul function
		}
		else if(op == "mov"){
			mov(rd, operand2, imm); //call to mov function
		}
		else if(op == "ldr"){
			ldr(rd, rn);
		}
		else if(op == "ldr1"){
			ldr1(rd, rn, operand2);
		}
		else if(op == "ldr2"){
			offset(rn, operand2);
			ldr(rd, rn);
		}
		else if(op == "ldr3"){
			ldr(rd, rn);
			offset(rn, operand2);
		}
		else if(op == "str"){
			str(rd, operand2);
		}
		else if(op == "b"){
			b(operand2);
		}
		else if(op == "bl"){
			bl(operand2);
		}
		else if(op == "bge"){
			bge(operand2);
		}
		else if(op == "bne"){
			bne(operand2);
		}
		else if(op == "beq"){
			beq(operand2);
		}
		else if(op == "blt"){
			blt(operand2);
		}
		else if(op == "cmp"){
			cmp(rn, operand2);
		}
		else if(op == "cmn"){
			cmn(rn, operand2);
		}
	}

	// display the contents of the register file
	void display(){
		for(int j = 0; j < 16; j++){
			printf("|r%d=%5d",j,r[j]) ;
		}
		cout << "|\n";
	}

	void run(vector <instructions> inst_vec){
		int pointer=0;
		while(pointer != inst_vec.size()){
			int old, newPC;
			old = r[15];
			newPC = r[15];
			execute(inst_vec[pointer]);   // execute the ith instruction
			if(r[15] != old){
				newPC = r[15]; // if PC has changed then store new value and 
				r[15] = old; // old value back to PC for display
			}
			display(); // display the contents of register
			
			r[15] = newPC;
			if(r[15] != old){
				// check whether pc is changed after the execution of instruction
				pointer = (r[15] - 1000)/4; // update the pointer 
			}
			else{
				pointer ++; // if pointer = pc then increment pointer 
				r[15] += 4; // update PC
			}
			// if(Debug == 1){
			// 	char c;
			// 	scanf("%c",&c);
			// }
		}
	}
};