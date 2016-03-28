#pragma once

#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <set>
#include <iostream>

class VertexForWaitForGraph { //declaring class for vertices for wait-for graph
private:
    long long _id; //id of mutexes will be over 0, id of threads will be under 0
    size_t _color; //0 - white; 1 - grey; 2 - black
public:
    std::set<VertexForWaitForGraph*> _vertexesTo; //set of neighbors of this vertex
    VertexForWaitForGraph(long long id); //create vertex of wait-for graph
    void setColor(int color); //set new color for dfs to vertex
    size_t getColor(); //returns number of color for dfs of this vertex
    long long getId(); //returns id of this vertex
};
