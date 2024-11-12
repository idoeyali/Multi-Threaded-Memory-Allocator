#ifndef MULTI_THREADED_MEMORY_ALLOCATOR_MEMORYPOOL_H
#define MULTI_THREADED_MEMORY_ALLOCATOR_MEMORYPOOL_H
#include <cstddef>
#include <cstdint>
#include <mutex>
 #include "vector"
using namespace std;

struct FreeBlock{
    void *start;
    size_t size;
};
class MemoryPool{
public:
    // Constructor: initializes the memory pool with the specified size
    MemoryPool(size_t poolSize);

    // Allocates a block of memory from the pool, return pointer to the
    // allocation start and nullptr if there is no place in memory pool to
    // allocate.
    // Note : The size of the allocated memory stored right before the user
    // memory.
    void* allocate(size_t size);

    // Deallocates a previously allocated block (implementation details to follow)
    void deallocate(void* ptr);

    // Destructor: releases the memory pool
    ~MemoryPool();

private:
    // Pointer to the start of the memory pool
    void* pool;

    // Size of the memory pool
    size_t poolSize;

    // A list of available memory blocks within the pool
    std::vector<FreeBlock> freeBlocks;

    // Class mutex to manage thread safety
    std::mutex poolMutex;

    // given free block, his index at the freeBlocks list and size to
    // allocate in this block, change his start&size accordingly or erase it
    // from the list
    void allocInBlock(size_t sizeToAlloc, int blockIndex, FreeBlock &block);

    // Add a block to the free blocks list and merge it with other block if
    // they are adjacent blocks for efficiency
    void addAndMerge(FreeBlock block);
};

#endif //MULTI_THREADED_MEMORY_ALLOCATOR_MEMORYPOOL_H
