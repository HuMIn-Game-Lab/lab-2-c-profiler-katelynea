#include "profiler.hpp"
#include "time.hpp"
#include <iostream>

Profiler* Profiler::gProfiler = nullptr;

TimeRecordStart::TimeRecordStart(char const* sectionName, double secondsAtStart): sectionName(sectionName), secondsAtStart(secondsAtStart){};
TimeRecordStart::~TimeRecordStart(){};

TimeRecordStop:: TimeRecordStop(const char* sectionName, double secondsAtStop, int lineNumber, const char* fileName, const char* functionName)
        : sectionName(sectionName), secondsAtStop(secondsAtStop), lineNumber(lineNumber), fileName(fileName), functionName(functionName) {}

// Constructor with default values for line number and file/function names
TimeRecordStop:: TimeRecordStop(const char* sectionName, double secondsAtStop)
        : TimeRecordStop(sectionName, secondsAtStop, 0, nullptr, nullptr) {} // Use nullptr for default values

TimeRecordStop:: ~TimeRecordStop(){};

ProfilerScopeObject::ProfilerScopeObject(char const* sectionName){
    Profiler::getInstance()->EnterSection(sectionName);
}
ProfilerScopeObject::~ProfilerScopeObject(){
    Profiler::getInstance()->ExitSection(sectionName);
}
Profiler::Profiler(){
    gProfiler = this;
    startTimes.reserve(100);
    elapsedTimes.reserve(1000000);
}

Profiler* Profiler::getInstance(){
    if(gProfiler == nullptr){
        gProfiler = new Profiler();
    }
    return gProfiler;
}

Profiler::~Profiler(){
    for(auto& pair: stats){
        delete pair.second;
    }
}

void Profiler::EnterSection(char const* sectionName) {
    double secondsAtStart = getTimeInSeconds();
    TimeRecordStart start = TimeRecordStart(sectionName, secondsAtStart);
    startTimes.push_back(start);
}
void Profiler::EnterSection(char const* sectionName) {
    double secondsAtStart = getTimeInSeconds();
    TimeRecordStart start = TimeRecordStart(sectionName, secondsAtStart);
    startTimes.push_back(start);
}

//memory management is slow, so we do pre allocation in Profiler()
// void Profiler :: EnterSection(char const* sectionName){
//     double secondsAtStart = GetCurrentTimeSeconds();
// emplace_back is optmizied function
//      startTimes.emplace_back(sectionName, secondsAtStart);

// }

void Profiler::ExitSection(char const* sectionName){
    double secondsAtStop = getTimeInSeconds();
    const TimeRecordStart& currentSection = startTimes.back();
    double elapsedTime = secondsAtStop - currentSection.secondsAtStart;
    elapsedTimes.emplace_back(sectionName, elapsedTime);
    startTimes.pop_back();
    
}

void Profiler:: ExitSection(char const* sectionName, int lineNumber, const char* fileName, const char* functionName){
    double secondsAtStop = getTimeInSeconds();
    const TimeRecordStart& currentSection = startTimes.back();
    double elapsedTime = secondsAtStop - currentSection.secondsAtStart;
    ReportSectionTime(sectionName, elapsedTime, lineNumber, fileName, functionName);
    startTimes.pop_back();
}

void Profiler:: ReportSectionTime(char const* sectionName, double elapsedTime){
    elapsedTimes.emplace_back(sectionName, elapsedTime);
}

void Profiler:: ReportSectionTime(char const* sectionName, double elapsedTime, int lineNumber, const char* fileName, const char* functionName){
    elapsedTimes.emplace_back(sectionName, elapsedTime, lineNumber, fileName, functionName);
}
ProfilerStats::ProfilerStats(char const* sectionName): sectionName(sectionName), count(0), totalTime(0), averageTime(0), minTime(0), maxTime(0), lineNumber(0), fileName("null"), functionName("null"){};
ProfilerStats::~ProfilerStats(){};

void Profiler:: calculateStats(){
cout << "Elapsed Times:" << endl;
    for (auto const& record : elapsedTimes){
        // calculate stats for each section
        if(stats.find(record.sectionName) == stats.end()){
            stats[record.sectionName] = new ProfilerStats(record.sectionName);
        }
        ProfilerStats* sectionStats = stats[record.sectionName];
        sectionStats->count++;
        sectionStats->totalTime += record.secondsAtStop;
        sectionStats->averageTime = sectionStats->totalTime / sectionStats->count;
        if(sectionStats->minTime == 0 || record.secondsAtStop < sectionStats->minTime){
            sectionStats->minTime = record.secondsAtStop;
        }
        if(sectionStats->maxTime == 0 || record.secondsAtStop > sectionStats->maxTime){
            sectionStats->maxTime = record.secondsAtStop;
        }
        sectionStats->lineNumber = record.lineNumber;
        sectionStats->fileName = record.fileName;
        sectionStats->functionName = record.functionName;

    }

}
void Profiler:: printStats(){
    cout<< "Profiler Stats"<<endl;
    cout<< "--------------------------------"<<endl;
    for(auto const& pair : stats){
        ProfilerStats* sectionStats = pair.second;
        cout<< "Section Name: "<< sectionStats->sectionName<<endl;
        cout<< "Count: "<< sectionStats->count<<endl;
        cout<< "Total Time: "<< sectionStats->totalTime<<endl;
        cout<< "Average Time: "<< sectionStats->averageTime<<endl;
        cout<< "Min Time: "<< sectionStats->minTime<<endl;
        cout<< "Max Time: "<< sectionStats->maxTime<<endl;
        cout<< "Line Number: "<< sectionStats->lineNumber<<endl;
        cout<< "File Name: "<< sectionStats->fileName<<endl;
        cout<< "Function Name: "<< sectionStats->functionName<<endl;
        cout<< "--------------------------------"<<endl;
    }
}
void Profiler::printStatsToCSV(const char* fileName){
}