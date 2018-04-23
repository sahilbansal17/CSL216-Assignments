#include "scan.h"
#include <vector> // used to create vectors

// This file contains all code relevant to parsing of the instructions and also the class named instructions,
// structure named labels for storing label names and their addresses
using namespace std;

instructions :: instructions(){
	; // default constructor
}
instructions :: instructions(string inst, string _op, int _rd, int _rn, int _op2, bool _imm){
	fullInst = inst;
	op = _op ;
	rd = _rd ;
	rn = _rn ;
	operand2 = _op2 ;
	imm = _imm ;
}
// getter functions
string instructions :: getFullInst(){
	return fullInst;
}
string instructions :: getOp(){
	return op;
}
int instructions :: getRd(){
	return rd;
}
int instructions :: getRn(){
	return rn;
}
int instructions :: getOp2(){
	return operand2;
}
bool instructions :: getImm(){
	return imm;
}
	// setter functions
void instructions :: setOp(string s){
	op = s;
}
void instructions :: setRd(int r){
	rd = r;
}
void instructions :: setRn(int r){
	rn = r;
}
void instructions :: setOp2(int r){
	operand2 = r;
}

vector <instructions> inst_vec; // vector of instructions class type object
// used so that we need to rescan the entire instruction, rather storing it in a format

string supportedInst[14]={"add", "sub", "mul", "mov", "ldr", "str", "cmp", "cmn", "bge", "b", "beq", "bne", "bl", "blt"};

int numSupported = 14;

vector <Label> labels; // to store the labels in the instructions

vector <data_Label> data_labels; // to store the data labels in the instructions

vector <string> str_inst; // vector of instructions string type

// increment the pointer j till point of no space ' ' or NO TABS '\t'
void ignoreSpaces(int &j, string s){
    int len = s.length();
    while(j < len && (s[j] == ' ' || s[j] == '\t')){
        j ++;
    }
}

