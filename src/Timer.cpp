#include "Timer.hpp"
#include "Io.hpp"

using namespace std::chrono;

Timer::Timer(const char* message)
    : m_message(message)
    , m_startTimepoint(steady_clock::now())
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
    time_point<steady_clock> endTimepoint = steady_clock::now();
    processResult(endTimepoint);
}

void Timer::reset(void)
{
    m_startTimepoint = steady_clock::now();
}

void Timer::processResult(const time_point<steady_clock>& endTimepoint) const
{
    int64_t duration = duration_cast<nanoseconds>(endTimepoint - m_startTimepoint).count();
    const char* message = m_message ? m_message : "";

    if (duration < 1000) {
        io::print_to_stdout_varargs(message, duration, "ns.");
    } else if (duration < 1e6) {
        io::print_to_stdout_varargs(message, duration * 0.001, "us.");
    } else if (duration < 1e9) {
        io::print_to_stdout_varargs(message, duration * 0.000001, "ms.");
    } else {
        io::print_to_stdout_varargs(message, duration * 0.000000001, "s.");
    }
}

template<typename D>
int64_t Timer::elapsed(void) const
{
    return duration_cast<D>(steady_clock::now() - m_startTimepoint).count();
}
template int64_t Timer::elapsed<nanoseconds> (void) const;
template int64_t Timer::elapsed<microseconds>(void) const;
template int64_t Timer::elapsed<milliseconds>(void) const;
template int64_t Timer::elapsed<seconds>     (void) const;
