#include "thread_pool.h"

ThreadPool::ThreadPool(size_t numThreads) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::move(task));
    }
    condition.notify_one();
}

void ThreadPool::wait() {
    std::unique_lock<std::mutex> lock(queueMutex);
    finished.wait(lock, [this]() { return tasks.empty() && activeTasks == 0; });
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]() { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
            ++activeTasks;
        }
        task();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            --activeTasks;
            if (tasks.empty() && activeTasks == 0) {
                finished.notify_all();
            }
        }
    }
}