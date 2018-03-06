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
    private: string op; int rd, rn, operand2; bool imm;
    public:
        instructions();
        instructions(string _op, int _rd, int _rn, int _op2, bool _imm);
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
    string supportedInst[];
    vector <instructions> inst_vec;
    int numSupported;
    vector <Label> labels;
    vector <string> str_inst;
    int getRegisterValue(int &j, string s);
    int getOperand2(int &j, string s, bool &imm);
    int getRnOffset(int &j, string s, int &rn, int &offset);
    int checkValidOp(string op);
    int checkValidLabel(string label);
    int scanLabels();
    int scanMain();

defined in arm.h :
    class ARM{
    private: int r[16], memory[100], startAddress, N, Z, C;
    public:
        ARM(); 
        void add(int rd, int rn, int operand2, bool i);
        void sub(int rd, int rn, int operand2, bool i);
        void mul(int rd, int rn, int operand2);
        void mov(int rd, int operand2, bool i);
        void ldr(int rd, int rn, int offset);
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
        void run(vector <instructions> inst_vec);
    };
*/

int main(){

    // ofstream fout; // to write output to the file
    // fout.open("out.txt"); // specify the output file

    int status = scanMain();

    if(status == -1){
        cout << "\nTerminating...";
    }
    else{
        // myArm.run(inst_vec);
    } 

    // fout.close(); // to close the output file
    return 0;
}
