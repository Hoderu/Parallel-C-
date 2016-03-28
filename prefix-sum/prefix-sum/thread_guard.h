//
//  threadGuard.h
//  Thread_Pool
//
//  Created by Андрей Решетников on 02.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#ifndef Thread_Pool_threadGuard_h
#define Thread_Pool_threadGuard_h

#include <iostream>
#include <thread>

class thread_guard {
public:
    
    thread_guard(std::thread& _t): t(_t){}
    thread_guard(thread_guard&& _t): t(_t.t){};
    
    ~thread_guard(){
        if (t.joinable()) {
            t.join();
        }
    }
private:
    std::thread& t;
    
    void operator=(const thread_guard& otherLockGuard) = delete;
    //thread_guard(const thread_guard& _t) = delete;
};

#endif
