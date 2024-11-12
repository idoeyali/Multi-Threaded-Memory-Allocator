#include "src/MemoryPool.h"
#include <iostream>
#include "tests/memoryPoolSimpleTests.h"
#include "tests/multiThreadingTests.h"

int main()
{
    runAllSimpleTests();

    runMultiThreadsTests();
    return 0;
}
