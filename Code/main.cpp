#include "profiler.hpp"
#include<iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;
constexpr float DEGREES_TO_RADIANS = 3.14159f/180.f;
Profiler* profiler = nullptr;

void Test3(){
   
constexpr int TRIG_TEST_NUM_ENTRIES = 100000;
float randomYawDegreeTable[TRIG_TEST_NUM_ENTRIES]= {};
for (int i =0; i < TRIG_TEST_NUM_ENTRIES; i++){
    PROFILER_ENTER("Random Angle Generation");

    randomYawDegreeTable[i] = 360.f * float(rand())/float(RAND_MAX);
}
PROFILER_EXIT("Random Angle Generation");
//profiler->ExitSection("Random Angle Generation", __LINE__, __FILE__, __FUNCTION__);
// make above macro
// make a macro for the line number, file name, and function name
float biggestSoFar = 0.0f;

PROFILER_ENTER("Compute Cos and Sin");
for(int i =0; i < TRIG_TEST_NUM_ENTRIES; i++){
    PROFILER_ENTER("Cos and Sin Compute");
    float yawDegrees = randomYawDegreeTable[i];
    float cosDegrees = cosf(yawDegrees * DEGREES_TO_RADIANS);
    float sinDegrees = sinf(yawDegrees * DEGREES_TO_RADIANS);

    PROFILER_EXIT("Cos and Sin Compute");
    if(cosDegrees +sinDegrees > biggestSoFar){
        biggestSoFar = cosDegrees + sinDegrees;
    }
}
PROFILER_EXIT("Compute Cos and Sin");
//PROFILER_EXIT("Trig Speed Test");
    cout<< "Biggest cos+sin= "<< biggestSoFar<<endl;

}

void Test4(){
    int numberVal =9;

    for (int i =0; i< 5; i++)
    {
    PROFILER_ENTER("Loop 1");

        numberVal++;
        for (int j =0; j< 5; j++)
        {
            PROFILER_ENTER("Loop 2");

            numberVal++;
            PROFILER_EXIT("Loop 2");

        }
        PROFILER_EXIT("Loop 1");;

    }
    cout<< "END of Cycle"<<endl;
}
void RunTest(){
    Test3();
    // Test4();
}
int main(int argc, char** argv) {
    profiler = Profiler::getInstance();
    RunTest();
    profiler->storeDetailedStatsbySectionCSV("Data/detailedStats.csv");

    profiler->calculateStats();
    profiler->printStats();
    profiler->printStatsToCSV("Data/profilerStatsCSV.csv");
    profiler->printStatsToXML("Data/profilerStatsXML.xml");

    delete profiler;
    profiler = nullptr;
    return 0;
}

// compile: 
// 	clear 
// 	clang++ -g -std=c++14 ./Code/*.cpp -o output
// 	./output
// 	@echo "Running Python visualization..."
// 	@conda run -n intromlenv python ./Code/visualize_detailed_profiling.py
// 	@conda run -n intromlenv python ./Code/visualize_profilerStats.py