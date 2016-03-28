//
//  HierarchicalMutex.h
//  HierarchicalMutex
//
//  Created by Андрей Решетников on 13.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//
#include <mutex>

__thread int levelOfThread = 0;

class HierarchicalMutex {
private:
    int levelOfMutex;
    int oldLevelOfMutex;
    std::mutex mtx;
    
public:
    HierarchicalMutex(int _level): levelOfMutex(_level), oldLevelOfMutex(0){}
    
    bool lock() {
        if (levelOfThread <= levelOfMutex) {
            mtx.lock();
            oldLevelOfMutex = levelOfThread;
            levelOfThread = levelOfMutex;
            std::cout << "levelOfMutex: " << levelOfMutex << std::endl;
            return true;
        }
        return false;
    }
    
    bool unlock() {
        if (levelOfThread == levelOfMutex) {
            levelOfThread = oldLevelOfMutex;
            mtx.unlock();
            return true;
        }
        return false;
    }
};