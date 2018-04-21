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
        int pc;
        int i;
		IF_ID(){pc=1000;i=0;}
    }IF_ID;

    struct ID_EX{
        int pc;
        int rn;
        int operand2;
        int rd;
        bool imm;
        string inst;
		ID_EX(){pc=0;rn=0;operand2=0;rd=0;imm=false;inst="";}
    }ID_EX;

    struct EX_MEM{
        int pc;
        int rd;
        int data;
        int rn;
        string inst;
		EX_MEM(){pc=0;rn=0;data=0;rd=0;inst="";}
    }EX_MEM;

    struct MEM_WB{
        int data;
        int rd;
        int reg_write;
		MEM_WB(){data=0;rd=0;reg_write=0;}
    }MEM_WB;

public:
	ARM();
	int alu(string inst, int a, int b);
	void mem(string inst, int addr, int data);
	int reg_file(int i);
	int getLatency(string s);
	void showLatencyData();
	void allocate(vector<data_Label> data_labels);
    void display(instructions i, int count);
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
