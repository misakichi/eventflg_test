#ifndef _EVT_CV_H
#define _EVT_CV_H

#include <mutex>
#include <condition_variable>
#include <atomic>

class EventConditionalVariable {
public:
    void initialize()
    {
        v_ = 0;
    }

    void terminate()
    {
    }


    void wait()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this] { return v_ == 1; });

    }

    void signal()
    {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            v_ = 1;
        }
        cv_.notify_all();
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        v_ = 0;
    }

private:
    std::atomic_int v_;
    std::condition_variable cv_;
    std::mutex  mtx_;
};

#endif