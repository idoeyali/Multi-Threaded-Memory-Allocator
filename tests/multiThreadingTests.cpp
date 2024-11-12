#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include "MemoryPool.h"    // Include your MemoryPool class
#include "multiThreadingTests.h"

const size_t poolSize = 1024 * 1024; // 1 MB pool

void allocationTest(MemoryPool &pool, size_t allocSize, int iterations)
{
    for (int i = 0; i < iterations; ++i)
    {
        void *ptr = pool.allocate(allocSize);
        assert(ptr != nullptr);
        pool.deallocate(ptr);
    }
}

void mixedAllocDeallocTest(MemoryPool &pool, size_t allocSize, int iterations)
{
    for (int i = 0; i < iterations; ++i)
    {
        void *ptr = pool.allocate(allocSize);
        if (ptr)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            pool.deallocate(ptr);
        }
    }
}

void stressTest(MemoryPool &pool, int iterations)
{
    for (int i = 0; i < iterations; ++i)
    {
        size_t allocSize = (rand() % 64) + 1;
        void *ptr = pool.allocate(allocSize);
        if (ptr)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(5));
            pool.deallocate(ptr);
        }
    }
}

void runMultiThreadsTests()
{
    std::cout << "Starting Multi-Threaded Memory Test..." << std::endl;
    MemoryPool pool(poolSize);

    // Basic Multi-threaded Allocation Test
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i)
        {
            threads.emplace_back(allocationTest, std::ref(pool), 128, 100);
        }
        for (auto &t: threads) t.join();
        std::cout << "[PASS] Basic Multi-threaded Allocation Test"
                  << std::endl;
    }

    // Mixed Allocation and Deallocation Test
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i)
        {
            threads.emplace_back(mixedAllocDeallocTest, std::ref(pool), 256,
                                 100);
        }
        for (auto &t: threads) t.join();
        std::cout << "[PASS] Mixed Allocation and Deallocation Test"
                  << std::endl;
    }

    // Stress Test with Small Allocations
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < 20; ++i)
        {
            threads.emplace_back(stressTest, std::ref(pool), 100);
        }
        for (auto &t: threads) t.join();
        std::cout << "[PASS] Stress Test with Small Allocations"
                  << std::endl;
    }
    std::cout << "All Multi-Threaded Memory Tests completed." << std::endl;
}
