#include "statistics.h"
using namespace std;

class ARM{
private:
	int r[16]; // for storing the registers
	int memory[100]; // for memory, assumed to be 100 words (100 * 32 bit)
	int startAddress; // start address of the memory
	int N; // negative Flag
	int Z; // zero Flag
	int C; // carry Flag
	vector <int> dlAddress; // to store data label addresses while allocating memory
	statistics st;
public:
	ARM(){
		for(int i = 0 ; i < 13 ; i ++){
			r[i] = 0;
		}
		r[13] = 1400; // stack pointer pointing at the end of memory location
		r[14] = 0;
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
	// function to perform ldr instruction
	void ldr(int rd, int rn, int offset){
		r[rd] = memory[(r[rn] + offset-1000)/4] ;
	}
	// function to read memory at a given addr
	void ldrPseudo(int rd, int dlIndex){
		r[rd] = dlAddress[dlIndex] ;
	}

	// function to perform str instruction
	void str(int rd, int rn, int offset){
		memory[(r[rn] + offset-1000)/4] = r[rd];
	}

	// function to get register value of a particular reg
	int getR_atIndex(int i){
		return r[i];
	}

	// function to get N flag
	int getN(){
		return N;
	}

	// function to get Z flag
	int getZ(){
		return Z;
	}

	// function to get C flag
	int getC(){
		return C;
	}

	// function to perform cmp instruction
	void cmp(int r1, int r2, bool i){
		int val;
		if(i == false){
			val = r[r1] - r[r2];
			if(r[r1] >= 0 && r[r2] <= 0){
				// no borrow is taken in this case
				C = 0;
			}
			else if(r[r1] <= 0 && r[r2] >= 0){
				// borrow is taken
				C = 1;
			}
			else if(r[r1] < r[r2]){
				// if r[r2] is greater than r[r1] then borrow is taken
				C = 0;
			}
			else{
				// if r[r2] is less than or equal to r[r1] then no borrow is taken
				C = 1;
			}
		}
		else{
			val = r[r1] - r2;
			if(r[r1] >= 0 && r2 <= 0){
				C = 0;
			}
			else if(r[r1] <= 0 && r2 >= 0){
				C = 1;
			}
			else if(r[r1] < r2){
				C = 0;
			}
			else{
				C = 1;
			}
		}
		if(val < 0){
			// if difference is less than 0 then N = 1
			N = 1;
			Z = 0;
		}
		else if(val == 0){
			// if difference is zero then Z = 0
			Z = 1;
			N = 0;
		}
	}

	// function to perform cmn instruction
	void cmn(int r1, int r2, bool i){
		int val;
		if(i == false){
			val = r[r1] + r[r2];
			// if one of them is negative and other is 0, then no carry
			if(r[r1] == 0 || r[r2] == 0){
				C = 0;
			}
			// else if either of them is negative and other is positive, or both negative then C = 1
			else if(r[r1] < 0 || r[r2] < 0){
				C = 1;
			}
			// else if both +ve then no carry
			else{
				C = 0;
			}
		}
		else{
			val = r[r1] + r2;
			if(r[r1] == 0 || r2 == 0){
				C = 0;
			}
			else if(r[r1] < 0 || r2 < 0){
				C = 1;
			}
			else{
				C = 0;
			}
		}
		if(val < 0){
			N = 1;
			Z = 0;
		}
		else if(val == 0){
			Z = 1;
			N = 0;
		}
	}

	// function to perform bge instruction
	void bge(int label){
		if(N == 0){
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
		r[14] = r[15] + 4;	// updating LR to next instruction address
		r[15] = label;		// updating pc to label address
	}

	// function to perform blt instruction
	void blt(int label){
		if(N == 1){
			r[15] = label;
		}
	}

	// function to perform bne instruction
	void bne(int label){
		if(Z == 0){
			r[15] = label;
		}
	}

	// function to perform offset
	void offset(int r1, int offset){
		r[r1] = r[r1] + offset;
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
			add(rd, rn, operand2, imm); // call to add function
			st.counter(op);
		}
		else if(op == "sub"){
			sub(rd, rn, operand2, imm); // call to sub function
			st.counter(op);
		}
		else if(op == "mul"){
			mul(rd, rn, operand2); // call to mul function
			st.counter(op);
		}
		else if(op == "mov"){
			mov(rn, operand2, imm); // call to mov function
			st.counter(op);
		}
		else if(op == "ldr"){
			ldr(rd, rn, 0); // normal ldr with no offset
			st.counter(op);
		}
		else if(op == "ldrImm"){
			ldr(rd, rn, operand2); // ldr with immediate offset
			st.counter("ldr");
		}
		else if(op == "ldrPre"){
			offset(rn, operand2); // first update the load address
			ldr(rd, rn, 0); // pre-indexed ldr
			st.counter("ldr");
		}
		else if(op == "ldrPost"){
			ldr(rd, rn, 0); // post-indexed ldr
			offset(rn, operand2); // update the load address later
			st.counter("ldr");
		}
		else if(op == "ldrPseudo"){
			// cout << "Called";
			ldrPseudo(rd, operand2); // Pseudo ldr
			st.counter("ldr");
		}
		else if(op == "str"){
			str(rd, rn, 0); // normal str with no offset
			st.counter("str");
		}
		else if(op == "strImm"){
			str(rd, rn, operand2); // ldr with immediate offset
			st.counter("str");
		}
		else if(op == "strPre"){
			offset(rn, operand2); // first update the load address
			str(rd, rn, 0); // pre-indexed ldr
			st.counter("str");
		}
		else if(op == "strPost"){
			str(rd, rn, 0); // post-indexed ldr
			offset(rn, operand2); // update the load address later
			st.counter("str");
		}
		else if(op == "b"){
			b(operand2);
			st.counter(op);
		}
		else if(op == "bl"){
			bl(operand2);
			st.counter(op);
		}
		else if(op == "bge"){
			bge(operand2);
			st.counter(op);
		}
		else if(op == "bne"){
			bne(operand2);
			st.counter(op);
		}
		else if(op == "beq"){
			beq(operand2);
			st.counter(op);
		}
		else if(op == "blt"){
			blt(operand2);
			st.counter(op);
		}
		else if(op == "cmp"){
			cmp(rn, operand2, imm);
			st.counter(op);
		}
		else if(op == "cmn"){
			cmn(rn, operand2, imm);
			st.counter(op);
		}
	}

	// display the contents of the register file and NZCV flags, also the memory
	void display(instructions i, int count){
		cout << count << ". " << i.getOp() << "\n";
		cout << ".Registers -";
		for(int j = 0; j < 16; j++){
			printf("|r%d=%2d",j,r[j]);
		}
		cout << "|\n";
		cout << ".Flags -";
		cout << " N :" << getN() << "| " << "Z :" << getZ() << "| " << "C :" << getC() << "|\n";
		cout << ".Memory Filled -";
		for(int j = 0; j < 100 ; j++){
			if(memory[j] != 0){
				printf("|(%d)=%2d",j*4+1000,memory[j]);
			}
		}
		cout << "|\n\n";
	}

	// to allocate memory to the data lables and push the address to dlAddress
	void allocate(vector<data_Label> data_labels){
		for(int i = 0; i < data_labels.size(); i++){
			dlAddress.push_back(startAddress);
			if(data_labels[i].size % 4 == 0){
				startAddress += data_labels[i].size;
			}
			else{
				startAddress += ((data_labels[i].size)/4)*4 + 4; // ceil value
			}
		}
	}

	// to run the single cycle version of the ARM simulator
	void runSingleCycle(vector <instructions> inst_vec){
		int count = 1;
		int pointer = 0;
		while(pointer != inst_vec.size()){
			int old, newPC;
			old = r[15];
			newPC = r[15];
			execute(inst_vec[pointer]);   // execute the ith instruction
			if(r[15] != old){
				newPC = r[15]; // if PC has changed then store new value and
				r[15] = old; // old value back to PC for display
			}
			// display(inst_vec[pointer], count ++); // display the contents of register

			r[15] = newPC;
			if(r[15] != old){
				// check whether pc is changed after the execution of instruction
				pointer = (r[15] - 1000)/4; // update the pointer
			}
			else{
				pointer ++; // if pointer = pc then increment pointer
				r[15] += 4; // update PC
			}
			if(Debug == 1){
				char c;
				scanf("%c",&c);
			}
		}
		cout<<"\n";
		st.display();
	}

	// to run the Multi Cycle version of the ARM Simulator
	void runMultiCycle(vector <instructions> inst_vec){

	}

	// to show the latency associated with each instruction
	void showCycleData(){
		for(int i = 0; i < latency_obj.size(); i++){
        	cout << latency_obj[i].command << "--> " << latency_obj[i].clock_cycle << endl;
    	}
	}
};
