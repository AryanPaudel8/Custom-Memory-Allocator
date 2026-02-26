#include <cstddef>
#include <iostream>
#include <sys/mman.h>

// BLOCK METADATA STRUCTURE
struct block {
  size_t size;
};

class Allocator {
  void *pool_start();
  size_t total_size;
};

int main() {
  // requesting 1 mb of memory
  size_t size = 1024 * 1024; // size_t :unsigned integer variable

  void *pool_start = mmap(NULL, size, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (pool_start == MAP_FAILED) {
    std::cerr << "Allocation failed!!" << std::endl;
    return 1;
  }

  std::cout << pool_start << std::endl;
  return 0;
}
