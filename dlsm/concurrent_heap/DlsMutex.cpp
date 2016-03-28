#include <boost/thread.hpp>
#include <iostream>
#include "DlsMutex.hpp"
#include "dld_ThreadIdentificator.hpp"
#include "dld_VertexForWaitForGraph.hpp"
#include "dld_WaitForGraph.hpp"
#include "dld_DeadlockChecker.hpp"

WaitForGraph DlsMutex::_graph;
std::atomic<long long> DlsMutex::_amountOfDlsMutexes;

DlsMutex::DlsMutex() {
    _id = --( _amountOfDlsMutexes );
}
void DlsMutex::lock() {
    _graph.addEdge( ThreadIdentifocator::getCurrentThreadId(), _id ); //adding an edge from the thread to the mutex to the WaitForGraph
    ++_graph.amountOfMutexes; //increase amount of using mutexes
    initDeadlocksChecker( &_graph ); //if we are there at the first time, deadlock-detection thread will be started
    _mtxForDls.lock(); //lock mutex
    _graph.deleteEdge( ThreadIdentifocator::getCurrentThreadId(), _id ); //deleteng an edge from the thread to the mutex to the WaitForGraph
    _graph.addEdge( _id, ThreadIdentifocator::getCurrentThreadId() ); //adding an edge from the mutex to the thread to the WaitForGraph
}
void DlsMutex::unlock() {
    _graph.deleteEdge( _id, ThreadIdentifocator::getCurrentThreadId() ); //here must be deleting an edge from the mutex to the thread to the WaitForGraph
    _mtxForDls.unlock(); //unlock mutex
    --_graph.amountOfMutexes; //decrease amount of using mutexes
}
bool DlsMutex::try_lock() {
    if( _mtxForDls.try_lock() ) {
        ++_graph.amountOfMutexes;
        _graph.addEdge( _id, ThreadIdentifocator::getCurrentThreadId() );
        initDeadlocksChecker( &_graph );
        return true;
    } else {
        return false;
    }
}