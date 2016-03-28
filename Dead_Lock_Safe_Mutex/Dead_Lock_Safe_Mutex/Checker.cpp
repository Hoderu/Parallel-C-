#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <iostream>
#include "Checker.hpp"

Checker::Checker(Resourse_Allocation_Graph* x) {
    graph = x;
}

void Checker::operator()() {
    while (graph->count_of_graph_mutexes > 0) {
        graph->deadlocksCheck();
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
    graph->isCheckerActive = false;
}

void initDeadlocksChecker(Resourse_Allocation_Graph* graph) {
    if (!graph->isCheckerActive) {
        boost::thread threadForDetectingDeadlocks = boost::thread(Checker(graph));
        graph->isCheckerActive = true;
    }
}