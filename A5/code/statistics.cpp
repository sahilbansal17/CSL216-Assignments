#include "statistics.h"
#include <vector> // used to create vectors
#include <iostream> // used for input/output related utility functions

using namespace std;
// This class handles statistics of the instructions.
int statistics :: find(string s){
    for(int i = 0; i < freqCounter.size(); i++){
        if(freqCounter[i].inst == s){
            return i;
        }
    }
    return -1;
}
// push the instruction if it appeared for the first time
void statistics :: counter(string op){
    int temp = find(op);
    if (temp == -1){  // if instruction op is never executed before.
        Counter temp;
        temp.inst = op;
        temp.count = 1;
        freqCounter.push_back(temp);
    }
    else{
        freqCounter[temp].count ++; // otherwise increase the count of the instruction
    }
}
// display the list of instructions called along with their frequency
void statistics :: display(){
    cout << "Program Statistics : \n\n";
    // frequency of each instruction
    for(int i = 0; i < freqCounter.size(); i++){
        printf("Instruction %10s is called %10d time",freqCounter[i].inst.c_str(), freqCounter[i].count);
        if(freqCounter[i].count > 1){
            cout << "s.\n";
        }
        else{
            cout << ".\n";
        }
    }
    IPC = double(instCount)/double(cycleCount);
    CPI = 1.0/IPC;
    printf("\nTotal %5s no. %5s of %5s Instructions %5s called = %64d\n", "", "", "", "", instCount);
    printf("Total %5s no. %5s of %5s clock cycles %5s taken  = %64d\n", "", "", "", "", cycleCount);
    printf("IPC (Average no. of Instructions per Cycle) %12s = %64f\n", "", IPC);
    printf("CPI (Average no. of Cycles per Instruction) %12s = %64f\n", "", CPI);
}
// set the instruction count after the execution of all the instructions
void statistics :: setInstCount(long int ct){
    instCount = ct;
}
void statistics :: setCycleCount(long int ct){
    cycleCount = ct;
}