#include <iostream> // used for input/output related utility functions 
#include <string> // used for strings
#include <vector> // used to create vectors 
#include <fstream> // used for file handling 
#include <algorithm> // used for transforming string to lowercase 
#include "scan.h" // contains all the code to parse the instructions 
#include "execute.h" // contains all the code to execute the instructions 

using namespace std;

/* 
defined in scan.h :
    class instructions; 
    vector <instructions> inst_vec;
    int getRegisterValue(int &j, string s);
    int getOperand2(int &j, string s, bool &imm);
    int scanMain();

defined in arm.h :
    class ARM;

defined in execute.h :
    ARM myArm;
    void execute(instructions i);
    void display();
*/

int main(){

    // ofstream fout; // to write output to the file
    // fout.open("out.txt"); // specify the output file

    int status = scanMain();

    if(status == -1){
        cout << "\nTerminating...";
    }
    else{
        int no_of_inst = inst_vec.size();
        for(int i = 0 ; i < no_of_inst ; i++){
            execute(inst_vec[i]); // execute the ith instruction
            display(); // display the contents of register 
        }
    } 

    // fout.close(); // to close the output file
    return 0;
}
