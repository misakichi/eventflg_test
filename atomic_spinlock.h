#ifndef _ATOMIC_SPINLOCK_H
#define _ATOMIC_SPINLOCK_H

#include <assert.h>
#include <atomic>
#include <sched.h>

template<typename CounterType>
class SpinLock {
public:
    void initialize()
    {
        evt_ = 0;
    }


    void terminate()
    {
    }


    void wait()
    {
        constexpr int yieldMask = 0x3f;
        int cnt = 0;
        while (evt_ == 0)
        {
            if (((cnt++) & yieldMask) == yieldMask)
                sched_yield();
        }


    }

    void signal()
    {
        evt_ = 1;
    }

    void clear()
    {
        evt_ = 0;
    }

private:
    CounterType evt_;
};

using AtomicSpinLock = SpinLock<std::atomic_int>;
using VolatileSpinLock = SpinLock<volatile int>;

#endif