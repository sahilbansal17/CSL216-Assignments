#include "latency.h"
// This file contains all code relevant to parsing of the instructions and also the class named instructions,
// structure named labels for storing label names and their addresses
using namespace std;

class instructions{

private:
	string op, fullInst; // operation name
	int rd, rn, operand2; // the main 3 operands for any kind of instrution
	bool imm; // whether immediate operand or not
   	// operand2 will only act as label for branch instructions, label address (int) will be passed in it
public:
	instructions();
	instructions(string inst, string _op, int _rd, int _rn, int _op2, bool _imm);
	// getter functions
	string getFullInst();
	string getOp();
	int getRd();
	int getRn();
	int getOp2();
	bool getImm()
	// setter functions
	void setOp(string s);
	void setRd(int r);
	void setRn(int r);
	void setOp2(int r);
};

struct Label{
	string label_name;
	int addr;
};

struct data_Label{
	string label_name;
	int size;
};

string supportedInst[];

vector <instructions> inst_vec;

int numSupported;

vector <Label> labels;
vector <data_Label> data_labels;
vector <string> str_inst;

void ignoreSpaces(int &j, string s);

int getRegisterValue(int &j, string s);
int getOperand2(int &j, string s, bool &imm);
int getRnOffset(int &j, string s, int &rn, int &offset);
int checkValidOp(string op);
int checkValidLabel(string label);
int checkValidDataLabel(string label);
int scanLabels();
int scanMain();
int scanLatency();