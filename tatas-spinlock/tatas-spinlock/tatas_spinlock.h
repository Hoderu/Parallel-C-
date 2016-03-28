//
//  tatas_spinlock.h
//  tatas-spinlock
//
//  Created by Андрей Решетников on 17.04.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#ifndef tatas_spinlock_tatas_spinlock_h
#define tatas_spinlock_tatas_spinlock_h


class tatas_spinlock {
private:
    std::atomic<bool> locked;
public:
    tatas_spinlock() : locked(0) {}
    void lock() {
        while (true) {
            while (locked.load(std::memory_order_relaxed));
            if (!locked.exchange(1, std::memory_order_acquire)) {
                return;
            }
        }
    }
    bool try_lock() {
        return !locked.exchange(1);
    }
    void unlock() {
        locked.store(0);
    }
};
#endif
