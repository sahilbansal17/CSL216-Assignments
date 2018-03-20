// This class handles statistics of the instructions.
class statistics{
    private:
    struct Counter{        // Counter contain the name of instruction and its frequency
        string inst;
        int count;
    };
    vector <Counter> freqCounter;

    public:
    // finds the instruction s in the list
    int find(string s){
        for(int i = 0; i < freqCounter.size(); i++){
            if(freqCounter[i].inst == s){
                return i;
            }
        }
        return -1;
    }
    // push the instruction if it appeared for the first time
    void counter(string op){
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
    void display()
    {
        for(int i = 0; i < freqCounter.size(); i++){
            cout << "Instruction " << freqCounter[i].inst << " is called " << freqCounter[i].count << " number of times\n";
        }
    }
};
