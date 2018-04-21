#include <vector> // used to create vectors
#include <iostream> // used for input/output related utility functions
using namespace std;

// This class handles statistics of the instructions.
class statistics{
private:
    struct Counter{        // Counter contain the name of instruction and its frequency
        string inst;
        int count;
    };
    vector <Counter> freqCounter;
    long int instCount, cycleCount;
    double IPC, CPI;
public:
    // finds the instruction s in the list
    int find(string s);
    // push the instruction if it appeared for the first time
    void counter(string op);
    // display the list of instructions called along with their frequency
    void display();
    // set the instruction count after the execution of all the instructions
    void setInstCount(long int ct);
    void setCycleCount(long int ct);
};
