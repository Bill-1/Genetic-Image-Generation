#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    // Enqueue a new task
    void enqueue(std::function<void()> task);

    // Wait for all tasks to complete
    void wait();

private:
    // Worker function for each thread
    void worker();

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    std::condition_variable finished;
    bool stop = false;
    size_t activeTasks = 0;
};

#endif