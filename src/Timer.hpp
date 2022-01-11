#ifndef TIMER_HPP
#define TIMER_HPP

#include "Global.hpp"

#include <chrono>

class Timer
{
public:
    Timer(const char* message);
    Timer(void);
    Timer(const Timer& other) = delete;
    ~Timer(void);
    void reset(void);
    void processResult(const std::chrono::time_point<std::chrono::steady_clock>& endTimepoint) const;

    template<typename D>
    int64_t elapsed(void) const;

private:
    const char* m_message;
    std::chrono::time_point<std::chrono::steady_clock> m_startTimepoint;
};

#endif
