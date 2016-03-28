#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <atomic>
#include <iostream>
#include <map>
#include <set>
#include "dld_WaitForGraph.hpp"
#include "dld_VertexForWaitForGraph.hpp"

boost::mutex WaitForGraph::_mtx;
std::atomic<int> WaitForGraph::amountOfMutexes;
bool WaitForGraph::isCheckerActive = false;

typedef  std::map<long long, VertexForWaitForGraph*>::iterator WfgVerticesIterator;

void WaitForGraph::addVertex ( long long id ) { //adding vertex with given to wait-for-graph
    _vertices.insert( std::make_pair( id, new VertexForWaitForGraph( id ) ) );
}

VertexForWaitForGraph* WaitForGraph::findOrCreateVertex ( long long id ) { //return pointer to vertex if exists; create new vertex and return pointer if doesn't exists
    WfgVerticesIterator requestedVertex = _vertices.find( id ); //try to find vertex with given id in map of vertices
    if( requestedVertex == _vertices.end() ) { //if was not found - insert to map and return pointer to vertex with given id
        addVertex( id );
        return _vertices.find( id )->second;
    } else { //if was found - just return pointer to vertex with given id
        return requestedVertex->second;
    }
}

VertexForWaitForGraph* WaitForGraph::findVertex ( long long id ) { //return pointer to vertex if exists; else return NULL;
    WfgVerticesIterator requestedVertex = _vertices.find( id );
    if( requestedVertex == _vertices.end() ) {
        return NULL;
    } else {
        return requestedVertex->second;
    }
}

void WaitForGraph::setAllVerticesToWhite() { //function for bfs-deadlock-detection start. Set colors of all vertices to white
    for (WfgVerticesIterator currentVertex = _vertices.begin(); currentVertex != _vertices.end(); ++currentVertex) {
        currentVertex->second->setColor( 0 );
    }
}

WaitForGraph::~WaitForGraph() { //free allocated in addVertex() method memory
    for (WfgVerticesIterator currentVertex = _vertices.begin(); currentVertex != _vertices.end(); ++currentVertex) {
        delete currentVertex->second;
    }
}

void WaitForGraph::printGraph() { //debagging function: prints graph to the screen
    for (WfgVerticesIterator currentVertex = _vertices.begin(); currentVertex != _vertices.end(); ++currentVertex) {
        std::cout<<currentVertex->second->getId()<<":"<<std::endl;
        for (std::set<VertexForWaitForGraph*>::iterator curNeighVertex = currentVertex->second->_vertexesTo.begin(); curNeighVertex != currentVertex->second->_vertexesTo.end(); ++curNeighVertex) {
            std::cout<<currentVertex->second->getId()<<" -> "<<(*curNeighVertex)->getId()<<std::endl;
        }
    }
}

void WaitForGraph::deadlocksCheckVisit( VertexForWaitForGraph* startVertex ) { //dfs visit
    for (std::set<VertexForWaitForGraph*>::iterator currentVertex = startVertex->_vertexesTo.begin(); currentVertex != startVertex->_vertexesTo.end(); ++currentVertex) {
        if( (*currentVertex)->getColor() == 1 ) { //here must be reaction for deadlock detection
            std::cout<<"Deadlock detected! Program terminated."<<std::endl;
            exit( 1 );
        }
        (*currentVertex)->setColor( 1 );
        deadlocksCheckVisit( *currentVertex );
        (*currentVertex)->setColor( 2 );
    }
}

void WaitForGraph::deadlocksCheck() { //dfs
    boost::lock_guard<boost::mutex> lockGuard( _mtx );
    setAllVerticesToWhite();
    for (WfgVerticesIterator currentVertex = _vertices.begin(); currentVertex != _vertices.end(); ++currentVertex) {
        if ( currentVertex->second->getColor() == 0) {
            currentVertex->second->setColor( 1 );
            deadlocksCheckVisit( currentVertex->second );
            currentVertex->second->setColor( 2 );
        }
    }
}
    
void WaitForGraph::addEdge(long long fromId, long long toId) { //method for edding edge from A to B (one of A,B is thread and one of A,B is mutex)
    boost::lock_guard<boost::mutex> lockGuard( _mtx );
    if(fromId*toId<0) { //we can add edge only from mutex to thread or from thread to mutex, nothing else
        VertexForWaitForGraph* fromVertex = findOrCreateVertex( fromId ); //try to find or create vertex with given "from" id
        VertexForWaitForGraph* toVertex = findOrCreateVertex( toId ); //try to find or create vertex with given "to" id
        if ( fromVertex->_vertexesTo.find( toVertex ) == fromVertex->_vertexesTo.end() ) { // check if this edge have already existed
            fromVertex->_vertexesTo.insert( toVertex ); //if this edge does not exist - add vertex "to" to "from"'s neighbors
        }
    }
}

void WaitForGraph::deleteEdge(long long fromId, long long toId) {
    boost::lock_guard<boost::mutex> lockGuard( _mtx );
    if(fromId*toId<0) { //we can delete edge only from mutex to thread or from thread to mutex, nothing else
        VertexForWaitForGraph* fromVertex = findVertex( fromId ); //try to find vertex with given "from" id
        VertexForWaitForGraph* toVertex = findVertex( toId ); //try to find vertex with given "from" id
        if ( fromVertex != NULL && toVertex != NULL ) { //if both of this vertices were founded, go on
            std::set<VertexForWaitForGraph*>::iterator toVertexInNeighbOfFromVertex = fromVertex->_vertexesTo.find( toVertex ); //find wertex "to" in neighborhood of vertex "from"
            if ( toVertexInNeighbOfFromVertex != fromVertex->_vertexesTo.end() ) { //if foundeed - delete it from the neighborhood
                fromVertex->_vertexesTo.erase( toVertex );
            }
        }
    }
}