#include "DeadLockSafeMutex.hpp"
#include "Thread_Id.hpp"
#include "My_Vertex.hpp"
#include "Resourse_Allocation_Graph.hpp"
#include "Checker.hpp"

std::atomic<int> DeadLockSafeMutex::count;
Resourse_Allocation_Graph DeadLockSafeMutex::my_graph;

DeadLockSafeMutex::DeadLockSafeMutex() {
    id = --count;
}

void DeadLockSafeMutex::lock() {
    my_graph.addEdge(Thread_Id::Get_Thread_Id(), id); // добавление request ребра
    ++my_graph.count_of_graph_mutexes;
    initDeadlocksChecker(&my_graph);
    mutex_for_DLSM.lock();
    my_graph.deleteEdge(Thread_Id::Get_Thread_Id(), id); // удаление request ребра
    my_graph.addEdge(id, Thread_Id::Get_Thread_Id()); // добавление hold ребра
}

void DeadLockSafeMutex::unlock() {
    my_graph.deleteEdge(id, Thread_Id::Get_Thread_Id()); // удаление hold ребра
    mutex_for_DLSM.unlock();
    --my_graph.count_of_graph_mutexes;
}