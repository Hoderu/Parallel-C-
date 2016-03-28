//
//  spsc_ring_buffer.h
//  spsc_ring_buffer
//
//  Created by Андрей Решетников on 16.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#ifndef spsc_ring_buffer_spsc_ring_buffer_h
#define spsc_ring_buffer_spsc_ring_buffer_h

#include <vector>
#define CACHE_LINE_SIZE 64

template <typename T>
class spsc_ring_buffer {
private:
    struct node_t {
        T element;
        char cache_line[CACHE_LINE_SIZE];
    };
    std::vector<node_t> _buffer;
    
    std::atomic<unsigned int> _head;
    char fake_buf[CACHE_LINE_SIZE];
    std::atomic<unsigned int> _tail;
    
    unsigned int _capacity;
    
    unsigned int next(unsigned int x) {
        return (x + 1) % (_capacity + 1);
    }
    
public:
    spsc_ring_buffer(unsigned int capacity) {
        _buffer.resize(capacity);
        _head = 0;
        _tail = 0;
        _capacity = capacity;
    }
    bool enqueue(T e) {
        unsigned int tail = _tail.load(std::memory_order_relaxed);
        unsigned int head = _head.load(std::memory_order_acquire);
        if (next(tail) == head) {
            return false;
        }
        _buffer[tail].element = e;
        _tail.store(next(tail), std::memory_order_release);
        return true;
    }
    bool dequeue(T& e) {
        unsigned int tail = _tail.load(std::memory_order_acquire);
        unsigned int head = _head.load(std::memory_order_relaxed);
        if (head == tail) {
            return false;
        }
        e = _buffer[head].element;
        _head.store(next(head), std::memory_order_release);
        return true;
    }
};

#endif
