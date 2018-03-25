#include <iostream> // used for input/output related utility functions
#include <string> // used for strings
#include <vector> // used to create vectors
#include <fstream> // used for file handling
#include <algorithm> // used for transforming string to lowercase
#include "scan.h" // contains all the code to parse the instructions
#include "arm.h" // contain all the code to execute all the arm related instructions
using namespace std;

ARM myArm;
/*
defined in scan.h :
    class instructions{
    private: string op, fullInst; int rd, rn, operand2; bool imm;
    public:
        instructions();
        instructions(string inst, string _op, int _rd, int _rn, int _op2, bool _imm);
        string getFullInst();
        string getOp();
        int getRd();
        int getRn();
        int getOp2();
        bool getImm();
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

defined in arm.h :
    class ARM{
    private:    int r[16], memory[100], startAddress, N, Z, C;
                vector <int> dlAddress;
                statistics st;
    public:
        ARM();
        void add(int rd, int rn, int operand2, bool i);
        void sub(int rd, int rn, int operand2, bool i);
        void mul(int rd, int rn, int operand2);
        void mov(int rd, int operand2, bool i);
        void ldr(int rd, int rn, int offset);
        void ldrPseudo(int rd, int operand2);
        void str(int rd, int rn, int offset);
        int getR_atIndex(int i);
        int getN();
        int getZ();
        int getC();
        void cmp(int r1, int r2, bool i);
        void cmn(int r1, int r2, bool i);
        void bge(int label);
        void b(int label);
        void beq(int label);
        void bl(int label);
        void blt(int label);
        void bne(int label);
        void offset(int r1, int offset);
        void execute(instructions i);
        void display();
        void allocate(vector<data_Label> data_labels);
        void runSingleCycle(vector <instructions> inst_vec);
        int getLatency(string s);
        void runMultiCycle(vector <instructions> inst_vec);
        void showLatencyData();
    };

defined in latency.h :
    struct latency{
        char *command;
        int clock_cycle;
    };
    extern vector <latency> latency_obj;
    extern int parse(void);

defined in statistics.h :
    class Statistics{
    private:
        struct Counter{
            string inst; inst count;
        }
        vector <Counter> freqCounter;
        long int instCount, cycleCount;
        double IPC, CPI;
    public:
        int find(string s);
        void counter(string op);
        void display();
        void setInstCount(long int ct)
        void setCycleCount(long int ct)
    }

file scanLatency.l :
    lex file to scan "latency.txt" and return the tokens to the parser and also handle errors

file parseLatency.y :
    yacc file to parse the tokens and after checking the grammar, push the data related to the cycles of a particular instruction to the latency_obj vector.

*/

int main(int argc, char * argv[]){

    // ofstream fout; // to write output to the file
    // fout.open("out.txt"); // specify the output file

    // first scan the latency text file
    int status = scanLatency(); // read from latency.txt and create a latency_obj vector
    if(status == -1){
        cout << "Latency text file has errors. No further execution possible.\n";
        return 0;
    }
    myArm.showLatencyData(); // shows the latency associated with each instruction

    // now, if latency.txt has no errors, scan the input containing assembly code
    string file_name = argv[1];
    status = scanMain(file_name); // used to scan the input and create an instructions vector
    if(status == -1){
        cout << "\nTerminating...";
    }
    else{
        myArm.allocate(data_labels); // to allocate memory to data labels
        myArm.runMultiCycle(inst_vec); // to run the instructions
    }

    // fout.close(); // to close the output file
    return 0;
}
