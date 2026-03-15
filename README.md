# Custom Memory Allocator (C++)

A custom memory allocator implemented in modern C++ that simulates how dynamic memory management works internally in operating systems and runtime environments.

This project builds a manual heap manager using mmap() to obtain raw memory from the OS and manages allocations through a linked-list based free block system using the First-Fit allocation algorithm with block splitting and coalescing to reduce fragmentation.

The allocator also includes memory usage statistics tracking and memory layout visualization tools, making it useful both as a learning tool and a systems programming demonstration.

## Table of Contents

## Overview

## Features

## Memory Architecture

## Allocation Algorithm

## Fragmentation Handling

## Statistics Tracking

## Time Complexity

## Project Structure

## How It Works

## Example Execution Flow

## Build Instructions

## Example Output

## Key Concepts Demonstrated

## Future Improvements

## Why This Project Matters

## Educational Value

## Author

## License

Overview

Dynamic memory allocation in C/C++ (malloc, free, new, delete) is handled by sophisticated allocators implemented in system libraries.

This project recreates a simplified but realistic allocator that:

Requests memory directly from the OS

Manages its own memory pool

Tracks free and used blocks

Handles fragmentation

Tracks runtime usage statistics

Instead of relying on the system allocator, this project demonstrates how memory managers actually work internally.

Features
### Custom Memory Pool

Uses mmap() to request a contiguous block of memory directly from the operating system.

### First-Fit Allocation Strategy

The allocator scans memory blocks sequentially and allocates the first block large enough to satisfy the request.

### Block Splitting

If a free block is larger than required, it is split into:

an allocated block

a remaining free block

This minimizes wasted memory.

### Block Coalescing

Adjacent free blocks are automatically merged when memory is freed to reduce fragmentation.

Alignment Support

All allocations are aligned to 8 bytes for proper memory access and performance.

Double-Free Detection

The allocator detects and prevents double-free errors.

Memory Usage Statistics

Tracks:

total allocations

total frees

current memory usage

peak memory usage

Memory Layout Visualization

A debugging tool prints:

block sizes

block status (free / used)

header addresses

data addresses

linked list connections

## Memory Architecture

Each block in the memory pool consists of a metadata header followed by user memory.

+-----------------------+
| BlockHeader           |
|-----------------------|
| size                  |
| isFree                |
| next                  |
| prev                  |
+-----------------------+
| User Data             |
| (allocated memory)    |
+-----------------------+
Memory Pool Layout

Memory Pool (mmap)

+--------------------------------------------------------------+
| Header | User Data | Header | User Data | Header | User Data |
+--------------------------------------------------------------+
BlockHeader Structure

Each block stores metadata:

block size

allocation status

pointer to next block

pointer to previous block

This structure allows the allocator to perform:

traversal

block splitting

block merging (coalescing)

Alignment Strategy

All allocations are aligned to 8 bytes.

size = (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);

This ensures:

proper CPU memory alignment

improved performance

avoidance of undefined behavior on some architectures

## Allocation Algorithm

The allocator uses the First-Fit strategy.

Steps

Traverse the block list

Find the first free block large enough

If the block is larger than required:

split the block

Mark the block as allocated

Return pointer to the data region

Simplified process:

search free list
      ↓
found block
      ↓
split if large
      ↓
mark as used
      ↓
return pointer
## Fragmentation Handling

Memory fragmentation occurs when small free blocks are scattered across memory.

This allocator handles fragmentation using coalescing.

Coalescing Strategy

When memory is freed:

merge with next block if it is free

merge with previous block if it is free

This reconstructs larger contiguous blocks.

Example:

Before Free:

[Used][Free][Used][Free]

After Free + Coalesce:

[Free-----------][Free]
## Statistics Tracking

The allocator records runtime statistics using a dedicated StatsTracker class.

Tracked metrics:

Metric	Description
Total Allocations	Number of allocation calls
Total Frees	Number of free calls
Current Memory Usage	Memory currently allocated
Peak Memory Usage	Highest memory usage reached

These metrics help analyze allocator behavior during program execution.

## Time Complexity
Operation	Complexity
allocate	O(n)
free	O(1)
coalesce	O(1)
memory traversal	O(n)

Explanation:

Allocation requires scanning the block list (First-Fit search).

Freeing is constant time since the block header is located directly before the memory pointer.

Coalescing only checks adjacent blocks, making it efficient.

## Project Structure
Custom-Memory-Allocator/
│
├── include/
│   ├── internal_structures.hpp
│   ├── public_interface.hpp
│   └── stats_tracking.hpp
│
├── src/
│   ├── internal_implementation.cpp
│   └── stats_tracking.cpp
│
├── tests/
│   └── main.cpp
│
└── README.md
## How It Works
### Step 1 — Create Allocator
MemoryAllocator allocator(1024 * 1024);

Creates a 1MB memory pool using mmap().

### Step 2 — Allocate Memory
void* p = allocator.allocate(100);

Process:

Align size

Find suitable free block

Split if necessary

Return pointer to data region

### Step 3 — Free Memory
allocator.freeMemory(p);

Process:

Locate block header

Mark block free

Merge adjacent free blocks

Step 4 — View Memory Layout
allocator.printMemoryLayout();

Shows block structure and memory usage.

Step 5 — View Statistics
allocator.printStats();

Displays allocator usage metrics.

## Example Execution Flow

Example test scenario:

Create allocator (1MB pool)

Allocate memory:

100 bytes

200 bytes

300 bytes

Free all blocks

Print memory layout

Print allocator statistics

Output includes:

block list visualization

allocation statistics

fragmentation information

## Build Instructions
Requirements

Linux / Unix environment

GCC or Clang

C++17 or newer

Compile
g++ -std=c++17 src/internal_implementation.cpp \
src/stats_tracking.cpp \
tests/main.cpp \
-o allocator
Run
./allocator
## Example Output
---Creating allocator with 1 MB pool---

Allocating 100 bytes
Allocating 200 bytes
Allocating 300 bytes

Freeing all allocated memory

---Final Layout---

Total Allocations: 3
Total Frees: 3
Current memory usage: 0
Peak memory usage: 600
## Key Concepts Demonstrated

This project demonstrates core systems programming concepts:

custom heap management

memory alignment

linked list memory blocks

fragmentation control

block splitting

block coalescing

OS-level memory allocation (mmap)

debugging tools for memory layout

runtime memory statistics tracking

## Future Improvements

Possible enhancements include:

Advanced Allocation Strategies

Best-Fit

Worst-Fit

Next-Fit

Thread Safety

mutex protection

lock-free structures

Segregated Free Lists

Multiple free lists for different block sizes.

Slab Allocator

Efficient allocation for small objects.

Reallocation Support
void* reallocate(void* ptr, size_t newSize);
Memory Leak Detection

Tracking unfreed allocations.

## Why This Project Matters

Understanding memory allocators is fundamental to many areas of computer science, including:

operating system design

runtime environments

game engines

databases

high-performance systems

Modern allocators such as jemalloc, tcmalloc, and ptmalloc build upon similar ideas including:

block metadata

free lists

fragmentation control

memory pooling

This project demonstrates the core principles behind production memory allocators.

## Educational Value

This project helps understand:

how malloc() works internally

memory fragmentation problems

low-level memory management

systems programming design

It provides a strong foundation for studying:

operating systems

compilers

high-performance systems

language runtimes

## Author

Aryan Paudel
Computer Science Student
Systems Programming Enthusiast

## License

MIT License

## Motivation

This project was built to deepen understanding of how dynamic memory allocators work internally in systems such as malloc, jemalloc, and operating system memory managers.
