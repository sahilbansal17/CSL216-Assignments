class ARM{
private:
	int r[16]; // for storing the registers
	int memory[100]; // for memory, assumed to be 100 words (100 * 32 bit)
	int startAddress; // start address of the memory
public:
	ARM(){
		for(int i = 0 ; i < 16 ; i ++){
			r[i] = 0;
		}
		for(int i = 0 ; i < 100 ; i ++){
			memory[i] = 0;
		}
		startAddress = 1000;
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
	// function to perform str instruction ( in progress )
	void str(int rd, int operand2){
	    memory[operand2] = r[rd];
	}

	// function to get register value of a particular reg 
	int getR_atIndex(int i){
		return r[i];
	}
};