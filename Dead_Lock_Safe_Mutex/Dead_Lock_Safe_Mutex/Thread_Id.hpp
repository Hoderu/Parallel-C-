#pragma once

#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <map>
#include <iostream>
#include "My_Vertex.hpp"

class Thread_Id {
private:
    static boost::mutex mutex;
    static std::map<boost::thread::id, int> Map_For_Threads_Id;
    static int Count_Of_Thread;
public:
    static int Get_Thread_Id();
};
