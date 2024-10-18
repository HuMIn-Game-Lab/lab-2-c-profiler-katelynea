#include "time.hpp"
#include <chrono>
using namespace std;
// you can put in class and create an object but it doesnt matter 
double getTimeInSeconds(){
    // returns the time in seconds
    // start time is the start time of the profiler 
    static auto start = chrono::high_resolution_clock::now();
    // high resolutions is based of CPU ticks and is not based off of the system clock
    auto now = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::duration<double>>(now - start).count();
}
