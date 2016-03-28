#pragma once

#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <map>
#include <iostream>
#include "dld_VertexForWaitForGraph.hpp"

class ThreadIdentifocator {
private:
    static boost::mutex _mtx; //mutex to synchronize threads while they are working with map
    static std::map<boost::thread::id, long long> _mapOfThreadsId; //map displays real and pseudo threads' id
    static long long amountOfThreads; //amount of threads for setting new ids
public:
    static long long getCurrentThreadId(); //recognises thread and returns its pseudo id
};
