#pragma once

#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <iostream>
#include "dld_ThreadIdentificator.hpp"
#include "dld_VertexForWaitForGraph.hpp"
#include "dld_WaitForGraph.hpp"
#include "dld_DeadlockChecker.hpp"

class DlsMutex { //our deadlocks-detecting mutex
private:
    long long _id; //mutex id
    boost::mutex _mtxForDls; //mutext to use
    static WaitForGraph _graph; //wait-for graph
    static std::atomic<long long> _amountOfDlsMutexes; //amount of initialized deadlocks-detecting mutexes
public:
    DlsMutex();
    void lock(); //redeclaration std::mutex lock()
    void unlock(); //redeclaration std::mutex unlock()
    bool try_lock(); //redeclaration std::mutex try_lock()
};