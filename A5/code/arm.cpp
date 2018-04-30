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

// to simulate the ALU hardware in the execute stage of the pipeline
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

// returns contents of register at a particular index 
int ARM :: regAtIndex(int i){
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
	// cout << "Error: No latency defined for instruction: " << s << "\n";
	return 1;
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

// to store the instructions in each stage of the pipeline
string pipelinedInstructions[5]; 

// display the contents of the register file and NZCV flags, also the memory
void ARM :: display(int count){
	cout << "Cycle No.: " << count << " => \n";
	cout << "IF : " << pipelinedInstructions[0] << "\n";
	cout << "ID : " << pipelinedInstructions[1] << "\n";
	cout << "EX : " << pipelinedInstructions[2] << "\n";
	cout << "MEM: " << pipelinedInstructions[3] << "\n";
	cout << "WB : " << pipelinedInstructions[4] << "\n";
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

// to simulate the IF stage of the pipeline
// updates the pipeline registers in the IF_ID structure 
void ARM :: IF(){
	// cout<<"Inside IF\n";

	if(IF_ID.instructionIndex >= 0){
		// if not a bubble then only fetch the instruction and get the latency value
		instructions temp = inst_vec[IF_ID.instructionIndex];
		IF_ID.latency_value = getLatency(temp.getOp()); // getting latency of the previous instruction
	}
	else if(IF_ID.instructionIndex == -3){
		IF_ID.latency_value --;		
	}
	else{
		IF_ID.latency_value = 1;
	}
	// Fetching next instruction if latency_value = 1
	if(IF_ID.latency_value == 1){

		// dealing the LOAD-USE DATA HAZARD
		if(IF_ID.PC < inst_vec.size() * 4 + 996){	// checking if not the last instruction 
			string s = ID_EX.inst;	// the instruction which was fetched in the previous clock cycle 
			instructions temp = inst_vec[(IF_ID.PC - 1000)/4 + 1]; // current instruction in the IF stage 
			if((s == "ldr" || s == "ldrImm" || s == "ldr_pseudo" || s == "ldrPre")){			// only regWrite inst are allowed
				if((ID_EX.rd == temp.getRn()) || (EX_MEM.rd == temp.getOp2() && !temp.getImm())){		// comapring the dest. register value with the current registers 
					IF_ID.instructionIndex = -2;	// instruction index -2 indicates stalling of the pipeline
					pipelinedInstructions[0] = "Bubble";	// inserting Bubble in the pipeline
					return;
				}
			}
		}
		
		// if not a branch instruction, PC will remain same in both these stages
		if(IF_ID.PC == ID_EX.PC && IF_ID.PC == r[15]){
			IF_ID.PC += 4;
			r[15] = IF_ID.PC;
		} // else if branch instruction, PC will be updated to branch address in the EX stage
		else{
			if(IF_ID.PC != ID_EX.PC){
				IF_ID.PC = ID_EX.PC;
				r[15] = IF_ID.PC;
			}
			else if(IF_ID.PC != r[15]){
				// if PC updated due to a mov kind of instruction
				IF_ID.PC = r[15];
			}
		}
		
		// if a valid instruction, then update its index
		if(IF_ID.PC < inst_vec.size() * 4 + 1000){
			IF_ID.instructionIndex = (IF_ID.PC - 1000)/4;
		}
		else{
			IF_ID.instructionIndex = -1; // so that will not propagate through the next stages of the pipeline
			IF_ID.latency_value = 1;
		}
		
		// set the instruction in IF stage to be the instruction at this index 
		if(IF_ID.instructionIndex == -1)
			pipelinedInstructions[0] = "";
		else{
			pipelinedInstructions[0].assign(inst_vec[IF_ID.instructionIndex].getFullInst());
		}
	}
	else if(IF_ID.latency_value > 1){
		IF_ID.instructionIndex = -3;	// instruction index -3 indicates stalling of the pipeline due to latency hazard
		pipelinedInstructions[0] = "Bubble due to latency";	// inserting Bubble in the pipeline
	}	
	// cout<<"Outside IF\n";
	
}

// to simulate the ID stage of the pipeline 
void ARM :: ID(){
	// cout << "Inside ID\n" ;
	ID_EX.latency_value = IF_ID.latency_value;
	if(ID_EX.latency_value == 1){
		// input from the IF_ID pipeline register
		ID_EX.PC = IF_ID.PC;
		// after the last instruction has been fetched, IF_ID.instructionIndex will be -1
		// so getting value from a negative index raises error bad_alloc	
		// cout<<"Between Id 1\n";
		
		if(IF_ID.instructionIndex == -1){
			// no instruction in pipeline 
			ID_EX.rn = 0; 
			ID_EX.rd = 0;
			ID_EX.imm = 0;
			ID_EX.inst = "NONE";
			ID_EX.operand2 = 0;
			ID_EX.latency_value = 1;
			pipelinedInstructions[1] = "";
			ID_EX.instructionIndex = -1;
			return;
		}
		else if(IF_ID.instructionIndex == -2){
			// bubble in the pipeline due to load-use hazard
			ID_EX.rn = 0; 
			ID_EX.rd = 0;
			ID_EX.imm = 0;
			ID_EX.inst = "NONE";
			ID_EX.operand2 = 0;
			pipelinedInstructions[1] = "Bubble";
			ID_EX.instructionIndex = -2;
			return;
		}
		// cout<<"Between Id 2\n";
		
		instructions temp = inst_vec[IF_ID.instructionIndex];
		ID_EX.instructionIndex = IF_ID.instructionIndex;
		
		// writing to the ID_EX pipeline registers 
		ID_EX.rn = regAtIndex(temp.getRn());
		ID_EX.rd = temp.getRd();
		ID_EX.imm = temp.getImm();
		ID_EX.inst = temp.getOp();
		ID_EX.operand2 = temp.getOp2();
		// if not a branch instruction
		if(ID_EX.inst[0] != 'b' && ID_EX.inst != "ldr_pseudo"){
			// only if immediate operand exists, then the 2nd operand is in operand2
			if(!ID_EX.imm){
				// since imm is false for a branch instruction that is why only for those which are not branch instructions
				ID_EX.operand2 = regAtIndex(ID_EX.operand2);
			}
		}

		// first handling the data hazard (MEM): MEM/WB.RegisterRd = ID/EX.RegisterRn1
		if(MEM_WB.instructionIndex != -1){
			string s = inst_vec[MEM_WB.instructionIndex].getOp();	// the instruction which was fetcehd two cycles before
			if((s == "ldr" || s == "ldrImm" || s == "ldr_pseudo" || s == "ldrPre" || s == "add" || s == "sub" || s == "mul" || s == "mov")){	// only regWrite inst are allowed
				if(MEM_WB.rd == temp.getRn()){		// comapring the dest register value with the current rn 
					ID_EX.rn = MEM_WB.data;				
				}
				if(MEM_WB.rd == temp.getOp2() && !ID_EX.imm){	// comapring the dest register value with the current operand2
					ID_EX.operand2 = MEM_WB.data;				
				}
			}
		}
		// now handling the data hazard (EX): EX/MEM.RegisterRd = ID/EX.RegisterRn1
		string str = EX_MEM.inst; // taking the instruction of the next pipeline
		if((str == "add" || str == "sub" || str == "mul" || str == "mov") && ID_EX.inst != "ldr_pseudo"){
			// only regWrite inst are allowed and their cannot occur any data hazard in ldr pseudo
			// but as the index of datalabel and register can be same, it was causing unnecessary forwarding in case of ldr_pseudo
			if(EX_MEM.rd == temp.getRn()){			// comparing the destination register value with the current rn 
				ID_EX.rn = EX_MEM.data;			
			}
			if(EX_MEM.rd == temp.getOp2() && !ID_EX.imm){		// comparing the destination register value with the current operand2
				ID_EX.operand2 = EX_MEM.data;			
			}
		}
		// compute result of compare instructions right in this stage of the pipeline
		if(ID_EX.inst == "cmn"){
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
			
			// raise N and Z flags appropriately
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
			
			// raise N and Z flags appropriately
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
		} // handle the branch instructions in the ID stage only =>
		else if(ID_EX.inst == "b"){
			ID_EX.operand2 = temp.getOp2();		
			ID_EX.PC = ID_EX.operand2;
		}
		else if(ID_EX.inst == "bl"){
			ID_EX.operand2 = temp.getOp2();	
			r[14] = r[15] + 4; // update link register (lr)
			ID_EX.PC = ID_EX.operand2;
		}
		else if(ID_EX.inst == "beq"){
			ID_EX.operand2 = temp.getOp2();	
			if(Z == 1){
				ID_EX.PC = ID_EX.operand2;
			}
		}
		else if(ID_EX.inst == "bge"){
			ID_EX.operand2 = temp.getOp2();	
			if(N == 0){
				ID_EX.PC = ID_EX.operand2;
			}
		}
		else if(ID_EX.inst == "blt"){
			ID_EX.operand2 = temp.getOp2();	
			if(N == 1){
				ID_EX.PC = ID_EX.operand2;
			}
		}
		else if(ID_EX.inst == "bne"){
			// cout<< "BNE called\n" ;
			ID_EX.operand2 = temp.getOp2();	
			if(Z == 0){
				ID_EX.PC = ID_EX.operand2;
			}
		}
		else if(ID_EX.inst == "mov" && ID_EX.rd == 15){
			// if mov pc type instruction
			ID_EX.PC = ID_EX.operand2;
			ID_EX.rd = -1;
		}
		// cout << "Outside ID\n" ;
		
		// set the instruction in ID stage to be the instruction at this index
		// the instruction in IF stage previously will run in ID stage now
		pipelinedInstructions[1].assign(inst_vec[ID_EX.instructionIndex].getFullInst());
	}
	else{
		ID_EX.instructionIndex = -3;	// instruction index -3 indicates stalling of the pipeline due to latency
		pipelinedInstructions[1] = "Bubble due to latency";	// inserting Bubble in the pipeline
	}	
}

// int counterEX = 0;
// to simulate the EX stage of the pipeline

void ARM :: EX(){
	EX_MEM.latency_value = ID_EX.latency_value;
	// cout << "LV:" << EX_MEM.latency_value << "\n";
	if(EX_MEM.latency_value == 1){
		EX_MEM.instOnHalt = ID_EX.instructionIndex; // get the correct instruction
	}

	if(IF_ID.instructionIndex == -3 && (ID_EX.inst == "add" || ID_EX.inst == "sub" || ID_EX.inst == "mul" || ID_EX.inst == "mov")){
		if(EX_MEM.latency_value == 1){
			int latency = getLatency(ID_EX.inst);
			pipelinedInstructions[2].assign(inst_vec[EX_MEM.instOnHalt].getFullInst() + " [Cycles Left: " + to_string(latency) + "]");
		}
		else{
			pipelinedInstructions[2].assign(inst_vec[EX_MEM.instOnHalt].getFullInst() + " [Cycles Left: " + to_string(EX_MEM.latency_value - 1) + "]");
		}
		
		EX_MEM.instructionIndex = -1;
		return;	
	}
	else if(IF_ID.instructionIndex != -3 && (ID_EX.inst == "add" || ID_EX.inst == "sub" || ID_EX.inst == "mul" || ID_EX.inst == "mov")){
		EX_MEM.latency_value = 1;
	}
	if(EX_MEM.latency_value == 1){
		// not stalled due to latency 
		// cout<<"Normally passed\n";
		// input from the ID_EX pipeline register 
		EX_MEM.rd = ID_EX.rd;
		// EX_MEM.rn = ID_EX.rn; // not used 
		EX_MEM.inst = ID_EX.inst;
		EX_MEM.instructionIndex = EX_MEM.instOnHalt;
		// writing to the EX_MEM pipeline registers 
		
		// cout << counterEX << " " << EX_MEM.instructionIndex << "\n";
		// counterEX ++; 
		if(EX_MEM.instructionIndex == -1){
			EX_MEM.data = 0;
			pipelinedInstructions[2] = "";
			EX_MEM.latency_value = 1;
			return ;
		}
		else if(EX_MEM.instructionIndex == -2){
			EX_MEM.data = 0;
			pipelinedInstructions[2] = "Bubble";
			return ;
		}

		// if an Arithmetic instruction 
		if(ID_EX.inst == "add" || ID_EX.inst == "sub" || ID_EX.inst == "mul"){
			EX_MEM.data = alu(ID_EX.inst, ID_EX.rn, ID_EX.operand2);		
		} // else if load/store with offset, imm or preOffset
		else if(ID_EX.inst == "ldrImm" || ID_EX.inst == "ldrPre" || ID_EX.inst == "strImm" || ID_EX.inst == "strPre"){
			EX_MEM.data = alu("add", ID_EX.rn, ID_EX.operand2); // call add instruction to get updated address to load/store
			
			// currently post offset is not implemented
			
		} // else if ldr_pseudo or str_pseudo
		else if(ID_EX.inst == "ldr_pseudo" || ID_EX.inst == "str_pseudo" ){
			EX_MEM.data = dlAddress[ID_EX.operand2];
		} // else if normal ldr/str with no offset
		else if(ID_EX.inst == "ldr" || ID_EX.inst == "str"){
			EX_MEM.data = ID_EX.rn;
		} // for move 
		else if(ID_EX.inst == "mov"){
			EX_MEM.data = ID_EX.operand2;
		} // else for compare and branch type instructions
		else{
			if(ID_EX.imm){
				EX_MEM.data = ID_EX.operand2;
			}
			else{
				EX_MEM.data = ID_EX.rn;			
			}
		}
		// set the instruction in EX stage to be the instruction at this index 
		// the instruction in ID stage previously will run in EX stage now
		pipelinedInstructions[2].assign(inst_vec[EX_MEM.instructionIndex].getFullInst());
		EX_MEM.instOnHalt = -1;

		// cout << "Normally exited.\n";
	}
	else{
		ID_EX.instructionIndex = -3;	// instruction index -3 indicates stalling of the pipeline due to latency
		pipelinedInstructions[2] = "Bubble due to latency";	// inserting Bubble in the pipeline
	}
}

// to simulate the MEM stage of the pipeline
void ARM :: MEM(){
	// input from the EX_MEM pipeline register
	MEM_WB.latency_value = EX_MEM.latency_value;
	if(MEM_WB.latency_value == 1){
		MEM_WB.instOnHalt = EX_MEM.instructionIndex; // get the correct instruction
	}

	if(ID_EX.instructionIndex == -3 && (EX_MEM.inst == "ldr" || EX_MEM.inst == "ldrPre" || EX_MEM.inst == "ldr_pseudo" || EX_MEM.inst == "str")){
		if(MEM_WB.instOnHalt != -1){
			if(MEM_WB.latency_value == 1){
				int latency = getLatency(EX_MEM.inst);
				pipelinedInstructions[3].assign(inst_vec[MEM_WB.instOnHalt].getFullInst() + " [Cycles Left: " + to_string(latency) + "]");
			}
			else{
				pipelinedInstructions[3].assign(inst_vec[MEM_WB.instOnHalt].getFullInst() + " [Cycles Left: " + to_string(MEM_WB.latency_value - 1) + "]");
			}
		}
		else{
			pipelinedInstructions[3] = "";
		}
		MEM_WB.instructionIndex = -1;
		return;	
	}
	else if(ID_EX.instructionIndex != -3 && (EX_MEM.inst == "ldr" || EX_MEM.inst == "ldrPre" || EX_MEM.inst == "ldr_pseudo" || EX_MEM.inst == "str")){
		MEM_WB.latency_value = 1;
	}
	// cout<<"Reached\n";
	

	if(MEM_WB.latency_value == 1){
		// cout<<"Normally started MEM\n";
		MEM_WB.rd = EX_MEM.rd;
		MEM_WB.instructionIndex = MEM_WB.instOnHalt;
		if(MEM_WB.instructionIndex == -1){
			MEM_WB.data = 0;
			MEM_WB.regWrite = false;
			pipelinedInstructions[3] = "";
			return;
		}
		else if(MEM_WB.instructionIndex == -2){
			MEM_WB.data = 0;
			MEM_WB.regWrite = false;
			pipelinedInstructions[3] = "Bubble";
			return;
		}		
		/* 
			if ldr instruction then load from memory 
			else if str then store to memory
			else if ALUinstruction then pass the data 
			else do nothing
		*/
		if(EX_MEM.inst == "ldr" || EX_MEM.inst == "ldrImm" || EX_MEM.inst == "ldrPre"){
			MEM_WB.data = memory[(EX_MEM.data - 1000) / 4];
			MEM_WB.regWrite = true;
		}
		else if(EX_MEM.inst == "str" || EX_MEM.inst == "strImm" || EX_MEM.inst == "strPre"){
			memory[(EX_MEM.data - 1000) / 4] = regAtIndex(EX_MEM.rd);
			MEM_WB.regWrite = false;
		}
		else if(EX_MEM.inst == "add" || EX_MEM.inst == "sub" || EX_MEM.inst == "mul" || EX_MEM.inst == "mov" || EX_MEM.inst == "ldr_pseudo"){
			// since data already loaded for ldr_pseudo
			MEM_WB.regWrite = true;
			MEM_WB.data = EX_MEM.data;			
		}
		else if(EX_MEM.inst == "str_pseudo"){
			memory[EX_MEM.data] = regAtIndex(EX_MEM.rd);
			MEM_WB.regWrite = false;
		}
		else{
			MEM_WB.regWrite = false;
			MEM_WB.data = 0;		
		}
		// set the instruction in MEM stage to be the instruction at this index 
		// the instruction in EX stage previously will run in MEM stage now
		pipelinedInstructions[3].assign(inst_vec[EX_MEM.instructionIndex].getFullInst());
		MEM_WB.instOnHalt = -1;
		// cout<<"Normally ended MEM\n";		
	}
	else{
		EX_MEM.instructionIndex = -3;	// instruction index -3 indicates stalling of the pipeline due to latency
		pipelinedInstructions[3] = "Bubble due to latency";	// inserting Bubble in the pipeline
	}
}

// to simulate the WB stage of the pipeline
void ARM :: WB(){
	// cout << "WB started.\n";
	if(MEM_WB.instructionIndex == -1){
		MEM_WB.regWrite = false;
		pipelinedInstructions[4] = "";
		return;
	}
	else if(MEM_WB.instructionIndex == -2){
		MEM_WB.regWrite = false;
		pipelinedInstructions[4] = "Bubble";
		return;
	}
	
	// if regWrite is on, then write to the register
	if(MEM_WB.regWrite == true){
		// cout << "Register" << MEM_WB.rd << ", value" << MEM_WB.data << endl;		
		if(MEM_WB.rd >= 0){
			// to especially handle mov pc type instructions
			r[MEM_WB.rd] = MEM_WB.data;
		}
		MEM_WB.regWrite = false;
	}
	// set the instruction in WB stage to be the instruction at this index 
	// the instruction in MEM stage previously will run in WB stage now
	pipelinedInstructions[4].assign(inst_vec[MEM_WB.instructionIndex].getFullInst());
	// cout << "WB ended.\n";
}

// the main function to run the pipeline till the end of program
void ARM :: run(){
	int cycle_count = 0;
	if(inst_vec.size() == 0){
		cout << "\nNo instructions in the pipeline. \n";
		return;
	}
	pipelinedInstructions[0].assign(inst_vec[0].getFullInst()); // starting instruction
	// extra 16 since additional 4 stages required for the completion of instructions at the end of pipeline
	while(IF_ID.PC < inst_vec.size() * 4 + 1016){
		cycle_count ++;
		display(cycle_count); // since instructions are now in pipelinedInstructions[]
		if(Debug == 1){
			char c;
			scanf("%c",&c);
		}
		// cout << "WB" << "\n";
		WB();
		// cout << "MEM" << "\n";
		MEM();
		// cout << "EX" << "\n";
		EX();
		// cout << "ID" << "\n";
		ID();
		// cout << "IF" << "\n";
		IF();
		// cout << "NEXT" << "\n";
	}
}

/*

// to run the Multi Cycle version of the ARM Simulator

ARM :: void runMultiCycle(vector <instructions> inst_vec){
	long int cycle_no = 1, inst_count = 1;
	int inst_cycle, old, newPC;
	int pointer = 0;
	char c; // for debug mode
	while(pointer != inst_vec.size()){
		// till all the instructions are not executed
		// get the no of clock cycles required to execute this instruction
		string current_inst = inst_vec[pointer].getOp();
		inst_cycle = getLatency(current_inst);
		if(inst_cycle == -1){
			return ; // since no latency defined for this kind of instruction
		}
		old = r[15];
		newPC = r[15];

		// expect user to enter \n in debug mode, executed cycle by cycle
		if(Debug == 1){
			cout << "Cycle " << cycle_no << ":\n";
			cout << "Instruction has started. Before start: \n";
			display(inst_vec[pointer], inst_count);
			scanf("%c",&c);
			while(inst_cycle--){
				cycle_no ++;
				if(inst_cycle){
					cout << "Cycle " << cycle_no << ":\n";
					cout << "Instruction in progress. " << inst_cycle << " more cycles to go.\n";
					scanf("%c",&c);
				}
			}
			cout << "Cycle " << cycle_no << ":\n";
			cout << "Instruction has ended. Current status: \n";

			execute(inst_vec[pointer]); // execute the instruction
			// handle PC for display
			if(r[15] != old){
				newPC = r[15]; // if PC has changed then store new value and
				r[15] = old; // old value back to PC for display
			}
			display(inst_vec[pointer], inst_count ++);
			scanf("%c",&c);
		}
		else{
			// normal mode of execution
			cout << "Cycle " << cycle_no << "-" << cycle_no + inst_cycle << "\n";
			cycle_no += inst_cycle;
			execute(inst_vec[pointer]); // execute the instruction
			// handle PC for display
			if(r[15] != old){
				newPC = r[15]; // if PC has changed then store new value and
				r[15] = old; // old value back to PC for display
			}
			display(inst_vec[pointer], inst_count++);
		}
		// handle PC and pointer to get next instruction address
		r[15] = newPC;
		if(r[15] != old){
			// check whether PC is changed after the execution of instruction
			pointer = (r[15] - 1000)/4; // update the pointer
		}
		else{
			pointer ++; // if pointer = PC then increment pointer
			r[15] += 4; // update PC
		}

	}
	cout << "\n";
	st.setInstCount(inst_count - 1); // since it is incremented even after executing the last inst
	st.setCycleCount(cycle_no - 1); // since it is always one more than the no of cycles executed 
	st.display();
}
*/