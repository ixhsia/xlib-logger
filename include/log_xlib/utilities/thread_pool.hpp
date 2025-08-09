#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP
#include <thread>
#include <vector>
#include <functional>
#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace xlib::Thread {
    //TODO: Implement a thread pool class that can manage a pool of threads for executing tasks concurrently.
    class ThreadPool {
        size_t thread_count_{};
        std::vector<std::thread> threads_;
    public:
        explicit ThreadPool(const size_t thread_count) {
            thread_count_ = thread_count;
        }
        ~ThreadPool() {
            for (auto& thread : threads_) {
                if (thread.joinable())
                    thread.join();
            }
        }
    };
}

#endif // THREAD_POOL_HPP