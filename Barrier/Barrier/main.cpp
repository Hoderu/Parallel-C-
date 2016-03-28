//
//  main.cpp
//  Barrier
//
//  Created by Андрей Решетников on 10.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <iostream>
#include <thread>
#include <vector>
#include "Cyclic_Barrier.h"

int count = 10;
Cyclic_Barrier cyclic_Barrier(count);
std::mutex mtx;

void work() {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    for (int i = 0; i < 10; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "Wait for others: " << std::this_thread::get_id() << std::endl;
        lock.unlock();
        cyclic_Barrier.enter();
        lock.lock();
        std::cout << "Done: " << std::this_thread::get_id() << std::endl;
        lock.unlock();
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < count; i++) {
        threads.emplace_back(std::thread(&work));
    }
    for (int i = 0; i < count; i++) {
        if (threads[i].joinable()) {
            threads[i].join();
        }
    }
    return 0;
}
