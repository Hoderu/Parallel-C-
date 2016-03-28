#pragma once
#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <atomic>
#include <iostream>
#include <map>
#include <set>
#include "My_Vertex.hpp"

class Resourse_Allocation_Graph {
private:
    static boost::mutex graph_mutex;
    std::map<int, My_Vertex*> vertices;
    void addVertex(int id);
    My_Vertex* findOrCreateVertex(int id);
    My_Vertex* findVertex(int id);
    void All_Vertexes_To_White();
public:
    ~Resourse_Allocation_Graph();
    static std::atomic<int> count_of_graph_mutexes;
    void addEdge(int x, int y);
    void deleteEdge(int x, int y);
    void dfs(My_Vertex* startVertex);
    void deadlocksCheck();
    static bool isCheckerActive;
};
