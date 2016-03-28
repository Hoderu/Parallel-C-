//
//  main.cpp
//  concurrent_heap
//
//  Created by Андрей Решетников on 23.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <iostream>
#include "concurrent_heap.h"
#include "rwlock.h"

int main(int argc, const char * argv[]) {

    concurrent_heap<int, concurrent_ops::inserts> heap(32);
    heap.insert(5);
    heap.insert(6);
    heap.insert(7);
    heap.insert(4);
    heap.insert(3);
    std::cout << "Hello, World!\n" << heap.extract_min();
    std::cout << heap.extract_min() << heap.extract_min();
    return 0;
}
