//
//  main.cpp
//  SpinLock
//
//  Created by Андрей Решетников on 06.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <iostream>

#include "spinlocks.h"

const std::size_t NUM_THREADS = 4;
const std::size_t NUM_ITERS_PER_THREAD = 100000;

//using mutex_t = std::mutex;
using mutex_t = mipt_parallel::tas_spinlock;

int main() {
    std::size_t cnt = 0;
    mutex_t mtx;
    
    auto start_time = std::chrono::steady_clock::now();
    
    std::vector<std::thread> threads;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(
                             [&]() {
                                 for (size_t k = 0; k < NUM_ITERS_PER_THREAD; ++k) {
                                     std::lock_guard<mutex_t> lock(mtx);
                                     ++cnt;
                                 }
                             }
                             );
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto end_time = std::chrono::steady_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    std::cout << "cnt = " << cnt << std::endl;
    std::cout << "elapsed: " << time_elapsed.count() << " microseconds" << std::endl;
    
    return 0;
};
