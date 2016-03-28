//
//  spinlocks.h
//  SpinLock
//
//  Created by Андрей Решетников on 06.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <atomic>
#include <thread>

namespace mipt_parallel {
    
    // naive implementation of Test-And-Set (TAS) spinlock
    // http://en.cppreference.com/w/cpp/atomic/atomic_flag
    
    class tas_spinlock {
    public:
        void lock() {
            while (locked_.test_and_set()) {
                std::this_thread::yield();
            }
        }
        
        bool try_lock() {
            return !locked_.test_and_set();
        }
        
        void unlock() {
            locked_.clear();
        }
        
    private:
        std::atomic_flag locked_ = ATOMIC_FLAG_INIT;
    };
    
}