// gets the register number
int getRegisterValue(int &j, string s){
    /* pattern is :
        1. detect spaces
        2. detect r
        3. detect register number (also check whether valid)
        4. detect spaces
        5. detect comma
    */
    int res; // result register
    string res_s; // result string
    ignoreSpaces(j, s);
    if(s[j] == 'r' || s[j] == 'R'){
		j += 1;
    	while(j < s.length() && s[j] != ' ' && s[j] != ','){
    		// s[j] cannot be other than number, so ascii 48-57
    		if(s[j] < 48 || s[j] > 57){
    			return -1;
    		}
    		res_s += s[j];
    		j ++;
    	}
        res = stoi(res_s); // convert to integer
        if(res >= 0 && res <= 15){
        	ignoreSpaces(j, s);
      		if(s[j++] != ','){
            	return -1;
        	}
        }
        else{
        	// not a valid register number
        	return -1;
        }
    }
	else if((s[j] == 's' && s[j+1] == 'p') || (s[j] == 'S' && s[j+1] == 'P')){
		j += 2;
		ignoreSpaces(j, s);
		if(s[j++] != ','){
			return -1;
		}
		res = 13;
	}
	else if((s[j] == 'l' && s[j+1] == 'r') || (s[j] == 'L' && s[j+1] == 'R')){
		j += 2;
		ignoreSpaces(j, s);
		if(s[j++] != ','){
			return -1;
		}
		res = 14;
	}
	else if((s[j] == 'p' && s[j+1] == 'c') || (s[j] == 'P' && s[j+1] == 'C')){
		j += 2;
		ignoreSpaces(j, s);
		if(s[j++] != ','){
			return -1;
		}
		res = 15;
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
        3. detect register number (also check whether valid)
        4. detect spaces
        5. detect \n
    */
    int res; // result register
    string res_s ; // result string
    ignoreSpaces(j, s);
    if(s[j] == 'r' || s[j] == 'R'){
        j ++; // since we need to check it for #imm2
        while(j < s.length() && s[j] != ' '){
        	// s[j] cannot be other than number, so ascii 48-57
    		if(s[j] < 48 || s[j] > 57){
    			return -1;
    		}
    		res_s += s[j];
    		j ++;
    	}
        res = stoi(res_s); // convert to integer
        if(res >=0 && res <= 15){
        	ignoreSpaces(j, s);
        	if(j == s.length() - 1 || s[j]){ // cannot be any value after register number
            	return -1;
        	}
        }
        else{
        	// not a valid register number
        	return -1;
        }
    }
    else if((s[j] == 's' && s[j+1] == 'p') || (s[j] == 'S' && s[j+1] == 'P')){
		j += 2;
		ignoreSpaces(j, s);
		if(j == s.length() - 1 || s[j]){ // cannot be any value after register number
            return -1;
        }
		res = 13;
	}
	else if((s[j] == 'l' && s[j+1] == 'r') || (s[j] == 'L' && s[j+1] == 'R')){
		j += 2;
		ignoreSpaces(j, s);
		if(j == s.length() - 1 || s[j]){ // cannot be any value after register number
            return -1;
        }
		res = 14;
	}
	else if((s[j] == 'p' && s[j+1] == 'c') || (s[j] == 'P' && s[j+1] == 'C')){
		j += 2;
		ignoreSpaces(j, s);
		if(j == s.length() - 1 || s[j]){ // cannot be any value after register number
            return -1;
        }
		res = 15;
	}
    else if(s[j++] == '#'){
    	ignoreSpaces(j, s);
        string num;
        bool neg = 0;
        if(s[j] == '-'){
        	neg = 1;
        	j ++;
        }
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
        if(neg == 1){
        	res = -1*res; // accepting neg no in operand2
        }
    }
    else{
        return -1; //error handling
    }
    return res;
}

// for the load and store instructions, gets the value of rn and offset and appropriate type of ldr/str
int getRnOffset(int &j, string s, int &rn, int &offset){
    /* pattern is :
        1. 	detect [ (already done before)
        	detect spaces
        2. 	detect r
        3. 	detect register number (also check whether valid)
        4. 	detect spaces
    */
	j ++ ; // because of step 1
    int res; // result register
    string res_s ; // result string
    ignoreSpaces(j, s);
    if(s[j] == 'r' || s[j] == 'R'){
        j ++; // since we need to check it for #imm2
        while(j < s.length() && s[j] != ' ' && s[j] != ',' && s[j]!=']'){
        	// s[j] cannot be other than number, so ascii 48-57
    		if(s[j] < 48 || s[j] > 57){
    			return -1;
    		}
    		res_s += s[j];
    		j ++;
    	}
        res = stoi(res_s); // convert to integer
        if(res >= 0 && res <= 15){
        	ignoreSpaces(j, s);
        }
        else{
        	// not a valid register number
        	return -1;
        }
    }
    else if((s[j] == 's' && s[j+1] == 'p') || (s[j] == 'S' && s[j+1] == 'P')){
		j += 2;
		ignoreSpaces(j, s);
		res = 13;
	}
	else if((s[j] == 'l' && s[j+1] == 'r') || (s[j] == 'L' && s[j+1] == 'R')){
		j += 2;
		ignoreSpaces(j, s);
		res = 14;
	}
	else if((s[j] == 'p' && s[j+1] == 'c') || (s[j] == 'P' && s[j+1] == 'C')){
		j += 2;
		ignoreSpaces(j, s);
		res = 15;
	}
	else{
        return -1; //error handling
    }
    rn = res;
    res_s.clear(); // use it again for getting operand 2
    /*
    	 5. a) 	detect ] => no offset
    			a1)	detect end of line
    			a2)	detect comma and then spaces
    				detect hash and then spaces
    				detect offset value and then spaces and then end of line
    */

    if(s[j++] == ']'){
    	ignoreSpaces(j, s);
    	if(j == s.length()){
    		offset = 0;
        	return 0;	// no offset, BASIC LDR/STR
    	}
    	else{
    		if(s[j++] != ','){
    			return -1;
    		}
    		ignoreSpaces(j, s);
    		if(s[j++] != '#'){
    			return -1;
    		}
    		ignoreSpaces(j, s);

    		// detect offset value
    		string num;
	        bool neg = 0;
	        if(s[j] == '-'){
	        	neg = 1;
	        	j ++;
	        }
	        while(j <= s.length() - 1 && s[j] >= '0' && s[j] <= '9'){
	            num += s[j];
	            j ++;
	        }
	        if(num.length() > 0) res = stoi(num); // convert string to int
	        else {
	        	return -1;
	        }
	        ignoreSpaces(j, s);
	        if(j == s.length() - 1 || s[j]){ // cannot be any value after offset
	            return -1;
	        }
	        if(neg == 1){
	        	res = -1*res; // accepting neg no in operand2
	        }
	        offset = res; // set offset right
	        return 1; // this is the POST-INDEXED LDR/STR
    	}
    }
    else{
    	/*
		5.	b) 	detect comma and then spaces
				detect # and then spaces
				detect offset value and then spaces
				detect ] and then spaces
				b1)	detect end of line
				b2) detect spaces and then '!' and then spaces and then end of line
    	*/
    	j--;
    	if(s[j++] != ','){
    		return -1;
    	}
    	ignoreSpaces(j, s);
    	if(s[j++] != '#'){
    		return -1;
    	}
    	ignoreSpaces(j, s);
    	// detect offset value
		string num;
        bool neg = 0;
        if(s[j] == '-'){
        	neg = 1;
        	j ++;
        }
        while(j <= s.length() - 1 && s[j] >= '0' && s[j] <= '9'){
            num += s[j];
            j ++;
        }
        if(num.length() > 0) res = stoi(num); // convert string to int
        else return -1;
        ignoreSpaces(j, s);
        if(neg == 1){
        	res = -1*res; // accepting neg no in operand2
        }
        offset = res; // set offset right
        if(s[j++] != ']'){
            return -1;
        }
        ignoreSpaces(j, s);
        if(j == s.length()){
	        return 2; // this is IMMEDIATE OFFSET LDR/STR
	    }
	    if(s[j++] == '!'){
	    	ignoreSpaces(j, s);
	    	if(j == s.length() - 1 || s[j]){
	    		return -1;
	    	}
	    	return 3; // this is PRE-INDEXED OFFSET
	    }
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

// checks whether a instruction label exists or not
int checkValidLabel(string label){
	for(int i = 0 ; i < labels.size() ; i++){
		if(label == labels[i].label_name){
			return labels[i].addr; // if it exists then return its address
		}
	}
	return -1;
}
// checks whethea a data label exists or not
int checkValidDataLabel(string label){
	for(int i = 0 ; i < data_labels.size() ; i++){
		if(label == data_labels[i].label_name){
			return i; // if it exists then return its index
		}
	}
	return -1;
}

int scanLabels(){

    string lab; // for the label name
	int valid_inst = 0;

    int tot_lines = str_inst.size(); // total lines of instructions/data/labels to parse
	for(int i = 0; i < tot_lines; i++){

        int j = 0, len_inst = str_inst[i].length();

        ignoreSpaces(j, str_inst[i]); // ignore spaces/tabs in the beginning

        if(j == len_inst){
        	continue; // ignore blank lines
        }
        // get the label/operation name
        while(j < len_inst && str_inst[i][j] != ' ' && str_inst[i][j] != ':'){
            lab += str_inst[i][j];
            j ++;
        }

        // maintaining the original case of the lab, we use tempLab
        string tempLab = lab; // just to check it needs to be transformed to lower case
        transform(tempLab.begin(), tempLab.end(), tempLab.begin(), ::tolower); //convert lab to lower case
        // check whether templab is a valid operation name
        int check = checkValidOp(tempLab);

        if(check == 0){
        	// check whether it can be a label
        	// 1. ignore Spaces
        	// ignoreSpaces(j, str_inst[i]);
        	// 2. check for ':', i.e. a colon
        	if(str_inst[i][j++] != ':'){
        		cout << "Instruction " << i+1 << ": Neither a valid operation nor a valid label.\n";
        		return -1;
        	}
        	else{
        		ignoreSpaces(j, str_inst[i]);
        		// should be nothing after the colon and whitespaces
        		if(j == len_inst - 1 || str_inst[i][j]){
					if(str_inst[i][j] == '.'){
						// case when it is an assembler directive
						// currently only .space is supported
						j ++;
						string directive, val;
						while(j < len_inst && str_inst[i][j] != ' '){
							directive += str_inst[i][j];
							j ++;
						}
						if(directive != "space"){
							cout << "Instruction " << i+1 << ": Currently only .space directive is supported.\n";
							return -1;
						}
						ignoreSpaces(j, str_inst[i]);
						while(j < len_inst && str_inst[i][j] != ' '){
							if(str_inst[i][j] >= 48 && str_inst[i][j] <= 57){
								val += str_inst[i][j];
							}
							else{
								cout << "Instruction " << i+1 << ": Value incorret in .space directive.\n";
								return -1;
							}
							j ++;
						}
						int amount = stoi(val);
						data_Label dl ;
						dl.label_name = lab;
						dl.size = amount;
						data_labels.push_back(dl);
						lab.clear();
						continue;
					}
        			cout << "Instruction " << i+1 << ": Not a valid label (label cannot be followed by any instruction).\n";
        			return -1;
        		}
        		// check whether a label exists with the same name
        		if(checkValidLabel(lab) != -1){
        			cout << "Instruction " << i+1 << ": Cannot declare two labels with the same name.\n";
        			return -1;
        		}
                Label temp;
                temp.label_name = lab;
                temp.addr = (valid_inst)*4 + 1000; // this will be the actual instruction index(0-based) the label points to
        		labels.push_back(temp); // add the label to the vector labels
        		lab.clear(); // clear the string lab
        		continue ; // parse the next instruction
        	}
        }
		else{
			valid_inst++;
			lab.clear(); // clear the string lab
			continue;
        }

    }
    int num_labels = labels.size() + data_labels.size(); // no of labels
    // cout << "Number of labels: " << num_labels << "\n";

    return 1;

}

// the main scanning function
int scanMain(string file_name){

    string inst; // current instruction
    string op; // for the operation name
    int rd, rn, operand2; // for add, sub
    bool imm; // 1 if immediate operand else 0
    // int rm; // for mul
	string label_name ;

	ifstream fin; // to read input from the file
    fin.open(file_name); // specify the input file
	if(!fin){
		cout << "\nError, no such input file.\n";
		return -1;
	}
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

        ignoreSpaces(j, str_inst[i]); // ignore spaces/tabs in the beginning

        if(j == len_inst){
        	continue; // ignore blank lines
        }

        // get the operation name
        while(j < len_inst && str_inst[i][j] != ' ' && str_inst[i][j] != ':'){
            op += str_inst[i][j];
            j ++;
        }

        string temp_dl = op ; // to check a data label
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
        			if(checkValidDataLabel(temp_dl) == -1){
        				cout << "Instruction " << i+1 <<": Not a valid label (label cannot be followed by any instruction).\n";
        				return -1;
        			}
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
			// no need to transform to lowerCase, labels are case sensitive
			int label_addr = checkValidLabel(label_name);
			if(label_addr != -1){
				operand2 = label_addr;
                inst_vec.push_back(instructions(str_inst[i], op, rd, rn, operand2, imm)); // push to the instructions class vector
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
		else if(op == "add" || op == "sub" || op == "mul"){
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
	        if(op == "mul"){
	        	if(imm == 1){
	        		cout << "Instruction " << i+1 << ": Error in operand2, MUL doesn't support immediate operand.\n";
	        		return -1;
	        	}
	        	if(rd == rn){
	        		cout << "Instruction " << i+1 << ": MUL can't have the same register for rd and rn.\n";
	        		return -1;
	        	}
	        }
	        inst_vec.push_back(instructions(str_inst[i], op, rd, rn, operand2, imm)); // push to the instructions class vector
	    }
    	else if(op == "cmp" || op == "mov" || op == "cmn"){
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
			inst_vec.push_back(instructions(str_inst[i], op, 0, rn, operand2, imm)); // rd not needed
		}
		else if(op == "str" || op == "ldr"){
			// get rd
	        rd = getRegisterValue(j, str_inst[i]);
	        if(rd == -1){
	            // error handling
	            cout << "Instruction " << i+1 << ": Error in rd\n";
	            return -1;
	        }
			ignoreSpaces(j, str_inst[i]);
			// now check for opening square bracket '['
			if(str_inst[i][j] == '['){
				int ldr_str_status = getRnOffset(j, str_inst[i], rn, operand2);
				if(ldr_str_status == -1){
					cout << "Instruction " << i+1 << ": Not a valid "<< op << " instruction.\n";
					return -1;
				}
				else if(ldr_str_status == 0){
					// op = "ldr/str"
					operand2 = 0; // no offset
					imm = 0;
					// cout << op << " " << rd << " " << rn << " " << operand2 << "\n";
				}
				else if(ldr_str_status == 1){
					// op = "ldrPost/strPost"
					op += "Post"; imm = 1;
					// cout << op << " " << rd << " " << rn << " " << operand2 << "\n";
				}
				else if(ldr_str_status == 2){
					// op = "ldrImm/strImm"
					op += "Imm"; imm = 1;
					// cout << op << " " << rd << " " << rn << " " << operand2 << "\n";
				}
				else{
					// op = "ldrPre/strPre"
					op += "Pre"; imm = 1;
					// cout << op << " " << rd << " " << rn << " " << operand2 << "\n";
				}
				inst_vec.push_back(instructions(str_inst[i], op, rd, rn, operand2, imm)); // rd not needed
			}
			else if(str_inst[i][j] == '='){
				// ldr arm pseudo instruction
				j ++;
				op += "_pseudo";
				string myDataLabel;
				// cout << myDataLabel << "\n";
				ignoreSpaces(j, str_inst[i]);
				while(j < len_inst && str_inst[i][j] != ' '){
					myDataLabel += str_inst[i][j];
					j ++;
				}
				// cout << myDataLabel << "\n";
				if(myDataLabel.length() == 0){
					cout << "Instruction " << i+1 << ": Not a valid ldr pseudo instruction.(no label specified) \n";
					return -1;
				}
				ignoreSpaces(j, str_inst[i]);
				if(j == len_inst - 1 || str_inst[i][j]){
					cout << "Instruction " << i+1 << ": Not a valid ldr pseudo instruction. \n";
					return -1;
				}
				int dlIndex = checkValidDataLabel(myDataLabel);
				if(dlIndex == -1){
					cout << "Instruction " << i+1 << ": No such label exists.\n";
					return -1;
				}
				// cout << dlIndex << "\n";
				inst_vec.push_back(instructions(str_inst[i], op, rd, 0, dlIndex, 0)); // rn, imm not needed
			}
			else{
				cout << "Instruction " << i+1 << ": Not a valid "<< op << " instruction.\n";
				return -1;
			}
		}
        op.clear(); // clear the string op
        imm = false; // set immediate operand to be false
    }
    int num_inst = inst_vec.size(); // no of instructions
    // cout << "Number of instructions: " << num_inst << "\n";
    // int num_labels = labels.size(); // no of labels
    // cout << "Number of labels: " << num_labels << "\n";

    return 1;
}

int scanLatency(){
	// parse to get the latency_obj vector filled with instructions along with their latency values
	int status = parse();
	if(status == -1){
		return -1;
	}
	/*
	for(int i = 0; i < latency_obj.size(); i++){
		cout << latency_obj[i].command << "--> " << latency_obj[i].clock_cycle << endl;
	}
	*/
	return 1;
}
