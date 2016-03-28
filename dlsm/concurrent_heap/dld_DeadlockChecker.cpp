#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <iostream>
#include "dld_DeadlockChecker.hpp"

void sleep( int msec ) { //function to make thread sleep
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC_);
    xt.sec += msec/1000;
    xt.nsec += msec * 1000000;

    boost::thread::sleep(xt);
}

FunctorForThreadForDetectingDeadlocks::FunctorForThreadForDetectingDeadlocks( WaitForGraph* graph ) {
    _graph = graph;
}

void FunctorForThreadForDetectingDeadlocks::operator()() {
    while ( _graph->amountOfMutexes > 0 ) { //do check only if there are any using mutexes
        _graph->deadlocksCheck(); //start finding cicles in wait-for graph with dfs
        sleep(1000);
    }
    _graph->isCheckerActive = false; //stop checking if there are no using mutexes
}

void initDeadlocksChecker( WaitForGraph* graph ) { //function that initialize deadlock-detecting-rhread
    if ( !graph->isCheckerActive ) { //if there are using mutexes but there is not check-thread, start chech-thread
        boost::thread threadForDetectingDeadlocks = boost::thread(FunctorForThreadForDetectingDeadlocks( graph )); //starting check-thread
        graph->isCheckerActive = true;
    }
}