#pragma once

#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <iostream>
#include "dld_WaitForGraph.hpp"

void sleep( int msec ); //function to make thread sleep

class FunctorForThreadForDetectingDeadlocks { //functor for the deadlocks-detecting-thread
private:
    WaitForGraph* _graph; //pointer to the wait-for graph in which we will find deadlocks
public:
    FunctorForThreadForDetectingDeadlocks( WaitForGraph* graph ); //constructor for class containing fuctor for thread for detecting deadlocks;
    void operator()();
};

void initDeadlocksChecker( WaitForGraph* graph ); //function that initialize deadlock-detecting-rhread