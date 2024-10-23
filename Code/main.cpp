#include "profiler.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <thread>
#include <chrono>

using namespace std;

Profiler* profiler = nullptr;

// Function to start Python HTTP server
void startServer() {
    // First kill any existing Python servers
    #ifdef _WIN32
    system("taskkill /F /IM python.exe > nul 2>&1");
    system("start /B python -m http.server 8080");
    #else
    system("pkill -f 'python3 -m http.server' > /dev/null 2>&1");
    system("python3 -m http.server 8080 &");
    #endif
    
    // Give the server time to start
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

// Function to open browser for both visualizers
void openBrowsers() {
    #ifdef _WIN32
    system("start http://localhost:8080/Code/profiler-visualizer.html");
    system("start http://localhost:8080/Code/detailed-profiler-visualizer.html");
    #elif __APPLE__
    system("open http://localhost:8080/Code/profiler-visualizer.html");
    system("open http://localhost:8080/Code/detailed-profiler-visualizer.html");
    #else
    system("xdg-open http://localhost:8080/Code/profiler-visualizer.html");
    system("xdg-open http://localhost:8080/Code/detailed-profiler-visualizer.html");
    #endif
}

void cleanupServer() {
    #ifdef _WIN32
    system("taskkill /F /IM python.exe > nul 2>&1");
    #else
    system("pkill -f 'python3 -m http.server' > /dev/null 2>&1");
    #endif
}

void bubbleSort1(vector<int>& arr) {
    PROFILER_ENTER("bubbleSort1");

    int n = arr.size();  // Ensure size is correctly determined
    for (int i = 0; i < n - 1; i++) {
        PROFILER_ENTER("bubbleSort1-inner");
        for (int j = 0; j < n - i - 1; j++) {
            PROFILER_ENTER("bubbleSort1-inner-inner");
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
            PROFILER_EXIT("bubbleSort1-inner-inner");
        }

        PROFILER_EXIT("bubbleSort1-inner");
    }
    PROFILER_EXIT("bubbleSort1");
}
void bubbleSort2(vector<int>& arr) {
    PROFILER_ENTER("bubbleSort2");
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        PROFILER_ENTER("bubbleSort2-inner");
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            PROFILER_ENTER("bubbleSort2-inner-inner");
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
            PROFILER_EXIT("bubbleSort2-inner-inner");
        }
        // Break the loop if no elements were swapped
        if (!swapped) {
            break;
        }
        PROFILER_EXIT("bubbleSort2-inner");
    }
    PROFILER_EXIT("bubbleSort2");
}
void bubbleSort3(vector<int>& arr) {
    PROFILER_ENTER("bubbleSort3");
    int n = arr.size();
    int newN;
    do {
        newN = 0;
        for (int i = 0; i < n - 1; i++) {
        PROFILER_ENTER("bubbleSort3-inner");

            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                newN = i + 1;  // Update the boundary of the sorted section
            }
            PROFILER_EXIT("bubbleSort3-inner");
        }
        n = newN;
    } while (newN > 0);
    PROFILER_EXIT("bubbleSort3");
}

void RunTest(){
    vector<int> vectarr = {112, 64, 34, 25, 109,103, 12,105, 22, 11, 90, 1, 92, 75, 88, 99, 101, 100, 102};

    bubbleSort1(vectarr);
    bubbleSort2(vectarr);
    bubbleSort3(vectarr);

}
int main(int argc, char** argv) {
    cleanupServer();

    profiler = Profiler::getInstance();
    RunTest();
    profiler->storeDetailedStatsbySectionCSV("Data/detailedStats.csv");
    profiler->calculateStats();
    profiler->printStats();
    profiler->printStatsToCSV("Data/profilerStatsCSV.csv");

     profiler->printStatstoJSON("Data/profilerStatsJSON.json");
    // Open the visualizer in the default browser
     cout << "Starting local server..." << endl;
    startServer();
    
    cout << "Opening visualizers in browser..." << endl;
    openBrowsers();
    
    cout << "Visualizers are running at:" << endl;
    cout << "1. http://localhost:8000/Code/profiler-visualizer.html" << endl;
    cout << "2. http://localhost:8000/Code/detailed-profiler-visualizer.html" << endl;
    cout << "Press Enter to exit and stop the server..." << endl;
    
    cin.get();
    
    // Cleanup
    cleanupServer();


    delete profiler;
    profiler = nullptr;
    return 0;
}