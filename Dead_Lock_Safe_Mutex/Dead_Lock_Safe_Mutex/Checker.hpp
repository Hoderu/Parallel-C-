#pragma once

#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <iostream>
#include "Resourse_Allocation_Graph.hpp"

class Checker {
private:
    Resourse_Allocation_Graph* graph;
public:
    Checker(Resourse_Allocation_Graph* x);
    void operator()();
};

void initDeadlocksChecker(Resourse_Allocation_Graph* graph);