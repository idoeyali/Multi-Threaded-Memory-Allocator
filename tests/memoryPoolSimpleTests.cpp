#include "MemoryPool.h"
#include "memoryPoolSimpleTests.h"
#include <iostream>
#include <cassert>

// Utility function for logging test results
void logTestResult(const std::string &testName, bool result)
{
    std::cout << (result ? "[PASS] " : "[FAIL] ") << testName << std::endl;
}

void testBasicAllocation()
{
    MemoryPool pool(1024); // Create a pool with 1024 bytes

    // Try allocating 128 bytes
    void *ptr1 = pool.allocate(128);
    assert(ptr1 != nullptr); // Check that allocation succeeded
    logTestResult("Basic Allocation - 128 bytes", ptr1 != nullptr);

    // Try allocating another 256 bytes
    void *ptr2 = pool.allocate(256);
    assert(ptr2 != nullptr); // Check that allocation succeeded
    logTestResult("Basic Allocation - 256 bytes", ptr2 != nullptr);

    // Try allocating more than remaining pool size (should fail)
    void *ptr3 = pool.allocate(800);
    assert(ptr3 == nullptr); // Should return nullptr since not enough space
    logTestResult("Basic Allocation - Over Allocation", ptr3 == nullptr);
}

void testDeallocationAndReuse()
{
    MemoryPool pool(512); // Create a pool with 512 bytes

    // Allocate and deallocate a block of memory
    void *ptr1 = pool.allocate(128);
    pool.deallocate(ptr1);

    // Now try to allocate the same size again (should succeed)
    void *ptr2 = pool.allocate(128);
    assert(ptr2 != nullptr);
    logTestResult("Deallocation and Reuse - 128 bytes", ptr2 != nullptr);
}

void testMergingFreeBlocks()
{
    MemoryPool pool(512); // Create a pool with 512 bytes

    // Allocate two blocks
    void *ptr1 = pool.allocate(128);
    void *ptr2 = pool.allocate(128);

    // Deallocate both blocks in different orders
    pool.deallocate(ptr1);
    pool.deallocate(ptr2);

    // Now try to allocate a block that requires merged space
    void *ptr3 = pool.allocate(256); // Should succeed if blocks are merged
    assert(ptr3 != nullptr);
    logTestResult("Merging Free Blocks - 256 bytes", ptr3 != nullptr);
}

void testAllocationAfterDeallocation()
{
    MemoryPool pool(1024); // Create a pool with 1024 bytes

    // Allocate multiple blocks
    void *ptr1 = pool.allocate(128);
    void *ptr2 = pool.allocate(128);
    void *ptr3 = pool.allocate(256);

    // Deallocate middle block
    pool.deallocate(ptr2);

    // Allocate a block that fits into the deallocated space
    void *ptr4 = pool.allocate(128);
    assert(ptr4 != nullptr);
    logTestResult("Allocation After Deallocation - Reuse 128 bytes",
                  ptr4 != nullptr);
}

void runAllSimpleTests()
{
    std::cout << "Starting Basic Allocation and Deallocation Test..." <<
              std::endl;
    testBasicAllocation();
    testDeallocationAndReuse();
    testMergingFreeBlocks();
    testAllocationAfterDeallocation();

    std::cout << "All simple tests completed.\n" << std::endl;
}
