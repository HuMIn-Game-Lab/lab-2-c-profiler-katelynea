#include "profiler.hpp"
#include<iostream>
#include <cstdlib>
#include <vector>
#include <cstdio>

using namespace std;
Profiler* profiler = nullptr;
void Test3(){
    PROFILER_ENTER("Test3 Loop");
    for(int i = 0; i < 1000; i++){
        PROFILER_ENTER("Test3Inner");
        PROFILER_EXIT("Test3Inner");
    }
    PROFILER_EXIT("Test3 Loop");

}

// interleaving test example
void Test2(){
    PROFILER_ENTER("Test2 Loop");
    for(int i = 0; i < 1000; i++){
        PROFILER_ENTER("Test2Inner");
        PROFILER_EXIT("Test2 Loop");
        PROFILER_EXIT("Test2Inner");
    }

}
void RunTest(){
    Test2();
    Test3();

}

int main(int argc, char** argv) {
    profiler = Profiler::getInstance();
    RunTest();
    profiler->storeDetailedStatsbySectionCSV("Data/detailedStats.csv");
    system("python ./Code/detailedStatsVisual.py");
    profiler->calculateStats();
    profiler->printStats();
    profiler->printStatsToCSV("Data/profilerStatsCSV.csv");
    system("python ./Code/profilerStatsVisual.py");
    profiler->printStatstoJSON("Data/profilerStatsJSON.json");
    // profiler->printStatsToXML("Data/profilerStatsXML.xml");


    delete profiler;
    //profiler = nullptr;
    return 0;
}