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
    class instructions; 
    vector <instructions> inst_vec;
    int getRegisterValue(int &j, string s);
    int getOperand2(int &j, string s, bool &imm);
    int scanMain();

defined in arm.h :
    class ARM;
*/

int main(){

    // ofstream fout; // to write output to the file
    // fout.open("out.txt"); // specify the output file

    int status = scanMain();

    if(status == -1){
        cout << "\nTerminating...";
    }
    else{
        myArm.run(inst_vec);
    } 

    // fout.close(); // to close the output file
    return 0;
}
