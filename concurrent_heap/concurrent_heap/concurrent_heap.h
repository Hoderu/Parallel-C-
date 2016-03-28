//
//  concurrent_heap.h
//  concurrent_heap
//
//  Created by Андрей Решетников on 23.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#ifndef concurrent_heap_concurrent_heap_h
#define concurrent_heap_concurrent_heap_h

#include <mutex>
#include <vector>
#include <bitset>
#include <string>
#include <tuple>
#include "rwlock.h"

enum class concurrent_ops { inserts, extract_mins };
using mutex_t = std::mutex;
class Overflow_Exception : public std::exception {};
class Empty_Heap_Exception : public std::exception {};

template<typename T, concurrent_ops M>
class concurrent_heap {
private:
    struct node{
        mutex_t spinlock;
        T data;
        bool priority;
    };
    std::vector<node> _heap;
    int _capacity, _size;
    std::condition_variable _overflow;
    mutex_t heap_mutex;
    
    int rev(int x) {
        std::string s = std::bitset<16>(x).to_string();
        std::reverse(s.begin() + s.find("1") + 1, s.end());
        return (int)std::bitset<16>(s).to_ulong();
    }
    int get_left_child(int index) {
        return 2 * index;
    }
    int get_right_child(int index) {
        return 2 * index + 1;
    }
    int get_parent(int index) {
        return index / 2;
    }
public:
    concurrent_heap(int capacity): _capacity(capacity), _size(0), _heap(capacity + 1) {
        for(auto i = _heap.begin(); i != _heap.end(); i++) {
            (*i).priority = false;
        }
    }
    
    void insert(T key) {
        rwlock rwl;
        if (M == concurrent_ops::inserts) {
            rwl.reader();
        } else {
            rwl.writer();
        }
        
        heap_mutex.lock();
        if (_size == _capacity) {
            heap_mutex.unlock();
            throw Overflow_Exception();
        }
        
        ++_size;
        std::atomic<int> index(_size);
        
        //index = rev(index);
        
        std::unique_lock<mutex_t> ulk_current_vertex(_heap[index].spinlock);
        _heap[index].data = key;
        _heap[index].priority = true;
        
        heap_mutex.unlock();
        
        
        while (index != 1) {
            std::unique_lock<mutex_t> ulk_parent_vertex(_heap[get_parent(index)].spinlock);
            if ( _heap[index].data < _heap[get_parent(index)].data ) {
                std::swap(_heap[index].data, _heap[get_parent(index)].data);
                ulk_current_vertex.unlock();
                ulk_current_vertex = std::move(ulk_parent_vertex);
                index = get_parent(index);
            } else {
                ulk_current_vertex.unlock();
                ulk_parent_vertex.unlock();
                return;
            }
        }
        ulk_current_vertex.unlock();
    }
    
    T extract_min() {
        rwlock rwl;
        if (M == concurrent_ops::inserts) {
            rwl.writer();
        } else {
            rwl.reader();
        }
        heap_mutex.lock();
        if (_size == 0) {
            heap_mutex.unlock();
            throw Empty_Heap_Exception();
        }
        heap_mutex.unlock();
        
        std::atomic<int> index(1);
        std::atomic<int> last;
        int count = 0, i = 1;
        while (count < _size) {
            if (_heap[i].priority == true) {
                count++;
            }
            i++;
        }
        last = i - 1;
        
        std::unique_lock<mutex_t> ulk_last_vertex(_heap[last].spinlock);
        std::unique_lock<mutex_t> ulk_parent_vertex(_heap[index].spinlock);
        T answer = _heap[index].data;
        _heap[index].data = _heap[last].data;
        _size--;
        _heap[last].priority = false;
        ulk_last_vertex.unlock();
        
        while (index <= _size / 2) {
            std::unique_lock<mutex_t> ulk_left_vertex(_heap[get_left_child(index)].spinlock);
            std::unique_lock<mutex_t> ulk_right_vertex(_heap[get_right_child(index)].spinlock);
            if (get_left_child(index) == _size) {
                if (_heap[get_left_child(index)].data < _heap[index].data) {
                    std::swap(_heap[2*index].data, _heap[index].data);
                    ulk_parent_vertex.unlock();
                    ulk_parent_vertex = std::move(ulk_left_vertex);
                    index = get_left_child(index);
                } else {
                    ulk_left_vertex.unlock();
                    ulk_right_vertex.unlock();
                    break;
                }
            } else if (_heap[get_left_child(index)].data < _heap[get_right_child(index)].data) {
                if (_heap[get_left_child(index)].data < _heap[index].data) {
                    std::swap(_heap[get_left_child(index)].data, _heap[index].data);
                    ulk_parent_vertex.unlock();
                    ulk_parent_vertex = std::move(ulk_left_vertex);
                    index = get_left_child(index);
                } else {
                    ulk_left_vertex.unlock();
                    ulk_right_vertex.unlock();
                    break;
                }
            } else if (_heap[get_right_child(index)].data < _heap[index].data) {
                std::swap(_heap[get_right_child(index)].data, _heap[index].data);
                ulk_parent_vertex.unlock();
                ulk_parent_vertex = std::move(ulk_right_vertex);
                index = get_right_child(index);
            } else {
                ulk_left_vertex.unlock();
                ulk_right_vertex.unlock();
                break;
            }
        }
        ulk_parent_vertex.unlock();
        return answer;
    }
};

#endif
