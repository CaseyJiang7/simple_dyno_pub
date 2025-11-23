#pragma once

#include <cstdint>
#include "global_utils.h"

/// @brief Utility class for timing loops with a fixed period.
/// 
/// LoopTimer tracks the previous invocation time and allows blocking
/// until the next desired period has elapsed. Useful for maintaining
/// consistent loop frequencies.
class LoopTimer {
public:
    /// @brief Creates a new LoopTimer and initializes its previous invocation time.
    inline LoopTimer() 
        : previous_time_(rtcNsSinceEpoch()), period_(0) {}

    /// @brief Blocks current thread until 'period' nanoseconds have passed
    /// since the last invocation.
    /// @param period The desired loop period in nanoseconds.
    inline void wait(const uint64_t period) {
        uint64_t current_time;
        do {
            current_time = rtcNsSinceEpoch();
        } while (current_time - previous_time_ < period);
        period_ = current_time - previous_time_;
        previous_time_ = current_time;
    }

    /// @brief Returns the actual period (in nanoseconds) of the last loop.
    /// @return Measured loop period in nanoseconds.
    inline uint64_t period() const { return period_; }

private:
    uint64_t previous_time_; ///< Previous invocation time (ns since Epoch)
    uint64_t period_;        ///< Measured period of the last loop (ns)
};
