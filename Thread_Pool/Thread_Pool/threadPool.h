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
    std::vector<std::thread> workers;
    //std::vector<thread_guard> guard_workers;
    int number_of_workers;
    
    threadSafeQueue<std::pair<std::promise<ResultType>, std::function<ResultType()>>> task_queue;
    
    void worker() {
        while (true) {
            std::pair<std::promise<ResultType>, std::function<ResultType()>> task;
            task_queue.wait_and_pop(task);
            if (task.second != nullptr) {
                task.first.set_value(task.second());
            }
        }
    }
public:
    threadPool(threadPool& otherThreadPool) = delete;
    void operator =(threadPool& otherThreadPool) = delete;
    threadPool(int _number_of_workers) {
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
        /*for (int i = 0; i < number_of_workers; i++) {
            if (workers[i].joinable()) {
                workers[i].join();
            }
        }*/
    }
    //очередь не пуста или стоит флажок чтобы остановиться
    
    std::future<ResultType> submit(std::function<ResultType()> f) {
        
        std::promise<ResultType> task_promise;
        std::future<ResultType> task_future = task_promise.get_future();
        
        task_queue.push(std::make_pair(std::move(task_promise), f));
        
        return task_future;
    }
};

#endif
