//
//  main.cpp
//  HierarchicalMutex
//
//  Created by Андрей Решетников on 13.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//
#include <iostream>
#include "HierarchicalMutex.h"

void check(bool x) {
    if (!x) {
       throw std::logic_error("Error of level of mutex");
    }
}

int main() {
    HierarchicalMutex a(1);
    HierarchicalMutex b(2);
    HierarchicalMutex c(3);
    
    check(a.lock());
    check(b.lock());
    check(c.lock());
    check(c.unlock());
    check(b.unlock());
    check(a.unlock());
    return 0;
}
