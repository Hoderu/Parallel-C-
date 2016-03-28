//
//  rwlock.h
//  concurrent_heap
//
//  Created by Андрей Решетников on 23.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#ifndef concurrent_heap_rwlock_h
#define concurrent_heap_rwlock_h

#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include </Users/Hoderu/boost/boost_1_57_0/boost/thread/locks.hpp>
#include </Users/Hoderu/boost/boost_1_57_0/boost/thread/shared_mutex.hpp>

class rwlock {
private:
    boost::shared_mutex _access;
public:
    void reader() {
        boost::shared_lock<boost::shared_mutex> lock(_access);
    }
    
    void writer() {
        // get upgradable access
        boost::upgrade_lock<boost::shared_mutex> lock(_access);
        
        // get exclusive access
        boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
    }
};

#endif
