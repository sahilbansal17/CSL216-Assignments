#include "latency.h"
// This file contains all code relevant to parsing of the instructions and also the class named instructions,
// structure named labels for storing label names and their addresses
#include <iostream> // used for input/output related utility functions
#include <string> // used for strings
#include <vector> // used to create vectors
#include <fstream> // used for file handling
#include <algorithm> // used for transforming string to lowercase
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
	bool getImm();
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

/*
string supportedInst[];
int numSupported;
*/

extern vector <instructions> inst_vec; // vector of instructions class type object
// used so that we need to rescan the entire instruction, rather storing it in a format

extern vector <Label> labels; // to store the labels in the instructions

extern vector <data_Label> data_labels; // to store the data labels in the instructions

extern vector <string> str_inst; // vector of instructions string type

void ignoreSpaces(int &j, string s);

int getRegisterValue(int &j, string s);
int getOperand2(int &j, string s, bool &imm);
int getRnOffset(int &j, string s, int &rn, int &offset);
int checkValidOp(string op);
int checkValidLabel(string label);
int checkValidDataLabel(string label);
int scanLabels();
int scanMain(string file_name);
int scanLatency();