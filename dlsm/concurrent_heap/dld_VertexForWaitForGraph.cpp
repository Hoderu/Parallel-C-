#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <set>
#include <iostream>
#include "dld_VertexForWaitForGraph.hpp"

VertexForWaitForGraph::VertexForWaitForGraph(long long id) {
    _id = id;
    _color = 0;
}

void VertexForWaitForGraph::setColor(int color) {
    if(color >= 0 && color <=2) {
        _color = color;
    }
}

size_t VertexForWaitForGraph::getColor() {
    return _color;
}
    
long long VertexForWaitForGraph::getId() {
    return _id;
}
