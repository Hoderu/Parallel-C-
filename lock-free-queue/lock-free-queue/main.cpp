//
//  main.cpp
//  lock-free-queue
//
//  Created by Андрей Решетников on 20.04.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <iostream>
#include "lock-free-queue.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    lock_free_queue<int> q;
    q.push(1);
    q.push(2);
    
    std::unique_ptr<int> a = q.pop();
    std::cout << *(a.get());
    
    a = q.pop();
    std::cout << *(a.get());
    
    std::cout << "Hello, World!\n";
    return 0;
}
