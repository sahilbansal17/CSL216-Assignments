#include <vector>
using namespace std;
struct latency{
        char *command;
        int clock_cycle;
    };
extern vector <latency> latency_obj;
extern int parse(void);