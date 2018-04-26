#include "statistics.h"
#include "scan.h" // contains all the code to parse the instructions

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

	struct IF_ID{
        int PC;
        int instructionIndex;
		IF_ID(){
			PC = 1000; instructionIndex = 0; // the beginning instruction of the program
		}
    }IF_ID;

    struct ID_EX{
        int PC;
        int rn; // first operand 
        int operand2; // second operand 
        int rd; // destination register 
        bool imm; // whether immediate operand
		int instructionIndex; 
        string inst;
		ID_EX(){
			PC = 1000; rn = 0; operand2 = 0; rd = 0; imm = false; inst = "NULL";
			instructionIndex = -1;
		}
    }ID_EX;

    struct EX_MEM{
        int rd; // destination register 
		int data; // result of the ALU operation:
		/*	can be:
			1. data to be stored to a register |
			2. memory location for LDR 
		*/
        string inst;
		int instructionIndex;
		EX_MEM(){
			instructionIndex = -1;
			data = 0; rd = 0; inst = "NULL";
		}
    }EX_MEM;

    struct MEM_WB{
        int data; // the data to be written to register
        int rd; // the destination register or register whose data to be stored to memory
        bool regWrite; // control signal for writing the register
		int instructionIndex;
		MEM_WB(){
			data = 0; rd = 0; regWrite = false;
			instructionIndex = -1;
		}
    }MEM_WB;

public:
	ARM();
	int alu(string inst, int a, int b);
	void mem(string inst, int addr, int data);
	int regAtIndex(int i);
	int getLatency(string s);
	void showLatencyData();
	void allocate(vector<data_Label> data_labels);
    void display(int count);
	void run();
    void IF();
    void ID();
    void EX();
    void MEM();
    void WB();
    int getN();
    int getZ();
    int getC();
    
};
