//
//  main.cpp
//  parallel_quick_sort
//
//  Created by Андрей Решетников on 15.05.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <iostream>
#include "parallel_quick_sort.h"
#pragma comment(linker, "/STACK:66777216")

int main(int argc, const char * argv[]) {
    int N = 10;
    std::vector<int> _input(N);
    
    std::cout << "Input array: \n";
    for(int i = 0; i < N; i++) {
        _input[i] = N - i;
        std::cout << N - i << " ";
    }
    
    parallel_quick_sort(_input);
    
    //std::cout << "Hello, World!\n";
    return 0;
}
