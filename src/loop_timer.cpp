#include "loop_timer.h"

LoopTimer::LoopTimer() : previous_time_(rtcNsSinceEpoch()) {}

void LoopTimer::wait(const uint64_t period) {
  uint64_t current_time;
  do {
    current_time = rtcNsSinceEpoch();
  } while (current_time - previous_time_ < period);
  period_ = current_time - previous_time_;
  previous_time_ = current_time;
}

uint64_t LoopTimer::period() const { return period_; }