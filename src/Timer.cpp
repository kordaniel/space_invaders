#include "Timer.h"

using namespace std::chrono;

Timer::Timer(bool printTimingResults)
    : m_printTimingRes(printTimingResults)
{
    m_startTimepoint = steady_clock::now();
}

Timer::~Timer(void)
{
    if (m_printTimingRes) {
        time_point<steady_clock> endTimepoint = steady_clock::now();
        processResult(endTimepoint, nullptr);
    }
}

void Timer::processResult(const time_point<steady_clock>& endTimepoint, const char* message)
{
    int64_t startTime =
        time_point_cast<microseconds>(m_startTimepoint).time_since_epoch().count();
    int64_t endTime =
        time_point_cast<microseconds>(endTimepoint).time_since_epoch().count();

    int64_t duration = endTime - startTime;
    double duration_ms = duration * 0.001;

    if (duration_ms < 1000)
        io::print_to_stdout_varargs((message != nullptr ? message : ""), duration, " us\t(", duration_ms, " ms)");
    else
       io::print_to_stdout_varargs((message != nullptr ? message : ""), duration, " us\t(", duration * 0.000001, " s)");
}


ScopeTimer::ScopeTimer(const char* scopeName)
    : m_scopeName(scopeName)
    , m_Timer(false)
{
    //
}

ScopeTimer::~ScopeTimer(void)
{
    time_point<steady_clock> endTimepoint = steady_clock::now();
    m_Timer.processResult(endTimepoint, m_scopeName);
}
