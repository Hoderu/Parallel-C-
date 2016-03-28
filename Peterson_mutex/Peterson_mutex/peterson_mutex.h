//
//  peterson_mutex.h
//  Peterson_mutex
//
//  Created by Андрей Решетников on 13.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#pragma once

#include <atomic>
#include <thread>

namespace mipt_parallel {
    
    class peterson_mutex {
    public:
        peterson_mutex() {
            victim_.store(0);
            for (size_t i = 0; i < 2; ++i) {
                flag_[i].store(false);
            }
        }
        
        void first_thread_lock() {
            lock(0);
        }
        
        void first_thread_unlock() {
            unlock(0);
        }
        
        void second_thread_lock() {
            lock(1);
        }
        
        void second_thread_unlock() {
            unlock(1);
        }
        
    private:
        // thread_id in {0, 1}
        
        void lock(const std::size_t thread_id) {
            std::size_t other_thread_id = 1 - thread_id;
            
            flag_[thread_id].store(true, std::memory_order_acquire);
            
            victim_.store(other_thread_id, std::memory_order_acquire);
            
            
            while (flag_[other_thread_id].load(std::memory_order_release) && victim_.load(std::memory_order_release) == other_thread_id) {
                // wait
                //std::this_thread::yield();
            }
        };
        
        void unlock(const std::size_t thread_id) {
            flag_[thread_id].store(false, std::memory_order_acquire);
        }
        
    private:
        std::atomic<bool> flag_[2];
        std::atomic<int> victim_;
    };
    
}
