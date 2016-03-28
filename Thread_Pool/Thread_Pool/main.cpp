//
//  main.cpp
//  Thread_Pool
//
//  Created by Андрей Решетников on 02.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <iostream>
#include "threadGuard.h"
#include "threadPool.h"
#include <random>
#include <time.h>
#include <vector>
#include <future>

#define N 10000000

/*
//-----------------------------Producer and Consumers-----------------------

threadSafeQueue<int> safe_queue;
std::mutex my_mutex;

void producer() {
    srand((unsigned int)time(0));
    size_t count = 10;//rand() % 10 + 1;
    for (int i = 0; i < count; i++) {
        int value = rand() % 100 + 1;
        safe_queue.push(value);
        std::cout << "New value is: " << value << std::endl;
    }
}

void consumer() {
    while (true) {
        if (safe_queue.empty()) {
            break;
        }
        std::lock_guard<std::mutex> lockForCout(my_mutex);
        int value;
        safe_queue.wait_and_pop(value);
        bool flag = true;
        int sqrt_val = (int)sqrt(value);
        for (int i = 2; i <= sqrt_val; i++) {
            if (value % i == 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            std::cout << std::this_thread::get_id() << " It is prime: " << value << std::endl;
        } else {
            std::cout << std::this_thread::get_id() << " It is not prime: " << value << std::endl;
        }
    }
}
//--------------------------------------------------------------------------
*/

std::vector<int> _x(N);

int test() {
    std::cout << "Hello!" << std::endl;
    return 4;
}

int producer_work_loop() {
    int sum = 0;
    for(int i = 1; i <= N; i++) {
        _x.push_back(i);
        sum += i;
    }
    return sum;
}

int consumer_work_loop() {
    int sum = 0;
    for(int i = 1; i <= N; i++) {
        while(_x.empty()) {
            std::this_thread::yield();
        }
        sum += _x.back();
        _x.pop_back();
    }
    return sum;
}

int main(int argc, const char * argv[]) {
    
    /*
    //-------------------Producer and Consumers-----------------------------
    std::thread producer_thread(producer);
    producer_thread.join();
    std::vector<std::thread> threads;
    std::vector<thread_guard> threadGuards;
    
    int count = 10;//rand() % 30 + 1;
    for (int i = 0; i < count; i++) {
        //std::thread consumer_thread(consumer);
        threads.emplace_back(std::thread(consumer));
    }
    
    for (int i = 0; i < count; i++) {
        threadGuards.emplace_back(threads[i]);
    }
    */
    clock_t t1,t2;
    
    threadPool<int> thPool(10);
    /*
     std::future<int> asyncResult = thPool.submit(test);
     std::cout << asyncResult.get() << std::endl;
    */
    t1=clock();
    
    std::future<int> produced_sum = thPool.submit(producer_work_loop);
    std::future<int> consumed_sum = thPool.submit(consumer_work_loop);
    
    int producer_sum = produced_sum.get();
    int consumer_sum = consumed_sum.get();
    
    t2=clock();
    float diff ((float)t2-(float)t1);
    std::cout << producer_sum << " " << consumer_sum << " time: " << diff;
    return 0;
}