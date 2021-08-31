#include <cstdio>
#include <pthread.h>
#include <thread>
#include <chrono>
#include <atomic>

#include "atomic_spinlock.h"
#include "evt_epoll.h"
#include "evt_poll.h"
#include "evt_cv.h"

template<class TestClass>
void test_job(std::atomic_int& threadRunCount, std::atomic_int& runCounter,TestClass& signal, TestClass& returnSignal, volatile bool& running, unsigned int index)
{
    ++threadRunCount;
    while (running) {
        signal.wait();
        if (!running)
            break;
        signal.clear();
        ++runCounter;
        returnSignal.signal();
    }
    returnSignal.signal();
    --threadRunCount;
}

template<class TestClass>
void test_job(unsigned testCount, const char* testName)
{
    TestClass signal;
    TestClass returnSignal;
    signal.initialize();
    returnSignal.initialize();

    volatile bool running = true;
    std::atomic_int runThreadCount(0);
    std::atomic_int runCounter(0);

    auto threadCount = std::thread::hardware_concurrency();
    std::thread** threads = new std::thread * [threadCount];
    for (unsigned ti = 0; ti < threadCount; ti++)
    {
        threads[ti] = new std::thread([&runThreadCount, &runCounter, &signal, &returnSignal, &running, ti]() {
            test_job<TestClass>(runThreadCount, runCounter, signal, returnSignal, running, ti);
        });
    }


    int test_count = 100000;
    running = true;
    printf("test count=%8d / thread_count=%2d  [%s] \n", test_count, threadCount, testName);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; i++) {
        signal.signal();
        returnSignal.wait();
        returnSignal.clear();
    }
    running = false;
    while (runThreadCount > 0)
        signal.signal();

    for (unsigned ti = 0; ti < threadCount; ti++)
    {
        threads[ti]->join();
        delete threads[ti];
    }
    delete[] threads;

    auto finish = std::chrono::high_resolution_clock::now() - start;
    printf("proc time:%.6f msec / run = %8d\n", (float)std::chrono::duration_cast<std::chrono::nanoseconds>(finish).count() * 1.0e-6, runCounter.load());

    signal.terminate();
    returnSignal.terminate();
}


int main()
{
    constexpr unsigned testCount = 100000;
    test_job<EventEPoll>(testCount, "epoll");
    test_job<EventPoll>(testCount, "poll");
    test_job<EventConditionalVariable>(testCount, "ConditionalVariable");
    test_job<VolatileSpinLock>(testCount, "volatile spinlock");
    test_job<AtomicSpinLock>(testCount, "atomic spinlock");



    return 0;
}