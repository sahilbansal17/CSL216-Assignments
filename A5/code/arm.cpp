#include "arm.h"
using namespace std;

ARM :: ARM(){
	for(int i = 0 ; i < 13 ; i ++){
		r[i] = 0;
	}
	r[13] = 1400; // stack pointer pointing at the end of memory location
	r[14] = 0;
	r[15] = 1000; // instruction addr starts with 1000
	for(int i = 0 ; i < 100 ; i ++){
		memory[i] = 0;
	}
	startAddress = 1000;
	N = 0; Z = 0; C = 0;
}
