#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

int r[16]; // for storing the registers
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

// function to perform add instruction
void add(int rd, int rn, int operand2, bool i){ 
    if(i == true){
        r[rd] = r[rn] + operand2;
    }
    else{
        r[rd] = r[rn] + r[operand2];
    }
}
// function to perform sub instruction
void sub(int rd, int rn, int operand2, bool i){ 
    if(i == true){
        r[rd] = r[rn] - operand2;
    }
    else{
        r[rd] = r[rn] - r[operand2];
    }
}
// function to perform mul instruction
void mul(int rd, int rn, int operand2){ 
    r[rd] = r[rn] * r[operand2];
}
// function to perform mov instruction
void mov(int rd, int operand2, bool i){
    if(i == true){
        r[rd] = operand2;
    }
    else{
        r[rd] = r[operand2];
    }
}
// function to perform ldr instruction  ( in progress )
void ldr(int rd, int operand2){ 
    r[rd] = memory[operand2];
}
// function to perform str instruction ( in progress )
void str(int rd, int operand2){ 
    memory[operand2] = r[rd];
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

    int num_inst = instructions.size(); // no of instructions
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
            fout << "Error in rd\n";
            break;
        }
        // get rn
        rn = getRegisterValue(j, instructions[i]);
        if(rn == -1){
            // error handling
            fout << "Error in rn\n";
            break;
        }
        // get operand2
        operand2 = getOperand2(j, instructions[i], imm);
        if(operand2 == -1){
            // error handling
            fout << "Error in operand2\n";
            break;
        }
        if(imm){
            fout << op << " " << rd << " " << rn << " #" << operand2 << " => ";
        }
        else{
            fout << op << " " << rd << " " << rn << " " << operand2 << " => ";
        }
        transform(op.begin(), op.end(), op.begin(), ::tolower); //convert op to lower case
        if(op == "add"){
            add(rd, rn, operand2, imm); //call to add function
        }
        else if(op == "sub"){
            sub(rd, rn, operand2, imm); //call to sub function
        }
        else if(op == "mul"){
            mul(rd, rn, operand2); //call to mul function
        }
        else if(op == "mov"){
            mov(rd, operand2, imm); //call to mov function
        }
        else if(op == "ldr"){
            ldr(rd, operand2);
        }
        else if(op == "str"){
            str(rd, operand2);
        }
        // display the contents of the register file 
        for(int j = 0; j < 10; j++){
            fout << "|r" << j << "=" << r[j] ;
        }
        fout << "|\n";
        op.clear(); // clear the string op
        imm = false; // set immediate operand to be false
    }

    fin.close(); // to close the input file
    fout.close(); // to close the output file
    return 0;
}
