#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "scan.h" // contains all the code to parse the instructions 
#include "execute.h" // contains all the code to execute the instructions 

using namespace std;
vector <instructions> inst_vec; // vector of instructions class type object 
// used so that we need to rescan the entire instruction, rather storing it in a format 

int main(){

    vector <string> str_inst; // vector of instructions string type 
    string inst; // current instruction
    string op; // for the operation name
    int rd, rn, operand2; // for add, sub
    bool imm; // 1 if immediate operand else 0
    // int rm; // for mul

    ifstream fin; // to read input from the file
    fin.open("in.txt"); // specify the input file

    // ofstream fout; // to write output to the file
    // fout.open("out.txt"); // specify the output file

    // till not reached the end of file
    while(!fin.eof()){
        getline(fin, inst); // take input line-by-line
        str_inst.push_back(inst);
    }

    int num_inst = str_inst.size(); // no of instructions
    cout << "Number of instructions: " << num_inst << "\n";
    for(int i = 0; i < num_inst; i++){
        // cout << str_inst[i] << "\n";
        cout << "Instruction " << i+1 << ": ";
        int j = 0, len_inst = str_inst[i].length();

        // get the operation name
        while(j < len_inst && str_inst[i][j] != ' '){
            op += str_inst[i][j];
            j ++;
        }
        op[j] = '\n'; // terminate the string op
        // get rd
        rd = getRegisterValue(j, str_inst[i]);
        if(rd == -1){
            // error handling
            cout << "Error in rd\n";
            break;
        }
        // get rn
        rn = getRegisterValue(j, str_inst[i]);
        if(rn == -1){
            // error handling
            cout << "Error in rn\n";
            break;
        }
        // get operand2
        operand2 = getOperand2(j, str_inst[i], imm);
        if(operand2 == -1){
            // error handling
            cout << "Error in operand2\n";
            break;
        }
        if(imm){
            cout << op << " " << rd << " " << rn << " #" << operand2 << " => ";
        }
        else{
            cout << op << " " << rd << " " << rn << " " << operand2 << " => ";
        }
        transform(op.begin(), op.end(), op.begin(), ::tolower); //convert op to lower case

        inst_vec.push_back(instructions(op, rd, rn, operand2, imm)); // push to the instructions class vector 
        
        execute(inst_vec[i]); 
        display();

        op.clear(); // clear the string op
        imm = false; // set immediate operand to be false
    }

    fin.close(); // to close the input file
    // fout.close(); // to close the output file
    return 0;
}
