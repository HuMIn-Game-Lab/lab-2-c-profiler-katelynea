#include "profiler.hpp"
#include "time.hpp"
#include <iostream>
#include <fstream>

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
void Profiler:: storeDetailedStatsbySectionCSV(const char* fileName){
    ofstream file;
    file.open(fileName);
    file<< "Section Name, Time"<<endl;
    for(auto const& record : elapsedTimes){
        file<< record.sectionName<<", ";
        file<< record.secondsAtStop<<endl;
    }
    file.close();
}

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
    ofstream file;
    file.open(fileName);
    // make sur file is clear
    // file.clear();
    file<< "Section Name, Count, Total Time, Average Time, Min Time, Max Time, Line Number, File Name, Function Name"<<endl;
    for(auto const& pair : stats){
        ProfilerStats* sectionStats = pair.second;
        file<< sectionStats->sectionName<<", ";
        file<< sectionStats->count<<", ";
        file<< sectionStats->totalTime<<", ";
        file<< sectionStats->averageTime<<", ";
        file<< sectionStats->minTime<<", ";
        file<< sectionStats->maxTime<<", ";
        file<< sectionStats->lineNumber<<", ";
        file<< sectionStats->fileName<<", ";
        file<< sectionStats->functionName<<endl;
    }
    file.close();
}
void Profiler:: printStatsToXML(const char* fileName){
    ofstream file;
    file.open(fileName);
    // file.clear();
    file<< "<ProfilerStats>"<<endl;
    for(auto const& pair : stats){
        ProfilerStats* sectionStats = pair.second;
        file<< "<Section>"<<endl;
        file<< "<SectionName>"<< sectionStats->sectionName<< "</SectionName>"<<endl;
        file<< "<Count>"<< sectionStats->count<< "</Count>"<<endl;
        file<< "<TotalTime>"<< sectionStats->totalTime<< "</TotalTime>"<<endl;
        file<< "<AverageTime>"<< sectionStats->averageTime<< "</AverageTime>"<<endl;
        file<< "<MinTime>"<< sectionStats->minTime<< "</MinTime>"<<endl;
        file<< "<MaxTime>"<< sectionStats->maxTime<< "</MaxTime>"<<endl;
        file<< "<LineNumber>"<< sectionStats->lineNumber<< "</LineNumber>"<<endl;
        file<< "<FileName>"<< sectionStats->fileName<< "</FileName>"<<endl;
        file<< "<FunctionName>"<< sectionStats->functionName<< "</FunctionName>"<<endl;
        file<< "</Section>"<<endl;
    }
    file<< "</ProfilerStats>"<<endl;
    file.close();
}
void Profiler:: printStatstoJSON(const char* fileName){
    ofstream file;
    file.open(fileName);
    // file.clear();
    file<< "{"<<endl;
    file<< "\"ProfilerStats\": ["<<endl;
    for(auto const& pair : stats){
        ProfilerStats* sectionStats = pair.second;
        file<< "{"<<endl;
        file<< "\"SectionName\": \""<< sectionStats->sectionName<< "\", "<<endl;
        file<< "\"Count\": "<< sectionStats->count<< ", "<<endl;
        file<< "\"TotalTime\": "<< sectionStats->totalTime<< ", "<<endl;
        file<< "\"AverageTime\": "<< sectionStats->averageTime<< ", "<<endl;
        file<< "\"MinTime\": "<< sectionStats->minTime<< ", "<<endl;
        file<< "\"MaxTime\": "<< sectionStats->maxTime<< ", "<<endl;
        file<< "\"LineNumber\": "<< sectionStats->lineNumber<< ", "<<endl;
        file<< "\"FileName\": \""<< sectionStats->fileName<< "\", "<<endl;
        file<< "\"FunctionName\": \""<< sectionStats->functionName<< "\" "<<endl;
        file<< "},"<<endl;
    }
    file<< "]}"<<endl;
    file.close();
}
