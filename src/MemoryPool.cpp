#include "MemoryPool.h"
#include <cstdlib>
#include <algorithm>

MemoryPool::MemoryPool(size_t poolSize)
{
    this->pool = malloc(poolSize);
    this->poolSize = poolSize;
    // initial the free blocks list with the all pool as first free block.
    FreeBlock freeBlock = {pool, poolSize};
    this->freeBlocks = {freeBlock};
}

void *MemoryPool::allocate(size_t size)
{
    std::lock_guard<std::mutex> lock(poolMutex);
    void *allocationPointer = nullptr;
    for (int i = 0; i < freeBlocks.size(); ++i)
    {
        FreeBlock &block = freeBlocks[i];
        if (size + sizeof(size_t) <= block.size)
        {
            *(static_cast<size_t *>(block.start)) = size;
            allocationPointer =
                    static_cast<char *>(block.start) + sizeof(size_t);
            allocInBlock(size, i, block);
            break;
        }
    }
    return allocationPointer;
}

void
MemoryPool::allocInBlock(size_t sizeToAlloc, int blockIndex, FreeBlock &block)
{
    if (sizeToAlloc + sizeof(size_t) < block.size)
    {
        // If there is more space left in this block, change the
        // free pointer and resize the block
        block.start = static_cast<char *>(block.start) + sizeToAlloc +
                      sizeof(size_t);
        block.size -= (sizeToAlloc + sizeof(size_t));
    } else // sizeToAlloc = block.size
    {
        freeBlocks.erase(freeBlocks.begin() + blockIndex);
    }
}

void MemoryPool::deallocate(void *ptr)
{
    std::lock_guard<std::mutex> lock(poolMutex);
    if (ptr == nullptr)
    {
        return;
    }
    void *blockPtr = static_cast<char *>(ptr) - sizeof(size_t);
    size_t blockSize = *(static_cast<size_t *>(blockPtr));
    FreeBlock block = {blockPtr, blockSize + sizeof(size_t)};
    addAndMerge(block);
}

MemoryPool::~MemoryPool()
{
    if (this->pool)
    {
        free(this->pool);
        this->pool = nullptr;
        this->poolSize = 0;
    }
}

void MemoryPool::addAndMerge(FreeBlock block)
{
    char *blockPtr = static_cast<char *>(block.start);
    for (int i = 0; i < freeBlocks.size(); ++i)
    {
        FreeBlock &freeBlock = freeBlocks[i];
        if (static_cast<char *>(freeBlock.start) + freeBlock.size == blockPtr)
        {
            freeBlock.size += block.size;
            // check if adjacent block from both direction
            if (i + 1 < freeBlocks.size() && static_cast<char *>(freeBlock
                    .start) + freeBlock.size ==
                                             static_cast<char *>(freeBlocks[i +
                                                                            1].start))
            {
                freeBlock.size += freeBlocks[i + 1].size;
                freeBlocks.erase(freeBlocks.begin() + i + 1);
            }
            return;
        }
            // Check if adjacent block after
        else if (static_cast<char *>(freeBlock.start) == blockPtr + block.size)
        {
            freeBlock.start = block.start;
            freeBlock.size += block.size;
            return;
        }
    }
    // If no adjacent block found, insert the block to the list as is.
    freeBlocks.push_back(block); // No merge possible, just add the block
    // Keep freeBlocks sorted for better merging efficiency
    std::sort(freeBlocks.begin(), freeBlocks.end(),
              [](const FreeBlock &a, const FreeBlock &b)
              { return a.start < b.start; });

}
