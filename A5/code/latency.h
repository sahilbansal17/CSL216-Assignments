#include <vector>
using namespace std;
// structure for latency to be used in scanLatency.l
struct latency{
    char *command;
    int clock_cycle;
};
// the vector storing the latency objects after reading from latency.txt
extern vector <latency> latency_obj;
// the function to be called to parse the latency.txt file and fill the vector
extern int parse(void);
