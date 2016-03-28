//
//  main.cpp
//  prefix-sum
//
//  Created by Андрей Решетников on 04.05.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include "prefix-sum.h"
using namespace std;

const int N = 128;
const int Num_Threads = 4;

template<typename T>
class Add {
public:
    T operator() (const T& x, const T& y) {
        return x + y;
    }
};

int main() {
    vector<int> data(N), summ(N);
    
    for (int i = 1; i < N; i++) {
        data[i] = i;
        summ[i] = summ[i - 1] + i;
    }

    vector<int> test = get_prefix_summ<int, Add<int>>(data, Add<int>(), Num_Threads);
    
    bool flag = true;
    for (int i = 0; i < N; i++) {
        if (summ[i] != test[i]) {
            flag = false;
            break;
        }
    }
    
    if (flag) {
        cout << "All is good";
    } else {
        cout << "All is bad";
    }
    return 0;
    
}
