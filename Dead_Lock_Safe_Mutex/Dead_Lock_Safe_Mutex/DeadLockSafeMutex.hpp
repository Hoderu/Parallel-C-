#include "Thread_Id.hpp"
#include "My_Vertex.hpp"
#include "Resourse_Allocation_Graph.hpp"
#include "Checker.hpp"

class DeadLockSafeMutex {
private:
    static std::atomic<int> count;
    int id;
    boost::mutex mutex_for_DLSM;
    static Resourse_Allocation_Graph my_graph;
public:
    DeadLockSafeMutex();
    void lock();
    void unlock();
};