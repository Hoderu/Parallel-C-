#pragma once

#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <set>
#include <iostream>

class My_Vertex {
private:
    int id;
    size_t color; //0 - белый; 1 - серый; 2 - черный
public:
    My_Vertex(int x);
    std::set<My_Vertex*> neighbours;
    size_t getColor();
    void setColor(int x);
    int getId();
};
