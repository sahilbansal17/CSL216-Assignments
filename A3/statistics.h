// This class handles statistics of the instructions.
class statistics{
    private:
    struct List{        // List contain the name of instruction and its frequency
        string func;
        int count;
    };
    vector <List> list;

    public:
    // finds the instruction s in the list
    int find(string s){
        for(int i=0;i<list.size();i++){
            if(list[i].func == s){
                return i;
            }
        }
        return -1;
    }
    //push the instruction if it never called before
    void counter(string op){
        int temp = find(op);
        if ( temp == -1 ){  // if instruction op is never executed before.
            List val;
            val.func=op;
            val.count=1;       
            list.push_back(val);
        }
        else{
            list[temp].count++; // increase the count of the instruction if called before.
        }        
    }
    //display the list of instructions called along with its frequency
    void display()
    {
        for(int i=0;i<list.size();i++){
            cout<<"Instruction "<<list[i].func<<" is called "<<list[i].count<<" number of times\n";
        }
    }
};