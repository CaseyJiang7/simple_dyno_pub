#pragma once

#include "global_utils.h"

class LoopTimer {
public:
  /// @brief Creates a new LoopTimer and initializes its previous invocation
  /// time
  LoopTimer();

  /// @brief Blocks current thread for 'period' nanoseconds, counted from this
  /// LoopTimer's previous invocation time (this->previous_time_)
  /// @param period The number of nanoseconds to block for (period of the loop)
  void wait(const uint64_t period);

  /// @brief Get the recorded period of the last loop of this timer
  /// @return Period of last loop in ns
  uint64_t period() const;

private:
  /// @brief Previous invocation time in nanoseconds since Epoch
  uint64_t previous_time_;

  /// @brief Recorded period of the last loop
  uint64_t period_;
};