#include <iostream> // used for input/output related utility functions
#include <string> // used for strings
#include <vector> // used to create vectors
#include <fstream> // used for file handling
#include <algorithm> // used for transforming string to lowercase
#include "arm.h" // contain all the code to execute all the arm related instructions
using namespace std;

ARM myArm;

int main(int argc, char * argv[]){

    // ofstream fout; // to write output to the file
    // fout.open("out.txt"); // specify the output file

    // first scan the latency text file
    int status = scanLatency(); // read from latency.txt and create a latency_obj vector
    if(status == -1){
        cout << "Latency text file has errors. No further execution possible.\n";
        return 0;
    }
    // myArm.showLatencyData(); // shows the latency associated with each instruction

    // now, if latency.txt has no errors, scan the input containing assembly code
    string file_name = argv[1];
    status = scanMain(file_name); // used to scan the input and create an instructions vector
    if(status == -1){
        cout << "\nTerminating...";
    }
    else{
        myArm.allocate(data_labels); // to allocate memory to data labels
        myArm.run(); // to run the instructions
    }

    // fout.close(); // to close the output file
    return 0;
}
