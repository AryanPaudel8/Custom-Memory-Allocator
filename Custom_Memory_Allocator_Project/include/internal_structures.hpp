#ifndef INTERNAL_STRUCTURES_HPP
#define INTERNAL_STRUCTURES_HPP

#include <cstddef>

const size_t ALIGNMENT = 8; // all allocations will be multiples 8 bytes

struct BlockHeader {
  size_t size; // the size of the block(excluding header)
  bool isFree; // Is this block free?

  BlockHeader *next; // Next block in the list
  BlockHeader *prev; // Previos block in the list
};

size_t alignSize(size_t size); // This function is for rounding up the size to
                               // The nearest multiple of ALIGNMENT

#endif
