//
//  prefix-sum.h
//  prefix-sum
//
//  Created by Андрей Решетников on 04.05.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#ifndef prefix_sum_prefix_sum_h
#define prefix_sum_prefix_sum_h

#include "Cyclic_Barier.h"
#include "thread_guard.h"
#include <thread>
#include <vector>

using namespace std;

template <typename T, class Add>
void worker(vector<T>* input, vector<T>* output, Add add, Cyclic_Barrier& barrier, int num_iterations, int start_pos, int end_pos) {
    int step = 1;
    for (int i = 0; i < num_iterations; i++, step *= 2) {
        for (int current_position = start_pos; current_position <= end_pos; current_position++) {
            if (current_position < step) {
                output->at(current_position) = input->at(current_position);
            } else {
                output->at(current_position) = add(input->at(current_position), input->at(current_position - step));
            }
        }
        std::swap(input, output);
        barrier.enter();
    }
}

template <typename T, class Add>
void parallel_scan(vector<T>* input, vector<T>* output, Add add, size_t num_threads) {
    
    Cyclic_Barrier barrier(num_threads);
    vector<thread> threads;
    vector<thread_guard> thread_guards;

    int num_iterations = 0;
    for (int i = 2; i <= input->size(); i *= 2, num_iterations++);
    
    int block_size = (int)input->size() / num_threads;
    int start_block = 0;
    int end_block = start_block + block_size - 1;
    
    for (int i = 1; i <= num_threads; i++) {
        threads.emplace_back(worker<int, Add>, input, output, add, std::ref(barrier), num_iterations, start_block, end_block);
        start_block = end_block + 1;
        end_block = start_block + block_size - 1;
    }
    for (int i = 0; i < num_threads; i++) {
        thread_guards.emplace_back(threads[i]);
    }
}

template <typename T, class Add>
vector<T> get_prefix_summ(const vector<T>& data, Add add, size_t num_threads) {
    if (num_threads > data.size()) {
        num_threads = data.size();
    }
    vector<T>* input = new vector<T>(data);
    vector<T>* prefix_sums = new vector<T>(data.size());
    parallel_scan(input, prefix_sums, add, num_threads);
    return *prefix_sums;
}

#endif
