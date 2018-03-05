// This file contains all code relevant to parsing of the instructions and also the class named instructions.
using namespace std;

class instructions{

private:
	string op; // operation name 
	int rd, rn, operand2; // the main 3 operands for any kind of instrution
	bool imm; // whether immediate operand or not 
public:
	instructions(){
		; // default constructor 
	}
	instructions(string _op, int _rd, int _rn, int _op2, bool _imm){
		op = _op ;
		rd = _rd ;
		rn = _rn ;
		operand2 = _op2 ;
		imm = _imm ;
	}
	// getter functions 
	string getOp(){
		return op;
	}
	int getRd(){
		return rd;
	}
	int getRn(){
		return rn;
	}
	int getOp2(){
		return operand2;
	}
	bool getImm(){
		return imm;
	}
	// setter functions 
	void setOp(string s){
		op = s;
	}
	void setRd(int r){
		rd = r;
	}
	void setRn(int r){
		rn = r;
	}
	void setOp2(int r){
		operand2 = r;
	}
};

vector <instructions> inst_vec; // vector of instructions class type object 
// used so that we need to rescan the entire instruction, rather storing it in a format 


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
        else return -1;
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

// the main scanning function 
int scanMain(){

	vector <string> str_inst; // vector of instructions string type 
    string inst; // current instruction
    string op; // for the operation name
    int rd, rn, operand2; // for add, sub
    bool imm; // 1 if immediate operand else 0
    // int rm; // for mul

	ifstream fin; // to read input from the file
    fin.open("in.txt"); // specify the input file

    // till not reached the end of file
    while(!fin.eof()){
        getline(fin, inst); // take input line-by-line
        str_inst.push_back(inst);
    }
    fin.close(); // to close the input file

    int num_inst = str_inst.size(); // no of instructions
    cout << "Number of instructions: " << num_inst << "\n";

	for(int i = 0; i < num_inst; i++){
        // cout << str_inst[i] << "\n";
        // cout << "Instruction " << i+1 << ": ";
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
            cout << "Instruction " << i+1 << ": Error in rd\n";
            return -1;
        }
        // get rn
        rn = getRegisterValue(j, str_inst[i]);
        if(rn == -1){
            // error handling
            cout << "Instruction " << i+1 << ": Error in rn\n";
            return -1;
        }
        // get operand2
        operand2 = getOperand2(j, str_inst[i], imm);
        if(operand2 == -1){
            // error handling
            cout << "Instruction " << i+1 << ": Error in operand2\n";
            return -1;
        }
        // if(imm){
        //     cout << op << " " << rd << " " << rn << " #" << operand2 << " => ";
        // }
        // else{
        //     cout << op << " " << rd << " " << rn << " " << operand2 << " => ";
        // }
        transform(op.begin(), op.end(), op.begin(), ::tolower); //convert op to lower case

        inst_vec.push_back(instructions(op, rd, rn, operand2, imm)); // push to the instructions class vector 
        
        op.clear(); // clear the string op
        imm = false; // set immediate operand to be false
    }
    return 1;
}
