#include "../include/internal_structures.hpp"
#include "../include/public_interface.hpp"
#include "../include/stats_tracking.hpp"

#include <cstddef>
#include <iostream>
#include <sys/mman.h>

// Definition of alignSize
size_t alignSize(size_t size) {
  // Rounding up to the nearest multiple of ALIGNMENT
  return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);

  // Simpler version
  //  size_t remainder = size % ALIGNMENT;
  // if (remainder == 0) return size;
  // return size + (ALIGNMENT - remainder);
}

// These variables are only visible to this file as they are static
static BlockHeader *freeListHead = nullptr; // pointer to the BlockHeader struct
static StatsTracker stats;

MemoryAllocator::MemoryAllocator(size_t size) {
  poolSize = size;

  // Getting memory from the OS
  poolStart =
      mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1,
           0); // poolStart points to raw memory
  // checking if mmap failed
  if (poolStart == MAP_FAILED) {
    std::cerr << "Allocation failed!!" << std::endl;
    exit(1);
  }

  freeListHead = (BlockHeader *)poolStart;
  freeListHead->size = poolSize - sizeof(BlockHeader);
  freeListHead->isFree = true;
  freeListHead->next = nullptr;
  freeListHead->prev = nullptr;
}

// Allocation logic- Using first-fit algorithm
void *MemoryAllocator::allocate(size_t size) {
  // rounding up size to alignment boundary (from internal_structures.hpp)
  size = alignSize(size);
  // start searching from the beginning of the free list
  BlockHeader *current = freeListHead;

  // first-fit search: To find the first free block thats big enough
  while (current) {
    if (current->isFree && current->size >= size) {
      size_t remaining = current->size - size;
      if (remaining > sizeof(BlockHeader)) {
        // creating new block after allocated portion
        BlockHeader *newBlock =
            (BlockHeader *)((char *)current + sizeof(BlockHeader) + size);
        newBlock->size = remaining - sizeof(BlockHeader);
        newBlock->isFree = true;
        newBlock->next = current->next;
        newBlock->prev = current;
        if (current->next) {
          current->next->prev = newBlock;
        }
        current->next = newBlock;
        current->size = size;
      }
      // Found a suitable block
      current->isFree = false; // Mark as used

      // Record this allocation in stats
      stats.recordAllocation(size);
      // Return pointer to memory after the header
      return (void *)((char *)current + sizeof(BlockHeader));
    }
    // Move to next block
    current = current->next;
  }

  return nullptr; // if no suitable block found (allocation failed)
}

// Free Logic
void MemoryAllocator::freeMemory(void *ptr) {
  // Checking for safety : we can not free nullptr
  if (ptr == nullptr) {
    return;
  }
  // Finding the BlockHeader(it is right before the user's memory)
  BlockHeader *header = (BlockHeader *)((char *)ptr - sizeof(BlockHeader));
  // Mark block as free
  header->isFree = true;
  // recording this free in stats
  stats.recordFree(header->size);
}

// Destructor- Freeing the memory pool
MemoryAllocator::~MemoryAllocator() {
  // return memory to os
  munmap(poolStart, poolSize);
}

// Printing the stats
void MemoryAllocator::printStats() {
  stats.printStats(); // calls StatsTracker function.
}
