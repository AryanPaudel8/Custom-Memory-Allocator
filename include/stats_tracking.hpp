#ifndef STATS_TRACKING_HPP
#define STATS_TRACKING_HPP

#include <cstddef>

struct AllocatorStats {
  size_t totalAllocations; // total number of times allocate() is called
  size_t totalFrees;       // total number of times free() is called

  size_t currentMemoryUsage; // how much memory is in use right now
  size_t peakMemoryUsage;    // The highest memory usage ever

  size_t totalFreeMemory; // how much free memory is available
};

class StatsTracker {
public:
  StatsTracker(); // constructor- starts tracking from 0

  void recordAllocation(size_t size); // call this when someone allocates
  void recordFree(size_t size);       // call this when someone frees

  void printStats(); // print the current stats

private:
  AllocatorStats stats; // creating a variable from the type AllocatorStats we
                        // made using struct
};

#endif
