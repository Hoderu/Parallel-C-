//
//  main.cpp
//  spsc_ring_buffer
//
//  Created by Андрей Решетников on 16.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <numeric>
#include <future>
#include "spsc_ring_buffer.h"

#define N 10000000

int producer_work_loop(spsc_ring_buffer<int>& _x) {
    int sum = 0;
    for(int i = 1; i <= N; i++) {
        while (!_x.enqueue(i)) {
            std::this_thread::yield();
        }
        sum += i;
    }
    return sum;
}

int consumer_work_loop(spsc_ring_buffer<int>& _x) {
    int sum = 0, el;
    for(int i = 1; i <= N; i++) {
        while (!_x.dequeue(el)) {
            std::this_thread::yield();
        }
        sum += el;
    }
    return sum;
}

int main(int argc, const char * argv[]) {
    
    clock_t t1,t2;
    
    int capacity = 10;
    spsc_ring_buffer<int> channel(capacity);
    
    t1=clock();
    
    std::future<int> produced_sum = std::async(std::launch::async, producer_work_loop, std::ref(channel));
    std::future<int> consumed_sum = std::async(std::launch::async, consumer_work_loop, std::ref(channel));

    int producer_sum = produced_sum.get();
    int consumer_sum = consumed_sum.get();
    
    t2=clock();
    float diff ((float)t2-(float)t1);
    std::cout << producer_sum << " " << consumer_sum << " time: " << diff;
    
    return 0;
}
