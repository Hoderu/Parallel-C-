#include "DeadLockSafeMutex.hpp"

DeadLockSafeMutex mutex_3;
DeadLockSafeMutex mutex_1;
DeadLockSafeMutex mutex_2;
#define NUMBER_OF_ITERATIONS 100

class Function_1_with_deadlock {
public:
    void operator()() {
        for (int i = 0; i < NUMBER_OF_ITERATIONS; i++) {
            mutex_1.lock();
            mutex_2.lock();
            mutex_2.unlock();
            mutex_1.unlock();
        }
    }
};

class Function_2_with_deadlock {
public:
    void operator()() {
        for (int i = 0; i < NUMBER_OF_ITERATIONS; i++) {
            mutex_2.lock();
            mutex_1.lock();
            mutex_1.unlock();
            mutex_2.unlock();
        }
    }
};

class Function_1_without_deadlock {
public:
    void operator()() {
        for (int i = 0; i < NUMBER_OF_ITERATIONS; i++) {
            mutex_1.lock();
            mutex_1.unlock();
        }
    }
};

class Function_2_without_deadlock {
public:
    void operator()() {
        for (int i = 0; i < NUMBER_OF_ITERATIONS; i++) {
            mutex_2.lock();
            mutex_2.unlock();
        }
    }
};

int main() {
    std::cout << "Do you want deadlock? 1/0" << std::endl;
    int number;
    std::cin >> number;
    
    if (number == 1) {
        std::cout<<"Start deadlock"<<std::endl;
        boost::thread thread_1 = boost::thread(Function_1_with_deadlock());
        boost::thread thread_2 = boost::thread(Function_2_with_deadlock());
        thread_1.join();
        thread_2.join();
    } else if (number == 0) {
        boost::thread thread_1 = boost::thread(Function_1_without_deadlock());
        boost::thread thread_2 = boost::thread(Function_2_without_deadlock());
        thread_1.join();
        thread_2.join();
    } else {
        std::cout << "Incorrect input!" << std::endl;
        return 0;
    }
    std::cout << "Successful completion!" << std::endl;
    return 0;
}
