#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <map>
#include <iostream>
#include "dld_ThreadIdentificator.hpp"

boost::mutex ThreadIdentifocator::_mtx;
std::map<boost::thread::id, long long> ThreadIdentifocator::_mapOfThreadsId;
long long ThreadIdentifocator::amountOfThreads = 0;

long long ThreadIdentifocator::getCurrentThreadId() {
    boost::lock_guard<boost::mutex> lockGuard( _mtx );
    boost::thread::id id = boost::this_thread::get_id();
    static std::map<boost::thread::id, long long>::iterator findedThreadIdIterator = _mapOfThreadsId.find( id ); //understand if we have already given id to this thread
    if( findedThreadIdIterator != _mapOfThreadsId.end() ) { //if this thread have already had id - return it
        return findedThreadIdIterator->second;
    } else { //if this thread have not already had id - create and return it
        _mapOfThreadsId.insert( std::make_pair( id, ++amountOfThreads ) );
        return _mapOfThreadsId.find( id )->second;
    }
}