// This file contains all code relevant to parsing of the instructions and also the class named instructions.
// #include <stack> // used for assigning labels in one pass of the instructions 
using namespace std;

class instructions{

private:
	string op; // operation name
	int rd, rn, operand2; // the main 3 operands for any kind of instrution
	bool imm; // whether immediate operand or not
   	// operand2 will only act as label for branch instructions, label address (int) will be passed in it
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

string supportedInst[14]={"add", "sub", "mul", "mov", "ldr", "str", "cmp", "cmn", "bge", "b", "beq", "bne", "bl", "blt"};
int numSupported = 14;

struct Label{
    string label_name;
    int addr;
};
vector <Label> labels; // to store the labels in the instructions

// increment the pointer j till point of no space
void ignoreSpaces(int &j, string s){
    int len = s.length();
    while(j < len && s[j] == ' '){
        j ++;
    }
}

// gets the register number
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

// gets either the register number for last operand or immediate op
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

// checks whether an operation is valid
int checkValidOp(string op){
	for(int i = 0 ; i < numSupported ; i++){
		if(op == supportedInst[i]){
			return 1;
		}
	}
	return 0;
}

// checks whether an Label exists or not
int checkValidLabel(string label){
	for(int i = 0 ; i < labels.size() ; i++){
		if(label == labels[i].label_name){
			return labels[i].addr; // if it exists then return its address
		}
	}
	return -1;
}


// stack <int> unassignedLabels;

vector <string> str_inst; // vector of instructions string type

int scanLabels(){

    string lab; // for the label name

    int tot_lines = str_inst.size(); // total lines of instructions/data/labels to parse
	for(int i = 0; i < tot_lines; i++){
       
        int j = 0, len_inst = str_inst[i].length();

        // get the label/operation name
        while(j < len_inst && str_inst[i][j] != ' ' && str_inst[i][j] != ':'){
            lab += str_inst[i][j];
            j ++;
        }
        // lab[j] = '\0'; // terminate the string lab
        transform(lab.begin(), lab.end(), lab.begin(), ::tolower); //convert lab to lower case
        // check whether lab is a valid operation name
        int check = checkValidOp(lab);

        if(check == 0){
        	// check whether it can be a label
        	// 1. ignore Spaces
        	ignoreSpaces(j, str_inst[i]);
        	// 2. check for ':', i.e. a colon
        	if(str_inst[i][j++] != ':'){
        		cout << "Instruction " << i+1 << ": Neither a valid operation nor a valid label.\n";
        		return -1;
        	}
        	else{
        		ignoreSpaces(j, str_inst[i]);
        		// should be nothing after the colon and whitespaces
        		if(j == len_inst - 1 || str_inst[i][j]){
        			cout << "Instruction " << i+1 <<": Not a valid label (label cannot be followed by any instruction).\n";
        			return -1;
        		}
                Label temp;
                temp.label_name = lab;
                temp.addr = (i - labels.size())*4 + 1000; // this will be the actual instruction index(0-based) the label points to
        		labels.push_back(temp); // add the label to the vector labels
        		lab.clear(); // clear the string lab
        		continue ; // parse the next instruction
        	}
        }
		else{
			lab.clear(); // clear the string lab
			continue;
        }
        
    }
    int num_labels = labels.size(); // no of labels
    cout << "Number of labels: " << num_labels << "\n";

    return 1;

}


// the main scanning function
int scanMain(){

    string inst; // current instruction
    string op; // for the operation name
    int rd, rn, operand2; // for add, sub
    bool imm; // 1 if immediate operand else 0
    // int rm; // for mul
	string label_name ;

	ifstream fin; // to read input from the file
    fin.open("in.txt"); // specify the input file

    // till not reached the end of file
    while(!fin.eof()){
        getline(fin, inst); // take input line-by-line
        str_inst.push_back(inst);
    }
    fin.close(); // to close the input file

    int status_labels = scanLabels(); // returns the status whether labels are properly defined 
    if(status_labels == -1){
    	return -1; // return, no more need to scan the instructions 
    }

    int tot_lines = str_inst.size(); // total lines of instructions/data/labels to parse
	for(int i = 0; i < tot_lines; i++){
        // cout << str_inst[i] << "\n";
        // cout << "Instruction " << i+1 << ": ";
        int j = 0, len_inst = str_inst[i].length();

        // get the operation name
        while(j < len_inst && str_inst[i][j] != ' ' && str_inst[i][j] != ':'){
            op += str_inst[i][j];
            j ++;
        }
        // op[j] = '\0'; // terminate the string op
        transform(op.begin(), op.end(), op.begin(), ::tolower); //convert op to lower case
        // check whether op is a valid operation name
        int check = checkValidOp(op);

        if(check == 0){
        	// check whether it can be a label
        	// 1. ignore Spaces
        	ignoreSpaces(j, str_inst[i]);
        	// 2. check for ':', i.e. a colon
        	if(str_inst[i][j++] != ':'){
        		cout << "Instruction " << i+1 << ": Neither a valid operation nor a valid label.\n";
        		return -1;
        	}
        	else{
        		ignoreSpaces(j, str_inst[i]);
        		// should be nothing after the colon and whitespaces
        		if(j == len_inst - 1 || str_inst[i][j]){
        			cout << "Instruction " << i+1 <<": Not a valid label (label cannot be followed by any instruction).\n";
        			return -1;
        		}
           		// its a valid label, so must already be in the labels vector 
        		op.clear(); // clear the string op
        		continue ; // parse the next instruction
        	}
        }
		else if(op[0] == 'b'){
			// if it is a branch instruction
			// 1. ignore spaces
			ignoreSpaces(j, str_inst[i]);
			// 2. get the label name
			while(j < len_inst && str_inst[i][j] != ' '){
				label_name += str_inst[i][j];
				j ++;
			}
			// label_name += '\0'; // add null char
			transform(label_name.begin(), label_name.end(), label_name.begin(), ::tolower); //convert label_name to lower case
			int label_addr = checkValidLabel(label_name);
			if(label_addr != -1){
				operand2 = label_addr;
                inst_vec.push_back(instructions(op, rd, rn, operand2, imm)); // push to the instructions class vector
                label_name.clear(); // clear the label name for further use 
                op.clear(); // clear the string op for next inst
				continue; // move to next instruction 
			}
			else{
				// this could have been the case when the label can be defined somewhere below in the program
				// but since labels are already processed, it must be available 
				cout << "Instruction " << i+1 << ": No such label exists in the file.\n";
				return -1;
			}
		}
		else if(op == "add" || op == "sub"){
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
	        inst_vec.push_back(instructions(op, rd, rn, operand2, imm)); // push to the instructions class vector
	    }
    	else if(op == "cmp"){
			rn = getRegisterValue(j, str_inst[i]);
			if(rn == -1){
				cout << "Instruction " << i+1 << ": Error in rn\n";
				return -1;
			}
			operand2 = getOperand2(j, str_inst[i], imm);
			if(operand2 == -1){
				cout << "Instruction " << i+1 << ": Error in operand2\n";
	            return -1;
			}
			inst_vec.push_back(instructions(op, 0, rn, operand2, imm)); 
		}
        op.clear(); // clear the string op
        imm = false; // set immediate operand to be false
    }
    int num_inst = inst_vec.size(); // no of instructions
    cout << "Number of instructions: " << num_inst << "\n";
    // int num_labels = labels.size(); // no of labels
    // cout << "Number of labels: " << num_labels << "\n";

    return 1;
}
