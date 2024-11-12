# Multi-Threaded Memory Allocator

## Overview
This project implements a custom memory pool with thread-safe allocation and deallocation. It provides an efficient way to allocate and deallocate memory blocks from a pre-allocated memory pool, optimizing memory management for multi-threaded applications.

The memory pool supports:
- **Basic memory allocation and deallocation**.
- **Reusing deallocated blocks**.
- **Merging adjacent free blocks** for better space utilization.
- **Thread-safe operations** using mutex to manage access from multiple threads.

## Features
- **MemoryPool Class**: Allocates and deallocates blocks of memory from a pre-allocated pool.
- **Thread Safety**: Uses `std::mutex` to ensure thread-safe memory allocation and deallocation.
- **Efficient Memory Management**: Implements strategies like block reuse and merging of free blocks for efficient space utilization.

## Running Tests
Tests are included in the `tests/` folder and cover basic allocation, deallocation, block reuse, and multi-threaded scenarios. You can run the tests to ensure the correct functionality of the memory pool.

### Basic Tests
The basic tests check that memory allocation and deallocation works as expected:
- **Basic Allocation**
- **Deallocation and Reuse**
- **Merging Free Blocks**
- **Allocation After Deallocation**

### Multi-threaded Tests
The multi-threaded tests verify that the memory pool works correctly in concurrent environments:
- **Basic Multi-threaded Allocation Test**
- **Mixed Allocation and Deallocation Test**
- **Stress Test with Small Allocations**

## Code Structure
- **MemoryPool.h**: Header file for the `MemoryPool` class, containing the function declarations and private member variables.
- **MemoryPool.cpp**: Implementation file for the `MemoryPool` class, defining the constructor, allocator, deallocator, and helper functions.
- **tests/memoryPoolSimpleTests.cpp**: Contains basic test cases for verifying the functionality of the `MemoryPool` class.
- **tests/multiThreadingTests.cpp**: Contains multi-threaded test cases for verifying the memory pool under concurrent access.

## License
This project is licensed under the MIT License - see the LICENSE file for details.
