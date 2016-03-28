//
//  parallel_quick_sort.h
//  parallel_quick_sort
//
//  Created by Андрей Решетников on 15.05.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#ifndef parallel_quick_sort_parallel_quick_sort_h
#define parallel_quick_sort_parallel_quick_sort_h

#include "threadPool.h"
#include <vector>
size_t LIMIT_OF_ARRAY = 10;

template <typename T>
class QuickSort {
private:
    std::vector<T>& _data;
    size_t _left;
    size_t _right;
    threadPool<bool>& _queue;
    
public:
    QuickSort(std::vector<T>& data, size_t left, size_t right, threadPool<bool>& queue): _data(data), _left(left), _right(right), _queue(queue) {}
    
    bool operator()() {
        std::vector<std::future<bool>> future;
        while (true) {
            
            size_t distance = _right - _left;
            if (distance < 2) {
                break;
            } else if (distance < LIMIT_OF_ARRAY) {
                std::sort(_data.begin() + _left, _data.begin() + _right);
                break;
            }
            
            size_t index = (_right + _left) / 2;
            std::swap(*(_data.begin() + _left), *(_data.begin() + index));
            T pivot = *(_data.begin() + _left);
            auto bound = std::partition(_data.begin() + _left, _data.begin() + _right,
                                              [pivot](T el) {
                                                  return el <= pivot;
                                              });
            std::swap(*(bound - 1), *(_data.begin() + _left));
            size_t middle = bound - _data.begin();
            QuickSort<T> left_part(_data, _left, middle - 1, _queue);
            future.push_back(_queue.submit(left_part));
            _left = middle;
        }
        for (auto it = future.rbegin(); it != future.rend(); it++) {
            _queue.wait(*it);
        }
        return true;
    }
};

template<typename T>
void parallel_quick_sort(std::vector<T>& input_array, size_t num_threads = 8) {
    threadPool<bool> queue(num_threads);
    
    QuickSort<T> sort_whole_array(input_array, 0, input_array.size(), queue);
    auto result = queue.submit(sort_whole_array);
    result.get();
    
    std::cout << "\n\nResult array: \n";
    for(size_t i = 0; i < input_array.size(); i++) {
        std::cout << input_array[i] << " ";
    }
}


#endif
