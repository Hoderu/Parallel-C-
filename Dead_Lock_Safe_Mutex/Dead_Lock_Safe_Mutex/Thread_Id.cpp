#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <map>
#include <iostream>
#include "Thread_Id.hpp"

boost::mutex Thread_Id::mutex;
std::map<boost::thread::id, int> Thread_Id::Map_For_Threads_Id;
int Thread_Id::Count_Of_Thread = 0;

int Thread_Id::Get_Thread_Id() {
    boost::lock_guard<boost::mutex> lockGuard(mutex);
    boost::thread::id id = boost::this_thread::get_id();
    auto it = Map_For_Threads_Id.find(id);
    if(it != Map_For_Threads_Id.end()) {
        return it->second;
    } else {
        Map_For_Threads_Id.insert(std::make_pair(id, ++Count_Of_Thread));
        return Map_For_Threads_Id.find(id)->second;
    }
}