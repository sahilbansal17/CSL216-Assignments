#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

int registers[16]; // for storing the registers
int memory[100]; // for memory, assumed to be 100 words (100 * 32 bit)
int startAddress = 1000 ; // start address of the memory

// increment the pointer j till point of no space
void ignoreSpaces(int &j, string s){
    int len = s.length();
    while(j < len && s[j] == ' '){
        j ++;
    }
}

int getRegisterValue(int &j, string s){
    /* pattern is :
        1. detect spaces
        2. detect r
        3. detect register number
        4. detect spaces
        5. detect comma
    */
    int res; // result register
    ignoreSpaces(j, s);
    if(s[j++] == 'r' || s[j++] == 'R'){
        res = s[j++]; // assuming r0-r9
        res -= 48; // get correct number from 0 to 9
        ignoreSpaces(j, s);
        if(s[j++] != ','){
            return -1;
        }
    }
    else{
        return -1; //error handling
    }
    return res;
}

int getOperand2(int &j, string s, bool &imm){
    /* pattern is :
        1. detect spaces
        2. detect r
        3. detect register number
        4. detect spaces
        5. detect \n
    */
    int res; // result register
    ignoreSpaces(j, s);
    if(s[j] == 'r' || s[j] == 'R'){
        j ++; // since we need to check it for #imm2
        res = s[j++]; // assuming r0-r9
        res -= 48; // get correct number from 0 to 9
        ignoreSpaces(j, s);
        // cout << s[j];
        if(j == s.length() - 1 || s[j]){ // cannot be any value after register number
            return -1;
        }
    }
    else if(s[j++] == '#'){
        string num;
        while(j <= s.length() - 1 && s[j] >= '0' && s[j] <= '9'){
            num += s[j];
            j ++;
        }
        if(num.length() > 0) res = stoi(num); // convert string to int
        ignoreSpaces(j, s);
        if(j == s.length() - 1 || s[j]){ // cannot be any value after register number
            return -1;
        }
        imm = true; // immediate operand
    }
    else{
        return -1; //error handling
    }
    return res;
}
int main(){

    vector <string> instructions; // instructions array
    string inst; // current instruction
    string op; // for the operation name
    int rd, rn, operand2; // for add, sub
    bool imm; // 1 if immediate operand else 0
    // int rm; // for mul

    ifstream fin; // to read input from the file
    fin.open("in.txt"); // specify the input file

    ofstream fout; // to write output to the file
    fout.open("out.txt"); // specify the output file

    // till not reached the end of file
    while(!fin.eof()){
        getline(fin, inst); // take input line-by-line
        instructions.push_back(inst);
    }

    int num_inst = instructions.size() - 1; // no of instructions, last instruction is null (new line)
    fout << "Number of instructions: " << num_inst << "\n";
    for(int i = 0; i < num_inst; i++){
        // cout << instructions[i] << "\n";
        fout << "Instruction " << i+1 << ": ";
        int j = 0, len_inst = instructions[i].length();

        // get the operation name
        while(j < len_inst && instructions[i][j] != ' '){
            op += instructions[i][j];
            j ++;
        }
        op[j] = '\n'; // terminate the string op
        // get rd
        rd = getRegisterValue(j, instructions[i]);
        if(rd == -1){
            // error handling
            cout << "Error in rd\n";
            break;
        }
        // get rn
        rn = getRegisterValue(j, instructions[i]);
        if(rn == -1){
            // error handling
            cout << "Error in rn\n";
            break;
        }
        // get operand2
        operand2 = getOperand2(j, instructions[i], imm);
        if(operand2 == -1){
            // error handling
            cout << "Error in operand2\n";
            break;
        }
        if(imm){
            fout << op << " " << rd << " " << rn << " #" << operand2 << "\n";
        }
        else{
            fout << op << " " << rd << " " << rn << " " << operand2 << "\n";
        }
        op.clear(); // clear the string op
        imm = false; // set immediate operand to be false
    }

    fin.close(); // to close the input file
    fout.close(); // to close the output file
    return 0;
}
