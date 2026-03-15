#include "../include/public_interface.hpp"

#include <iostream>

int main() {
  std::cout << "---Creating akllocator with 1 MB pool---\n";
  MemoryAllocator allocator(1024 * 1024);

  std::cout << "\nInitial Layout\n";
  allocator.printMemoryLayout();

  std::cout << "\nAllocating 100 bytes\n";
  void *p1 = allocator.allocate(100);
  allocator.printMemoryLayout();

  std::cout << "\nAllocating 200 bytes\n";
  void *p2 = allocator.allocate(200);
  allocator.printMemoryLayout();

  std::cout << "\nAllocating 300 bytes\n";
  void *p3 = allocator.allocate(300);
  allocator.printMemoryLayout();

  std::cout << "\nFreeing all allocated memory\n";
  allocator.freeMemory(p1);
  allocator.freeMemory(p2);
  allocator.freeMemory(p3);

  std::cout << "\n---Final Layout---\n";
  allocator.printMemoryLayout();
  allocator.printStats();

  return 0;
}
