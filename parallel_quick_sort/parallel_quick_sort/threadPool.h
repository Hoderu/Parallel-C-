//
//  threadPool.h
//  Thread_Pool
//
//  Created by Андрей Решетников on 02.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#ifndef Thread_Pool_threadPool_h
#define Thread_Pool_threadPool_h

#include "threadSafeQueue.h"
#include <future>
#include <functional>

template<typename ResultType>
class threadPool {
private:
    using Task = std::pair<std::promise<ResultType>, std::function<ResultType()>>;
    
    std::vector<std::thread> workers;
    //std::vector<thread_guard> guard_workers;
    int number_of_workers;
    
    threadSafeQueue<Task> task_queue;
    
    void worker() {
        while (true) {
            Task task;
            if (!task_queue.wait_and_pop(task)) {
                break;
            }
            try {
                ResultType result = task.second();
                task.first.set_value(result);
            } catch (...) {
                task.first.set_exception(std::current_exception());
            }
        }
    }
public:
    threadPool(threadPool& otherThreadPool) = delete;
    void operator =(threadPool& otherThreadPool) = delete;
    threadPool(size_t _number_of_workers) {
        number_of_workers = _number_of_workers;
        for (int i = 0; i < number_of_workers; i++) {
            workers.push_back(std::thread(&threadPool::worker, this));
        }
        /*for (int i = 0; i < number_of_workers; i++) {
         guard_workers.emplace_back(workers[i]);
         }*/
    }
    ~threadPool() {
        task_queue.shutDown();
        for (int i = 0; i < number_of_workers; i++) {
            if (workers[i].joinable()) {
                workers[i].join();
            }
        }
        //exit(0);
    }
    //очередь не пуста или стоит флажок чтобы остановиться
    
    void wait(std::future<ResultType>& async_result) {
        Task task;
        std::chrono::milliseconds span(0);
        while (async_result.wait_for(span) != std::future_status::ready) {
            if (!task_queue.try_pop(task)) {
                std::this_thread::yield();
                continue;
            }
            try {
                const ResultType result = (task.second)();
                task.first.set_value(result);
            } catch (...) {
                task.first.set_exception(std::current_exception());
            }
        }
    }
    
    std::future<ResultType> submit(std::function<ResultType()> f) {
        
        std::promise<ResultType> task_promise;
        std::future<ResultType> task_future = task_promise.get_future();
        
        task_queue.push(std::make_pair(std::move(task_promise), f));
        
        return task_future;
    }
};

#endif
