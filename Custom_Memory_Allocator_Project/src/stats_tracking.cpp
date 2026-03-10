#include "../include/stats_tracking.hpp"
#include <iostream>

// initializing all the stats to zero
StatsTracker::StatsTracker() { stats = {0, 0, 0, 0, 0}; }
// making constructors
void StatsTracker::recordAllocation(size_t size) {
  stats.totalAllocations++;
  stats.currentMemoryUsage += size;
  // stats.totalFreeMemory -= size;

  if (stats.currentMemoryUsage > stats.peakMemoryUsage) {
    stats.peakMemoryUsage = stats.currentMemoryUsage;
  }
}

void StatsTracker::recordFree(size_t size) {

  stats.totalFrees++;
  if (size > stats.currentMemoryUsage) {
    stats.currentMemoryUsage = 0;
  } else {
    stats.currentMemoryUsage -= size;
  }
  //  stats.totalFreeMemory += size; // freeing memory adds free sapce
}

void StatsTracker::printStats() {
  std::cout << "Total Allocations: " << stats.totalAllocations << std::endl;
  std::cout << "Total Frees: " << stats.totalFrees << std::endl;
  std::cout << "Current memory usage: " << stats.currentMemoryUsage
            << std::endl;
  std::cout << "Peak memory usage: " << stats.peakMemoryUsage << std::endl;
  // std::cout << "Total free memory: " << stats.totalFreeMemory << std::endl;
}
