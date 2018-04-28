/*
    header file containing the function declarations for implementing the memory hierarchy 
*/

// int blockSize; // size of the block is assumed 1

// one cell of the cache memory 
struct cell{
    bool valid; // valid bit
    bool dirty; // dirty bit
    int tag; 
    int data;
};

class cache{
private:
    static int numBlocks;
    static int setSize; // set associative cache 
    static int numSets;
    cell **mem; // since cell mem[numSets][setSize] is not allowed    
public:
    cache();
    int blockAddress(int byteAddress);          // returns the block address
    int blockNo(int blockAddress);              // returns the block no = (block address) % (num of blocks)
    int index(int blockNo);                     // returns the index = (block no) % (num of sets)
    int getTag(int byteAddress);                // returns the tag from the byte address     
    int read(int byteAddress);                  // returns the value read from the cache memory 
    int loadMEM(int byteAddress);               // returns and loads the value into the cache from the main memory
    void write(int byteAddress, int data);      // writes(updates) the value into the cache memory at the byte address
    void storeMEM(int byteAddress, int data);   // updates/stores the value into the main memory at the byte address
};