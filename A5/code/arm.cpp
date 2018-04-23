#include "arm.h"
using namespace std;

ARM :: ARM(){
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

int ARM :: alu(string inst, int a, int b){
	if(inst == "add"){
		return a + b;
	}
	else if(inst == "sub"){
		return a - b;
	}
	else if(inst == "mul"){
		return a * b;
	}
	else{
		return 0;
	}
}

int ARM :: reg_file(int i){
	return r[i];
}

int ARM :: getLatency(string s){
		// return 1; // for testing purpose
		if(s != "ldr_pseudo" && s[0] == 'l' && s[1] == 'd' && s[2] == 'r'){
			// except ldr_pseudo, all kinds of ldr will will stored as "ldr" in latency_obj
			s = "ldr";
		}
		else if(s[0] == 's' && s[1] == 't' && s[2] == 'r'){
			// all kinds of str will be stored as "str" in latency_obj
			s = "str";
		}
		for(int i = 0 ; i < latency_obj.size(); i ++){
			string latency_inst(latency_obj[i].command);
			if(s == latency_inst){
				return latency_obj[i].clock_cycle;
			}
		}
		cout << "Error: No latency defined for instruction: " << s << "\n";
		return -1;
	}


// to allocate memory to the data lables and push the address to dlAddress
void ARM :: allocate(vector<data_Label> data_labels){
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

// to show the latency associated with each instruction
void ARM :: showLatencyData(){
	cout << "\n\nInstructions with their latencies: \n";
	for(int i = 0; i < latency_obj.size(); i++){
		cout << latency_obj[i].command << " --> " << latency_obj[i].clock_cycle << endl;
	}
	cout << "\n\n";
}

// function to get N flag
int ARM :: getN(){
	return N;
}

// function to get Z flag
int ARM :: getZ(){
	return Z;
}

// function to get C flag
int ARM :: getC(){
	return C;
}

// display the contents of the register file and NZCV flags, also the memory
void ARM :: display(instructions i, int count){
	cout << "Cycle No.: " << count << " => " << i.getFullInst() << "\n";
	cout << ".Registers -";
	for(int j = 0; j < 16; j++){
		printf("|r%d=%2d",j,r[j]);
	}
	cout << "|\n";
	cout << ".Flags -";
	cout << " N :" << getN() << "| " << "Z :" << getZ() << "| " << "C :" << getC() << "|\n";
	cout << ".Memory Filled - ";
	int memoryNull = 1;
	for(int j = 0; j < 100 ; j++){
		if(memory[j] != 0){
			memoryNull = 0;
			printf("|(%d)=%2d",j*4+1000,memory[j]);
		}
	}
	if(!memoryNull) cout << "|\n\n";
	else cout << "NIL \n\n";
}

void ARM :: IF(){
	if(IF_ID.pc == ID_EX.pc){
		IF_ID.pc += 4;
		r[15] = IF_ID.pc;
	}
	else{
		IF_ID.pc = ID_EX.pc;
		r[15] = IF_ID.pc;
	}

	if(IF_ID.pc < inst_vec.size()  * 4 + 1000){
		IF_ID.i = (IF_ID.pc-1000)/4;
	}
}

void ARM :: ID(){
	// cout<<"Inside ID"<<endl;
	ID_EX.pc = IF_ID.pc;
	instructions temp = inst_vec[IF_ID.i];
	ID_EX.rn = reg_file(temp.getRn());
	ID_EX.rd = temp.getRd();
	ID_EX.imm = temp.getImm();
	ID_EX.inst = temp.getOp();
	ID_EX.operand2 = temp.getOp2();

	if(ID_EX.inst != "b" && ID_EX.inst != "bl" && ID_EX.inst != "bne" && ID_EX.inst != "beq" && ID_EX.inst != "blt" && ID_EX.inst != "bge"){
		if(!ID_EX.imm){
			ID_EX.operand2 = reg_file(ID_EX.operand2);
		}
	}

	if(ID_EX.inst == "ldr_pseudo"){
		ID_EX.operand2 = temp.getOp2();
	}
	else if(ID_EX.inst == "cmn"){
		int temp = ID_EX.rn + ID_EX.operand2;
		// if one of them is negative and other is 0, then no carry
		if(ID_EX.rn == 0 || ID_EX.operand2 == 0){
			C = 0;
		}
		// else if either of them is negative and other is positive, or both negative then C = 1
		else if(ID_EX.rn < 0 || ID_EX.operand2 < 0){
			C = 1;
		}
		// else if both +ve then no carry
		else{
			C = 0;
		}
		if(temp < 0){
			N = 1;
			Z = 0;
		}
		else if(temp == 0){
			Z = 1;
			N = 0;
		}
	}
	else if(ID_EX.inst == "cmp"){		
		int temp = ID_EX.rn - ID_EX.operand2;
		if(ID_EX.rn >= 0 && ID_EX.operand2 <= 0){
			// no borrow is taken in this case
			C = 0;
		}
		else if(ID_EX.rn <= 0 && ID_EX.operand2 >= 0){
			// borrow is taken
			C = 1;
		}
		else if(ID_EX.rn < ID_EX.operand2){
			// if r[r2] is greater than r[r1] then borrow is taken
			C = 0;
		}
		else{
			// if r[r2] is less than or equal to r[r1] then no borrow is taken
			C = 1;
		}
		if(temp < 0){
			// if difference is less than 0 then N = 1
			N = 1;
			Z = 0;
		}
		else if(temp == 0){
			// if difference is zero then Z = 0
			Z = 1;
			N = 0;
		}		
	}
	else if(ID_EX.inst == "b"){
		ID_EX.operand2 = temp.getOp2();		
		ID_EX.pc = ID_EX.operand2;
	}
	else if(ID_EX.inst == "bl"){
		ID_EX.operand2 = temp.getOp2();	
		r[14] = r[15] + 4;
		ID_EX.pc = ID_EX.operand2;
	}
	else if(ID_EX.inst == "beq"){
		ID_EX.operand2 = temp.getOp2();	
		if(Z == 1){
			ID_EX.pc = ID_EX.operand2;
		}
	}
	else if(ID_EX.inst == "bge"){
		ID_EX.operand2 = temp.getOp2();	
		if(N == 0){
			ID_EX.pc = ID_EX.operand2;
		}
	}
	else if(ID_EX.inst == "blt"){
		ID_EX.operand2 = temp.getOp2();	
		if(N == 1){
			ID_EX.pc = ID_EX.operand2;
		}
	}
	else if(ID_EX.inst == "bne"){
		cout<<"BNE called"<<endl;
		ID_EX.operand2 = temp.getOp2();	
		if(Z == 0){
			ID_EX.pc = ID_EX.operand2;
		}
	}
	// cout<<"Outside ID"<<endl;
}

void ARM :: EX(){
	EX_MEM.rd = ID_EX.rd;
	EX_MEM.rn = ID_EX.rn;
	EX_MEM.inst = ID_EX.inst;

	if(ID_EX.inst == "add" || ID_EX.inst == "sub" || ID_EX.inst=="mul"){
		EX_MEM.data = alu(ID_EX.inst, ID_EX.rn, ID_EX.operand2);		
	}
	else if(ID_EX.inst == "ldrImm" || ID_EX.inst == "ldrPre" || ID_EX.inst == "strImm" || ID_EX.inst == "strPre"){
		EX_MEM.data = alu("add", ID_EX.rn, ID_EX.operand2);
	}
	else if(ID_EX.inst == "ldr_pseudo"){
		EX_MEM.data = dlAddress[ID_EX.operand2];
	}
	else if(ID_EX.inst == "ldr" || ID_EX.inst == "str"){
		EX_MEM.data = ID_EX.rn;
	}
	else{
		if(ID_EX.imm){
			EX_MEM.data = ID_EX.operand2;			
		}
		else{
			EX_MEM.data = ID_EX.rn;			
		}
	}
}

void ARM :: MEM(){
	MEM_WB.rd = EX_MEM.rd;
	if(EX_MEM.inst == "ldr" || EX_MEM.inst == "ldrImm" || EX_MEM.inst == "ldrPre"){
		MEM_WB.data = memory[(EX_MEM.data - 1000) / 4];
		MEM_WB.reg_write = 1;
	}
	else if(EX_MEM.inst == "str" || EX_MEM.inst == "strImm" || EX_MEM.inst == "strPre" || EX_MEM.inst == "str_pseudo"){
		memory[(EX_MEM.data - 1000) / 4] = reg_file(EX_MEM.rd);
		MEM_WB.reg_write = 0;
	}
	else if(EX_MEM.inst == "add" || EX_MEM.inst == "sub" || EX_MEM.inst == "mul" || EX_MEM.inst == "mov" || EX_MEM.inst == "ldr_pseudo"){
		MEM_WB.reg_write = 1;
		MEM_WB.data = EX_MEM.data;			
	}
	else{
		MEM_WB.reg_write = 0;
		MEM_WB.data = 0;		
	}
}

void ARM :: WB(){
	if(MEM_WB.reg_write == 1){
		// cout << "Register" << MEM_WB.rd << "value" << MEM_WB.data << endl;		
		r[MEM_WB.rd] = MEM_WB.data;
		MEM_WB.reg_write = 0;
	}
}

void ARM :: run(){
	int cycle_count = 0;
	while(IF_ID.pc < inst_vec.size() * 4 + 1016){
		cycle_count++;
		display(inst_vec[IF_ID.i], cycle_count);
		WB();
		MEM();
		EX();
		ID();
		IF();
	}
}

// // to run the Multi Cycle version of the ARM Simulator
// ARM :: void runMultiCycle(vector <instructions> inst_vec){
// 	long int cycle_no = 1, inst_count = 1;
// 	int inst_cycle, old, newPC;
// 	int pointer = 0;
// 	char c; // for debug mode
// 	while(pointer != inst_vec.size()){
// 		// till all the instructions are not executed
// 		// get the no of clock cycles required to execute this instruction
// 		string current_inst = inst_vec[pointer].getOp();
// 		inst_cycle = getLatency(current_inst);
// 		if(inst_cycle == -1){
// 			return ; // since no latency defined for this kind of instruction
// 		}
// 		old = r[15];
// 		newPC = r[15];

// 		// expect user to enter \n in debug mode, executed cycle by cycle
// 		if(Debug == 1){
// 			cout << "Cycle " << cycle_no << ":\n";
// 			cout << "Instruction has started. Before start: \n";
// 			display(inst_vec[pointer], inst_count);
// 			scanf("%c",&c);
// 			while(inst_cycle--){
// 				cycle_no ++;
// 				if(inst_cycle){
// 					cout << "Cycle " << cycle_no << ":\n";
// 					cout << "Instruction in progress. " << inst_cycle << " more cycles to go.\n";
// 					scanf("%c",&c);
// 				}
// 			}
// 			cout << "Cycle " << cycle_no << ":\n";
// 			cout << "Instruction has ended. Current status: \n";

// 			execute(inst_vec[pointer]); // execute the instruction
// 			// handle PC for display
// 			if(r[15] != old){
// 				newPC = r[15]; // if PC has changed then store new value and
// 				r[15] = old; // old value back to PC for display
// 			}
// 			display(inst_vec[pointer], inst_count ++);
// 			scanf("%c",&c);
// 		}
// 		else{
// 			// normal mode of execution
// 			cout << "Cycle " << cycle_no << "-" << cycle_no + inst_cycle << "\n";
// 			cycle_no += inst_cycle;
// 			execute(inst_vec[pointer]); // execute the instruction
// 			// handle PC for display
// 			if(r[15] != old){
// 				newPC = r[15]; // if PC has changed then store new value and
// 				r[15] = old; // old value back to PC for display
// 			}
// 			display(inst_vec[pointer], inst_count++);
// 		}
// 		// handle PC and pointer to get next instruction address
// 		r[15] = newPC;
// 		if(r[15] != old){
// 			// check whether pc is changed after the execution of instruction
// 			pointer = (r[15] - 1000)/4; // update the pointer
// 		}
// 		else{
// 			pointer ++; // if pointer = pc then increment pointer
// 			r[15] += 4; // update PC
// 		}

// 	}
// 	cout << "\n";
// 	st.setInstCount(inst_count - 1); // since it is incremented even after executing the last inst
// 	st.setCycleCount(cycle_no - 1); // since it is always one more than the no of cycles executed 
// 	st.display();
// }