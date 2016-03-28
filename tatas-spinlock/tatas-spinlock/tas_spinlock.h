//
//  tas_spinlock.h
//  tatas-spinlock
//
//  Created by Андрей Решетников on 17.04.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#ifndef tatas_spinlock_tas_spinlock_h
#define tatas_spinlock_tas_spinlock_h

class tas_spinlock {
private:
    std::atomic<bool> locked;
public:
    tas_spinlock() : locked(0) {}
    void lock() {
        while (locked.exchange(1, std::memory_order_acquire));
    }
    bool try_lock() {
        return !locked.exchange(1);
    }
    void unlock() {
        locked.store(0);
    }
};

#endif
