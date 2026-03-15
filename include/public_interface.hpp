#ifndef PUBLIC_INTERFACE_HPP
#define PUBLIC_INTERFACE_HPP

#include "../include/internal_structures.hpp"
#include <cstddef> // this header gives us the standard c++ defn for size related typoes and constants in our case for (size_t).
// Defining the functions that my allocator will do or call.

class MemoryAllocator {
public:
  // Constructopr for our memory allocator
  MemoryAllocator(size_t poolSize);

  // Destructor for our memory allocator
  ~MemoryAllocator();

  // To allocate memory
  void *allocate(size_t size);

  // For freeing memory
  void freeMemory(void *ptr);

  // To print the statsics
  void printStats();
  // to print the memroy layout
  void printMemoryLayout();

private:
  void *poolStart; // where our memory pool begins
  size_t poolSize; // How big our memory pool is

  void
  coalesce(BlockHeader *block); // internal helper for coalescing free blocks
};

#endif
