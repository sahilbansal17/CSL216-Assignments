/*
    contains the definitions for the functions implementing the memory hierarchy
*/

#include "memory.h"

cache :: cache(){
    int numBlocks   = 12;
    int setSize     = 03; // set associative cache 
    int numSets     = numBlocks/setSize;
    mem = new cell*[numSets];
    for(int i = 0 ; i < numSets; i ++){
        mem[i] = new cell[setSize];
        for(int j = 0 ; j < setSize; j ++){
            mem[i][j].valid    = false;
            mem[i][j].dirty    = false;
            mem[i][j].tag      = 0;
            mem[i][j].data     = 0;
        }
    }
}