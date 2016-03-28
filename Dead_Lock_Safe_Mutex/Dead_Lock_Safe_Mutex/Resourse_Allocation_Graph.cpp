#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <atomic>
#include <iostream>
#include <map>
#include <set>
#include "Resourse_Allocation_Graph.hpp"
#include "My_Vertex.hpp"

boost::mutex Resourse_Allocation_Graph::graph_mutex;
std::atomic<int> Resourse_Allocation_Graph::count_of_graph_mutexes;
bool Resourse_Allocation_Graph::isCheckerActive = false;

Resourse_Allocation_Graph::~Resourse_Allocation_Graph() {
    for (auto i = vertices.begin(); i != vertices.end(); i++) {
        delete i->second;
    }
}

void Resourse_Allocation_Graph::addVertex (int id) {
    vertices.insert(std::make_pair(id, new My_Vertex(id)));
}

void Resourse_Allocation_Graph::addEdge(int x, int y) {
    boost::lock_guard<boost::mutex> lockGuard(graph_mutex);
    if ((x > 0 && y < 0) || (x < 0 && y > 0)) {
        My_Vertex* from = findOrCreateVertex(x);
        My_Vertex* to = findOrCreateVertex(y);
        if (from->neighbours.find(to) == from->neighbours.end()) {
            from->neighbours.insert(to);
        }
    }
}

void Resourse_Allocation_Graph::deleteEdge(int x, int y) {
    boost::lock_guard<boost::mutex> lockGuard(graph_mutex);
    if ((x > 0 && y < 0) || (x < 0 && y > 0)) {
        My_Vertex* from = findVertex(x);
        My_Vertex* to = findVertex(y);
        if (from != NULL && to != NULL) {
            auto i = from->neighbours.find(to);
            if (i != from->neighbours.end()) {
                from->neighbours.erase(to);
            }
        }
    }
}

My_Vertex* Resourse_Allocation_Graph::findOrCreateVertex(int id) {
    auto temp = vertices.find(id);
    if(temp == vertices.end()) {
        addVertex(id);
        return vertices.find(id)->second;
    } else {
        return temp->second;
    }
}

My_Vertex* Resourse_Allocation_Graph::findVertex(int id) {
    auto temp = vertices.find(id);
    if(temp == vertices.end()) {
        return NULL;
    } else {
        return temp->second;
    }
}

void Resourse_Allocation_Graph::All_Vertexes_To_White() {
    for (auto i = vertices.begin(); i != vertices.end(); i++) {
        i->second->setColor(0);
    }
}

void Resourse_Allocation_Graph::dfs(My_Vertex* startVertex) {
    for (auto i = startVertex->neighbours.begin(); i != startVertex->neighbours.end(); i++) {
        if((*i)->getColor() == 1) { //попали в серую вершину - нашли дедлок
            std::cout<<"Deadlock found!"<< std::endl;
            std::cout<<(*i)->getId() << " and " << (*startVertex).getId() << " are last vertexes of deadlock" <<std::endl;
            exit(1);
        }
        (*i)->setColor(1);
        dfs(*i);
        (*i)->setColor(2);
    }
}

void Resourse_Allocation_Graph::deadlocksCheck() {
    boost::lock_guard<boost::mutex> lockGuard(graph_mutex);
    All_Vertexes_To_White();
    for (auto i = vertices.begin(); i != vertices.end(); i++) {
        if (i->second->getColor() == 0) {
            i->second->setColor(1);
            dfs(i->second);
            i->second->setColor(2);
        }
    }
}