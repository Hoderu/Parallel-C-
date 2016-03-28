#pragma once

#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <atomic>
#include <iostream>
#include <map>
#include <set>
#include "dld_VertexForWaitForGraph.hpp"

typedef  std::map<long long, VertexForWaitForGraph*>::iterator WfgVerticesIterator;

class WaitForGraph { //declaring class for wait-for graph
private:
    static boost::mutex _mtx;
    std::map<long long, VertexForWaitForGraph*> _vertices; //map of all (thread and mutexes) vertices in wait-for-graph
    void addVertex ( long long id ); //adding vertex to wait-for-graph
    VertexForWaitForGraph* findOrCreateVertex ( long long id ); //return pointer to vertex if exists; create new vertex and return pointer if doesn't exists
    VertexForWaitForGraph* findVertex ( long long id ); //return pointer to vertex if exists; else return NULL;
    void setAllVerticesToWhite(); //function for bfs-deadlock-detection start. Set colors of all vertices to white
public:
    static bool isCheckerActive; //bool to undestertand if check-thread is active
    static std::atomic<int> amountOfMutexes; //amount of using mutexes; needed to know when check-thread must start
    ~WaitForGraph(); //free allocated memory
    void printGraph(); //debagging function: prints graph to the screen
    void deadlocksCheckVisit( VertexForWaitForGraph* startVertex ); //dfs visit
    void deadlocksCheck(); //checking for deadlocks with dfs; doing exit(1) if deadlock founded
    void addEdge(long long fromId, long long toId); //method for edding edge from A to B (one of A,B is thread and one of A,B is mutex)
    void deleteEdge(long long fromId, long long toId); ////method for remove edge from A to B (one of A,B is thread and one of A,B is mutex)
};
