//
//  main.cpp
//  Peterson_mutex
//
//  Created by Андрей Решетников on 13.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <iostream>
#include <thread>

#include "peterson_mutex.h"

using mipt_parallel::peterson_mutex;

const std::size_t NUM_ITERS = 10000000;

int main() {
    std::size_t cnt = 0;
    peterson_mutex mtx;
    
    std::thread first_thread(
                             [&]() {
                                 for (size_t i = 0; i < NUM_ITERS; ++i) {
                                     mtx.first_thread_lock();
                                     ++cnt;
                                     mtx.first_thread_unlock();
                                 }
                             }
                             );
    
    std::thread second_thread(
                              [&]() {
                                  for (size_t i = 0; i < NUM_ITERS; ++i) {
                                      mtx.second_thread_lock();
                                      ++cnt;
                                      mtx.second_thread_unlock();
                                  }
                              }
                              );
    
    first_thread.join();
    second_thread.join();
    
    std::cout << "cnt = " << cnt << std::endl;
    
    return 0;
}