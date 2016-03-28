//
//  main.cpp
//  tatas-spinlock
//
//  Created by Андрей Решетников on 17.04.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <iostream>
#include <future>
#include <vector>
#include "tas_spinlock.h"
#include "tatas_spinlock.h"

#define K 10000000

int increment() {
    tas_spinlock tasp;
    int j = 0;
    for(int i = 0; i < K; i++) {
        tasp.lock();
        j++;
        tasp.unlock();
    }
    return j;
}

int main(int argc, const char * argv[]) {
    
    for (int n = 1; n < 11; n++) {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::future<int>> threads;
        threads.reserve(n);
        for (int i = 0; i < n; i++) {
            threads.push_back(std::async(increment));
        }
        for (int i = 0; i < n; i++) {
            std::cout << threads[i].get() << std::endl;
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "time for " << n << " threads = " << diff.count() << std::endl << std::endl;
        }
    return 0;
}
