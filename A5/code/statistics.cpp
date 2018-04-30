#include "statistics.h"

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
    FILE *fp; // to write output to the file
    fp = fopen("statistics.txt", "w"); // specify the output file
    fprintf(fp,"Program Statistics : \n\n");
    // frequency of each instruction
    for(int i = 0; i < freqCounter.size(); i++){
        fprintf(fp, "Instruction %10s is called %10d time",freqCounter[i].inst.c_str(), freqCounter[i].count);
        if(freqCounter[i].count > 1){
            fprintf(fp, "s.\n");
        }
        else{
            fprintf(fp, ".\n");
        }
    }
    CPI = 1 + (4 + cycleCount) / instCount;
    IPC = 1 / CPI;
    fprintf(fp, "\nTotal %5s no. %5s of %5s Instructions %5s called = %64d\n", "", "", "", "", instCount);
    fprintf(fp, "Total %5s no. %5s of %5s clock cycles %5s taken  = %64d\n", "", "", "", "", cycleCount);
    fprintf(fp, "Total %5s no. %5s of %5s stalls %15s    = %64d\n","", "", "", "",stallCount);
    fprintf(fp, "Stalls per instruction  %32s = %64f\n","",(double)stallCount / (double)instCount);
    fprintf(fp, "IPC (Average no. of Instructions per Cycle) %12s = %64f\n", "", IPC);
    fprintf(fp, "CPI (Average no. of Cycles per Instruction) %12s = %64f\n", "", CPI);
    if(Debug == 0){
        fprintf(fp, "Program Execution Time (milliseconds) %18s = %64f\n","",executionTime);
        fprintf(fp, "Avg value of MIPS %38s = %64f\n ","",instCount / (executionTime * 1000));
    }
    fclose(fp); // to close the output file    
}
// set the instruction count after the execution of all the instructions
void statistics :: setInstCount(long int ct){
    instCount = ct;
}
void statistics :: setCycleCount(long int ct){
    cycleCount = ct;
}
void statistics :: setExecutionTime(double ct){
    executionTime = ct;
}
void statistics :: setStallCount(long int ct){
    stallCount = ct;
}