#ifndef TIMER_H
#define TIMER_H

#include "global.h"
#include "Io.h"

#include <chrono>

class Timer
{
public:
    Timer(bool printTimingResults = true);
    Timer(const Timer& other) = delete;
    ~Timer(void);
    void processResult(const std::chrono::time_point<std::chrono::steady_clock>& endTimepoint, const char* message);

private:
    bool m_printTimingRes;
    std::chrono::time_point<std::chrono::steady_clock> m_startTimepoint;    
};


class ScopeTimer
{
public:
    ScopeTimer(const char* scopeName);
    ScopeTimer(const ScopeTimer& other) = delete;
    ~ScopeTimer(void);
private:
    const char* m_scopeName;
    Timer m_Timer; // NOTE: This must always be initialized as the last member for maximum accuracy!!
};

#endif
