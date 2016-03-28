#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <iostream>
#include "DlsMutex.hpp"

DlsMutex dlsMutex1;
DlsMutex dlsMutex2;
DlsMutex dlsMutex3;

class ThreadFunction1 { //declaring of the class for the thread-function
public:
    void operator()() {
        for (int i = 0; i<10000; ++i) {
            dlsMutex1.lock();
            dlsMutex2.lock();
            dlsMutex3.lock();
            dlsMutex3.unlock();
            dlsMutex2.unlock();
            dlsMutex1.unlock();
        }
    }
};

class ThreadFunction2 { //declaring of the class for the thread-function
public:
    void operator()() {
        for (int i = 0; i<10000; ++i) {
            dlsMutex2.lock();
            dlsMutex3.lock();
            dlsMutex1.lock();
            dlsMutex1.unlock();
            dlsMutex3.unlock();
            dlsMutex2.unlock();
        }
    }
};

class ThreadFunction3 { //declaring of the class for the thread-function
public:
    void operator()() {
        for (int i = 0; i<10000; ++i) {
            dlsMutex3.lock();
            dlsMutex1.lock();
            dlsMutex2.lock();
            dlsMutex2.unlock();
            dlsMutex1.unlock();
            dlsMutex3.lock();
        }
    }
};

int main() {
    std::cout<<"Start simple thread..."<<std::endl;
    boost::thread thr4 = boost::thread( ThreadFunction1() );
    std::cout<<"Start  sleeping..."<<std::endl;
    sleep( 5000 );
    std::cout<<"Start deadlock..."<<std::endl;
    boost::thread thr1 = boost::thread( ThreadFunction1() );
    boost::thread thr2 = boost::thread( ThreadFunction2() );
    boost::thread thr3 = boost::thread( ThreadFunction2() );
    thr1.join();
    thr2.join();
    thr3.join();

    return 0;
}
