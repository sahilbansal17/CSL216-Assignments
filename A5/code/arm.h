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
	ARM();
};
