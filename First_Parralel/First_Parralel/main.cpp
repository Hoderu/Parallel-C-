//
//  main.cpp
//  First_Parralel
//
//  Created by Андрей Решетников on 13.02.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <iostream>
#include <thread>

int f(int& a) {
    return a++;
};

class thread_guard {
public:
    thread_guard(std::thread& _t): t(_t){};
    
    ~thread_guard(){
        if (t.joinable()) {
            t.join();
        }
    }
private:
    std::thread &t;
};

int main(int argc, const char * argv[]) {
    /*int a = 1;
    auto c = [=]()->int {
        return a+1;
    };*/
    //thread_local int x;
    std::thread t( []() {
        std::cout<<"Hello world!" << std::endl;
    });
    t.join();
    
    int b = 1;
    std::cout << "Was: " << b << std::endl;
    std::thread t1(f, std::ref(b));
    thread_guard g(t1);
    t1.join();
    std::cout << "Now: " << b << std::endl;
}
