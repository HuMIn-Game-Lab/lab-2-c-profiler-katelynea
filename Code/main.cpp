#include "profiler.hpp"
#include<iostream>
#include <cstdlib>
#include <vector>
#include <cstdio>

using namespace std;
Profiler* profiler = nullptr;
void SelectionSort1(int size){
    PROFILER_ENTER("Creating Array");
    vector<int> arr(size);
    // Fill the vector with random values
    srand(static_cast<unsigned int>(time(0)));  // Seed random number generator
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 100;  // Random values between 0 and 99
    }
    PROFILER_EXIT("Creating Array");
    int n = arr.size();
    PROFILER_ENTER("SelectionSort");
    for(int i = 0; i < n-1; i++){
        PROFILER_ENTER("SelectionSortInnerLoop");
        int minIndex = i;
        for(int j = i+1; j < n; j++){
            PROFILER_ENTER("SelectionSortComparison");
            if(arr[j] < arr[minIndex]){
                minIndex = j;
            }
        }
        PROFILER_EXIT("SelectionSortComparison");
        swap(arr[i], arr[minIndex]);
    }
    PROFILER_EXIT("SelectionSortInnerLoop");
    PROFILER_EXIT("SelectionSort");
    
}
void SelectionSort2(int size){
    PROFILER_ENTER("Creating Array2");
    vector<int> arr(size);
    // Fill the vector with random values
    srand(static_cast<unsigned int>(time(0)));  // Seed random number generator
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 100;  // Random values between 0 and 99
    }
    int n = arr.size();
    PROFILER_ENTER("SelectionSort2");
    PROFILER_EXIT("Creating Array2");

    for(int i = 0; i < n-1; i++){
        PROFILER_ENTER("SelectionSortInnerLoop2");
        int minIndex = i;
        for(int j = i+1; j < n; j++){
            PROFILER_ENTER("SelectionSortComparison2");
            if(arr[j] < arr[minIndex]){
                minIndex = j;
            }
        }
        PROFILER_EXIT("SelectionSortComparison2");
        swap(arr[i], arr[minIndex]);
    }
    PROFILER_EXIT("SelectionSort2");

    PROFILER_EXIT("SelectionSortInnerLoop2");
    
}
void RunTest(){
    //Test3();
    int arraySize(500);
    SelectionSort1(arraySize);
    SelectionSort2(arraySize);

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
    profiler->printStatsToXML("Data/profilerStatsXML.xml");


    delete profiler;
    //profiler = nullptr;
    return 0;
}