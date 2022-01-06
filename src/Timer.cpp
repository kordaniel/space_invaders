#include "Timer.hpp"
#include "Io.hpp"

#include <ratio>

Timer::Timer(const char* message)
    : m_message(message)
    , m_startTimepoint(std::chrono::steady_clock::now())
{
    //
}

Timer::Timer()
    : Timer(nullptr)
{
    //
}

Timer::~Timer(void)
{
    std::chrono::time_point<std::chrono::steady_clock> endTimepoint
        = std::chrono::steady_clock::now();
    processResult(endTimepoint);
}

void Timer::reset(void)
{
    m_startTimepoint = std::chrono::steady_clock::now();
}

void Timer::processResult(const std::chrono::time_point<std::chrono::steady_clock>& endTimepoint) const
{
    using namespace std::literals::chrono_literals;
    using std::chrono::duration;
    duration dur = endTimepoint - m_startTimepoint;
    const char* message = m_message ? m_message : "";

    if (dur < 1us)
        io::print_to_stdout_varargs(message, duration<double, std::nano>(dur).count(), "ns.");
    else if (dur < 1ms)
        io::print_to_stdout_varargs(message, duration<double, std::micro>(dur).count(), "us.");
    else if (dur < 1s)
        io::print_to_stdout_varargs(message, duration<double, std::milli>(dur).count(), "ms.");
    else
        io::print_to_stdout_varargs(message, duration<double, std::ratio<1>>(dur).count(), "s.");
}

template<typename D>
int64_t Timer::elapsed(void) const
{
    return std::chrono::duration_cast<D>(std::chrono::steady_clock::now() - m_startTimepoint).count();
}
template int64_t Timer::elapsed<std::chrono::nanoseconds> (void) const;
template int64_t Timer::elapsed<std::chrono::microseconds>(void) const;
template int64_t Timer::elapsed<std::chrono::milliseconds>(void) const;
template int64_t Timer::elapsed<std::chrono::seconds>     (void) const;
