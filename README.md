# Custom Memory Allocator (C++)

<div align="center">

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue?style=flat-square&logo=cplusplus)](https://cplusplus.com/)
[![Platform](https://img.shields.io/badge/Platform-Linux%2FUnix-orange?style=flat-square)](https://www.linux.org/)
[![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)](LICENSE)
[![Stars](https://img.shields.io/github/stars/AryanPaudel8/Custom-Memory-Allocator?style=flat-square)](https://github.com/AryanPaudel8/Custom-Memory-Allocator)

A blazing-fast, educational memory allocator that demonstrates how modern operating systems and runtime environments manage dynamic memory allocation.

[Features](#-features) • [Architecture](#-memory-architecture) • [Quick Start](#-quick-start) • [Examples](#-example-usage) • [Contributing](#-contributing)

</div>

---

## 📚 Overview

Ever wondered how `malloc()` and `new` actually work under the hood? This project demystifies dynamic memory allocation by building a complete memory allocator from scratch.

Using **mmap()** to request raw memory directly from the OS, this allocator manages its own heap with:

- 🎯 **First-Fit allocation strategy** for efficient memory assignment
- 🧩 **Block splitting** to minimize wasted space
- 🔗 **Block coalescing** to reduce fragmentation
- 📊 **Runtime statistics tracking** for performance analysis
- 🔍 **Memory layout visualization** for debugging

This is exactly how production allocators like **jemalloc**, **tcmalloc**, and **ptmalloc** work!

---

## ✨ Features

### 🎯 Smart Allocation
- **First-Fit Strategy** - Quickly finds suitable memory blocks
- **Block Splitting** - Splits large blocks to minimize waste
- **8-Byte Alignment** - Ensures optimal CPU memory alignment
- **Double-Free Detection** - Prevents common memory errors

### 🛡️ Fragmentation Control
- **Automatic Coalescing** - Merges adjacent free blocks
- **Before & After Tracking** - Shows fragmentation reduction
- **Contiguous Memory Reconstruction** - Optimizes memory usage

### 📈 Performance Monitoring
- **Total Allocations/Deallocations** - Track operation counts
- **Peak Memory Usage** - Identify memory bottlenecks
- **Current Memory Usage** - Monitor real-time allocation
- **Detailed Statistics** - Analyze allocator behavior

### 🔧 Developer Tools
- **Memory Layout Visualization** - See the exact memory structure
- **Block Metadata Display** - Inspect allocation details
- **Address Information** - Track header and data pointers
- **Linked List Traversal** - Understand memory organization

---

## 🏗️ Memory Architecture

### Block Structure
```
┌─────────────────────────────┐
│     BlockHeader             │
├─────────────────────────────┤
│ • size         (8 bytes)    │
│ • isFree       (1 byte)     │
│ • next pointer (8 bytes)    │
│ • prev pointer (8 bytes)    │
├─────────────────────────────┤
│     User Data               │
│ (requested allocation)      │
├─────────────────────────────┤
│     Free Block Metadata     │
│ (managed by allocator)      │
└─────────────────────────────┘
```

### Memory Pool Layout
```
mmap() Request → +─────────────────────────────────────────────────+
                 │ [H|Data] [H|Data] [H|Data] [H|Data] [Free...]   │
                 +─────────────────────────────────────────────────+
                 
Where: H = Header, 8-byte aligned blocks
```

### Alignment Strategy
```cpp
size = (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
```

This ensures:
- ��� Proper CPU memory alignment
- ✅ Improved performance
- ✅ Avoidance of undefined behavior on strict architectures

---

## 🔄 Allocation Algorithm

### First-Fit Strategy

```
START: Need 256 bytes

┌─────────────────────────────────────────┐
│ [100 USED] [150 FREE] [300 FREE]       │ ← Scan free list
└─────────────────────────────────────────┘
           ↓
    Found 150 FREE (too small)
           ↓
    Continue scanning
           ↓
    Found 300 FREE (large enough!)
           ↓
    SPLIT: [256 USED] [44 FREE]
           ↓
    Return pointer to user data
```

**Steps:**
1. 🔍 Traverse the block list
2. 🎯 Find the first free block large enough
3. ✂️ Split if block is larger than needed
4. ✅ Mark block as allocated
5. 🎁 Return pointer to data region

**Time Complexity: O(n)** - depends on number of blocks

---

## 🧩 Fragmentation Handling

### Problem: External Fragmentation
```
Before Free:
[████ USED ████][Free][████ USED ████][Free]

After Free (without coalescing):
[Free][Free][████ USED ████][Free] ← Scattered free blocks!
```

### Solution: Block Coalescing
```
After Free + Coalesce:
[█████████ LARGE FREE █████████][Free]
      ↑
  Merged blocks = better future allocations!
```

**Coalescing Strategy:**
- Merge with **next** block if free
- Merge with **previous** block if free
- Reconstruct larger contiguous blocks

---

## 📊 Time Complexity Analysis

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| **Allocate** | O(n) | First-Fit requires scanning free list |
| **Free** | O(1) | Header directly precedes memory |
| **Coalesce** | O(1) | Only checks adjacent blocks |
| **Traversal** | O(n) | Visits all blocks |

**Why so fast?** The block header is stored right before allocated memory, so freeing is just marking and checking neighbors!

---

## 📁 Project Structure

```
Custom-Memory-Allocator/
│
├── 📄 include/
│   ├── internal_structures.hpp    # Block metadata structures
│   ├── public_interface.hpp       # Main allocator API
│   └── stats_tracking.hpp         # Statistics tracker
│
├── 📝 src/
│   ├── internal_implementation.cpp  # Core allocation logic
│   └── stats_tracking.cpp           # Statistics implementation
│
├── ✅ tests/
│   └── main.cpp                     # Usage examples
│
├── 🔨 CMakeLists.txt               # Build configuration
└── 📖 README.md                    # This file
```

---

## 🚀 Quick Start

### Prerequisites
```bash
✓ Linux / Unix environment
✓ GCC 9.0+ or Clang 10.0+
✓ C++17 or newer
✓ CMake 3.10+ (optional, for build)
```

### Compile from Source

**Using g++:**
```bash
g++ -std=c++17 \
    src/internal_implementation.cpp \
    src/stats_tracking.cpp \
    tests/main.cpp \
    -o allocator
```

**Using CMake:**
```bash
mkdir build && cd build
cmake ..
make
```

### Run
```bash
./allocator
```

---

## 💻 Example Usage

### Basic Allocation

```cpp
#include "include/public_interface.hpp"

int main() {
    // Step 1: Create allocator with 1MB pool
    MemoryAllocator allocator(1024 * 1024);
    
    // Step 2: Allocate memory
    void* ptr1 = allocator.allocate(100);   // 100 bytes
    void* ptr2 = allocator.allocate(200);   // 200 bytes
    void* ptr3 = allocator.allocate(300);   // 300 bytes
    
    // Step 3: Use the memory
    std::memset(ptr1, 'A', 100);
    std::memset(ptr2, 'B', 200);
    std::memset(ptr3, 'C', 300);
    
    // Step 4: View memory layout
    std::cout << "\n=== Memory Layout ===" << std::endl;
    allocator.printMemoryLayout();
    
    // Step 5: View statistics
    std::cout << "\n=== Statistics ===" << std::endl;
    allocator.printStats();
    
    // Step 6: Free memory
    allocator.freeMemory(ptr1);
    allocator.freeMemory(ptr2);
    allocator.freeMemory(ptr3);
    
    return 0;
}
```

### Expected Output

```
===== Creating allocator with 1 MB pool =====

✓ Allocating 100 bytes
✓ Allocating 200 bytes
✓ Allocating 300 bytes

===== Memory Layout =====
[USED: 100B] → [USED: 200B] → [USED: 300B] → [FREE: 1048076B]

===== Statistics =====
Total Allocations: 3
Total Frees: 0
Current Memory Usage: 600 bytes
Peak Memory Usage: 600 bytes

===== Freeing all memory =====

===== Final Layout =====
[FREE: 1048576B]

Total Allocations: 3
Total Frees: 3
Current Memory Usage: 0 bytes
Peak Memory Usage: 600 bytes
```

---

## 🎓 Key Concepts Demonstrated

This project teaches fundamental systems programming principles:

- 🧠 **Heap Memory Management** - How allocators organize memory
- 🔗 **Linked Lists** - Dynamic data structure implementation
- 🎯 **First-Fit Algorithm** - Greedy allocation strategy
- ⚙️ **Memory Alignment** - Hardware performance optimization
- 🔄 **Block Splitting** - Minimizing memory waste
- 🧩 **Block Coalescing** - Fragmentation reduction
- 📍 **Metadata Overhead** - Tracking allocation information
- 🎚️ **mmap() System Call** - OS-level memory requests
- 📊 **Performance Profiling** - Statistics tracking
- 🐛 **Debugging Tools** - Memory visualization

---

## 🔮 Future Enhancements

### Advanced Allocation Strategies
- [ ] **Best-Fit** - Find smallest suitable block
- [ ] **Worst-Fit** - Use largest available block
- [ ] **Next-Fit** - Continue from last allocation

### Concurrency
- [ ] **Thread Safety** - Mutex protection for concurrent access
- [ ] **Lock-Free Structures** - Atomic operations without locks

### Advanced Techniques
- [ ] **Segregated Free Lists** - Separate lists by size
- [ ] **Slab Allocator** - Efficient small object allocation
- [ ] **Reallocation Support** - `reallocate(ptr, newSize)`
- [ ] **Memory Leak Detection** - Track unfreed allocations

### Performance
- [ ] **Benchmarking Suite** - Compare with system allocators
- [ ] **Memory Defragmentation** - Compact fragmented memory
- [ ] **Adaptive Strategies** - Adjust algorithm based on patterns

---

## 📚 Why This Project Matters

Understanding memory allocators is crucial for:

- **Operating Systems** - Core kernel functionality
- **Language Runtimes** - Java GC, Python, Node.js memory
- **Game Engines** - High-performance allocation in Unreal, Unity
- **Databases** - Custom allocators in PostgreSQL, MongoDB
- **High-Performance Systems** - Finance, AI, real-time applications

Modern allocators like **jemalloc**, **tcmalloc**, and **ptmalloc** build on these core principles:

```
This Project          →  Production Allocators
├─ Block Metadata    →  ├─ jemalloc (per-thread arenas)
├─ Free Lists        →  ├─ tcmalloc (size classes)
├─ Coalescing        →  ├─ ptmalloc (heap consolidation)
└─ Fragmentation     →  └─ All: Advanced fragmentation strategies
```

---

## 🎯 Educational Value

This project provides strong foundations for studying:

- **Operating Systems** (OS 201+)
- **Compiler Design** - Memory management in compiled languages
- **High-Performance Computing** - Optimization techniques
- **Language Runtimes** - How interpreters manage memory
- **Systems Programming** - Low-level C++ and syscalls

**What You'll Learn:**
- ✅ How `malloc()` works internally
- ✅ Memory fragmentation challenges
- ✅ Low-level memory management
- ✅ Systems programming design patterns
- ✅ Performance profiling techniques

---

## 👨‍💻 Author

**Aryan Paudel**
- 🎓 Computer Science Student
- 💻 Systems Programming Enthusiast
- 📧 [GitHub Profile](https://github.com/AryanPaudel8)

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

```
MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
```

---

## 🤝 Contributing

Contributions are welcome! Feel free to:

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/amazing-feature`)
3. **Commit** your changes (`git commit -m 'Add amazing feature'`)
4. **Push** to the branch (`git push origin feature/amazing-feature`)
5. **Open** a Pull Request

---

## 📖 Resources & References

### Learning Materials
- [Malloc Tutorial](http://www.malloc.org/) - Understanding malloc
- [Jemalloc Documentation](http://jemalloc.net/) - Production allocator
- [Memory Allocators](https://www.gnu.org/software/libc/manual/html_node/The-GNU-Allocator.html) - GNU libc details

### Related Projects
- [ptmalloc3](http://www.malloc.org/malloc.c) - GNU malloc variant
- [tcmalloc](https://github.com/google/tcmalloc) - Google's allocator
- [Hoard](http://www.hoard.org/) - Scalable memory allocator

---

<div align="center">

### ⭐ If this project helped you, please consider giving it a star!

**Happy Allocating! 🚀**

</div>
