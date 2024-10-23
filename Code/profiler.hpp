#pragma once
// pragma once is a preprocessor directive that tells the compiler to include the file only once
#include <cfloat>
#include <vector>
#include <iostream>
#include <map>
// use singleton design pattern
#define PROFILER_ENTER(sectionName) Profiler::getInstance()->EnterSection(sectionName);
#define PROFILER_EXIT(sectionName) Profiler::getInstance()->ExitSection(sectionName, __LINE__, __FILE__, __FUNCTION__);


using namespace std;

class ProfilerScopeObject{
    public:
        ProfilerScopeObject(char const* sectionName);
        ~ProfilerScopeObject();
        char const* sectionName;
};
class TimeRecordStart{
    public:
        TimeRecordStart(char const* sectionName, double secondsAtStart);
        ~TimeRecordStart();

        char const* sectionName;
        double secondsAtStart;


};

class TimeRecordStop{
    public:
        TimeRecordStop(char const* sectionName, double secondsAtStop);
        TimeRecordStop(char const* sectionName, double secondsAtStop, int lineNumber, const char* fileName, const char* functionName);
        ~TimeRecordStop();

        char const* sectionName;
        double secondsAtStop;
        int lineNumber;
        const char* fileName;
        const char* functionName;
};
class ProfilerStats{
    public:
        ProfilerStats(char const* sectionName);
        ~ProfilerStats();

        char const* sectionName;
        int count;
        double totalTime;
        double averageTime;
        double minTime;
        double maxTime;

        int lineNumber;
        char const* fileName;
        char const* functionName;

};

class Profiler{
    public:
        // Profiler();
        ~Profiler();

        void EnterSection(char const* sectionName);
        void ExitSection(char const* sectionName);
        void ExitSection(char const* sectionName, int lineNumber, const char* fileName, const char* functionName);
        void storeDetailedStatsbySectionCSV(const char* fileName);
        void calculateStats();
        void printStats();
        void printStatsToCSV(const char* fileName);
        void printStatsToXML(const char* fileName);
        void printStatstoJSON(const char* fileName);

        static Profiler* gProfiler;
        static Profiler *getInstance();
        char const* sectionName;

        private:
            Profiler();
            
            void ReportSectionTime(char const* sectionName, double elapsedTime);
            void ReportSectionTime(char const* sectionName, double elapsedTime, int lineNumber, const char* fileName, const char* functionName);
            map<char const*, ProfilerStats*> stats;
            vector<TimeRecordStart> startTimes;
            vector<TimeRecordStop> elapsedTimes;
};
