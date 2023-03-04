#pragma once
#include<ostream>
#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif
#include "MsTimer.h"

namespace Perf {
  class PerformanceCounter :
    public MsTimer
  {
  public:
    PerformanceCounter();
    ~PerformanceCounter();
    uint64_t start() override;
    uint64_t stop() override;

    uint64_t clock_ticks() {
      return (stop_clock_tick_
        - start_clock_tick_);
    }

    friend std::ostream& operator<<(std::ostream& os, const PerformanceCounter& counter);
  private:
    uint64_t start_clock_tick_{};
    uint64_t stop_clock_tick_{};
  };
}

