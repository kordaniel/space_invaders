#ifndef TOOLS_H
#define TOOLS_H

#include "Global.hpp"


namespace SI {
    namespace Timetools {

        /// A convenient wrapper for one single double to represent a duation of time in seconds.
        class Timestep {
        public:
            Timestep(double seconds = 0.0);
            void operator+=(double seconds);
            void operator-=(double seconds);
            void operator*=(double coefficient);
            operator double()            const;
            double GetSeconds(void)      const;
            double GetMilliseconds(void) const;

        private:
            double m_timestep;
        };

        /// Computes a estimated duration, based on the historical durations
        /// passed in to the method GetEstimate. This is achieved by implementing
        /// Welford's online algorithm to compute the variance ovar all durations
        /// in a single pass.
        /// https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford%27s_online_algorithm
        class TimeEstimate {
        public:
            TimeEstimate(Timestep estimate, double mean);

            void AddTimestep(Timestep ts);
            double GetEstimate(void) const;
            size_t GetCount(void)    const;

        private:
            double m_estimate;
            double m_mean;
            double m_m2;
            size_t m_count;
        };

    } // end namespace Timetools

    namespace thread {

        /// https://blat-blatnik.github.io/computerBear/making-accurate-sleep-function/
        /// Sleeps in 1ms intervals while the remaining time is less than the estimated (seconds)
        /// time, counting for overhead. Spin-locks for the remaining time. Achieves very good
        /// accuracy with a cpu-usage overhead of about 3-5%.
        void PreciseSleep(SI::Timetools::Timestep seconds, SI::Timetools::TimeEstimate & estimate);

    } // end namespace thread

    namespace math {

        // Returns a random 0 or 1
        int32_t RandomBinary(void);

        // Returns a random double between 0 and 1
        double RandomNormalized(void);

    } // end namespace math

} // end namespace SI


#endif
