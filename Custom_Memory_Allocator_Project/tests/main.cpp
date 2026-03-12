#include "../include/public_interface.hpp"

#include <iostream>

int main() {
  MemoryAllocator allocator(1024 * 1024);

  void *p1 = allocator.allocate(100);
  void *p2 = allocator.allocate(200);
  void *p3 = allocator.allocate(300);
  allocator.freeMemory(p1);
  allocator.freeMemory(p2);
  allocator.freeMemory(p3);

  allocator.printStats();
  std::cout << "p1: " << p1 << std::endl;
  std::cout << "p2: " << p2 << std::endl;
  return 0;
}
