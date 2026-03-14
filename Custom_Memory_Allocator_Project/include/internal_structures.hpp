#ifndef INTERNAL_STRUCTURES_HPP
#define INTERNAL_STRUCTURES_HPP

#include <cstddef>

const size_t ALIGNMENT = 8; // all allocations will be multiples 8 bytes
// Rounds up size to the nearest multiple of ALIGNMENT
// Ensures all  memory stats at aligned address
inline size_t alignSize(size_t size) {
  return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}
struct BlockHeader {
  size_t size; // the size of the block(excluding header)
  bool isFree; // Is this block free?

  BlockHeader *next; // Next block in the list
  BlockHeader *prev; // Previos block in the list
};

#endif
