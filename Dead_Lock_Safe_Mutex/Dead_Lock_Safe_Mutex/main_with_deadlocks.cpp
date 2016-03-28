#include </Users/Hoderu/boost/boost_1_57_0/boost/thread.hpp>
#include <iostream>
#include "DlsMutex.hpp"

DlsMutex my_w_dlsMutex1;
DlsMutex my_w_dlsMutex2;
DlsMutex my_w_dlsMutex3;

class ThreadFunction1 { //declaring of the class for the thread-function
public:
    void operator()() {
        for (int i = 0; i<10000; ++i) {
            my_w_dlsMutex1.lock();
            my_w_dlsMutex2.lock();
            my_w_dlsMutex3.lock();
            my_w_dlsMutex3.unlock();
            my_w_dlsMutex2.unlock();
            my_w_dlsMutex1.unlock();
        }
    }
};

class ThreadFunction2 { //declaring of the class for the thread-function
public:
    void operator()() {
        for (int i = 0; i<10000; ++i) {
            my_w_dlsMutex2.lock();
            my_w_dlsMutex3.lock();
            my_w_dlsMutex1.lock();
            my_w_dlsMutex1.unlock();
            my_w_dlsMutex3.unlock();
            my_w_dlsMutex2.unlock();
        }
    }
};

class ThreadFunction3 { //declaring of the class for the thread-function
public:
    void operator()() {
        for (int i = 0; i<10000; ++i) {
            my_w_dlsMutex3.lock();
            my_w_dlsMutex1.lock();
            my_w_dlsMutex2.lock();
            my_w_dlsMutex2.unlock();
            my_w_dlsMutex1.unlock();
            my_w_dlsMutex3.lock();
        }
    }
};

int my_w_main() {
    boost::thread thr1 = boost::thread( ThreadFunction1() );
    boost::thread thr2 = boost::thread( ThreadFunction2() );
    boost::thread thr3 = boost::thread( ThreadFunction3() );
    thr1.join();
    thr2.join();
    thr3.join();

    return 0;
}