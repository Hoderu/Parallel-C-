//
//  threadSafeQueue.h
//  Thread_Pool
//
//  Created by Андрей Решетников on 02.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#ifndef Thread_Pool_threadSafeQueue_h
#define Thread_Pool_threadSafeQueue_h

#include <mutex>
#include <thread>
#include <memory>
#include <queue>
#include <condition_variable>

template<typename T>
class threadSafeQueue {
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
    bool isEnd;
public:
    threadSafeQueue(): isEnd(false) {}
    threadSafeQueue(threadSafeQueue const& other) {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }
    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        //data_cond.wait(lk, [this]{return (!data_queue.empty() && !isEnd);});
        while (data_queue.empty() && !isEnd) {
            data_cond.wait(lk);
        }
        value = std::move(data_queue.front());
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        //data_cond.wait(lk, [this]{ return (!data_queue.empty() && !isEnd); });
        while (data_queue.empty() && !isEnd) {
            data_cond.wait(lk);
        }
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool empty() {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
    void shutDown() {
        std::unique_lock<std::mutex> lk(mut);
        isEnd = true;
        data_cond.notify_all();
    }
};

#endif
