#include "Tools.h"

#include <cmath>
#include <cstdlib> // rand
#include <chrono>
#include <thread>


namespace SI {
    namespace Timetools {

        Timestep::Timestep(double seconds)
            : m_timestep(seconds)
        {
            //
        }

        void Timestep::operator+=(double seconds)     { m_timestep += seconds; }
        void Timestep::operator-=(double seconds)     { m_timestep -= seconds; }
        void Timestep::operator*=(double coefficient) { m_timestep *= coefficient; }
        Timestep::operator double()            const  { return m_timestep; }
        double Timestep::GetSeconds(void)      const  { return m_timestep; }
        double Timestep::GetMilliseconds(void) const  { return 1000.0 * m_timestep; }

        TimeEstimate::TimeEstimate(Timestep estimate, double mean)
            : m_estimate(estimate)
            , m_mean(mean)
            , m_m2(0.0)
            , m_count(1)
        {
            //
        }

        void TimeEstimate::AddTimestep(Timestep ts)
        {
            ++m_count;
            double delta = ts - m_mean;
            m_mean      += delta / m_count;
            m_m2        += delta * (ts - m_mean);
            m_estimate   = m_mean + std::sqrt(m_m2 / (m_count - 1)); // mean + stddev
        }

        double TimeEstimate::GetEstimate(void) const { return m_estimate; }
        size_t TimeEstimate::GetCount(void)    const { return m_count; }

    } // end namespace Timetools

    namespace thread {

        void PreciseSleep(SI::Timetools::Timestep seconds, SI::Timetools::TimeEstimate & estimate)
        {
            using namespace std::chrono;
            constexpr double durationTimeToSecs = 1.0 * steady_clock::period::num / steady_clock::period::den;

            while (seconds > estimate.GetEstimate())
            {
                auto startTime = steady_clock::now();
                std::this_thread::sleep_for(milliseconds(1));
                auto endTime = steady_clock::now();
                double duration = durationTimeToSecs * (endTime - startTime).count();
                seconds -= duration;
                estimate.AddTimestep(duration);
            }

            auto startTime = steady_clock::now();
            while (durationTimeToSecs * (steady_clock::now() - startTime).count() < seconds);
        }

    } // end namespace thread

    namespace math {

        int32_t RandomBinary(void)     { return rand() % 2; }
        double  RandomNormalized(void) { return static_cast<double>(rand()) / RAND_MAX; }

    } // end namespace math

} // end namespace SI
