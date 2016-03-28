#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <set>
#include <iostream>
#include "My_Vertex.hpp"

My_Vertex::My_Vertex(int x) {
    id = x;
    color = 0;
}

size_t My_Vertex::getColor() {
    return color;
}

int My_Vertex::getId() {
    return id;
}

void My_Vertex::setColor(int x) {
    if(x >= 0 && x <= 2) {
        color = x;
    }
}

