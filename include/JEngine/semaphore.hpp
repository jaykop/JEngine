#pragma once

#include <mutex>
#include <thread>
#include <condition_variable>

class Semaphore
{
public:
    Semaphore() = delete;
    Semaphore(int i) : count_(i), mtx_(), cv_() {}

    void wait() {
        // set lk to use condition_variable
        std::unique_lock<std::mutex> lk(mtx_);
        // do not wait when the count is greater than zero
        cv_.wait(lk, [&] { return count_ > 0; });
        // decrease the count
        count_--;
    }

    void signal() {
        // increase the count
        count_++;
        // wake one thread and let it work 
        // whenever send a signal
        cv_.notify_one();
    }

private:
    int count_;
    std::mutex mtx_;
    std::condition_variable cv_;
};
