//
//  lock-free-queue.h
//  lock-free-queue
//
//  Created by Андрей Решетников on 20.04.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#ifndef lock_free_queue_lock_free_queue_h
#define lock_free_queue_lock_free_queue_h

template<typename T>
class lock_free_queue {
private:
    
    struct node;
    
    struct counted_node_ptr {
        int external_count;
        node* ptr;
    };
    
    //на многих платформах хранение структуры в одном машинном слове повышает шансы на то, что атомарные операции окажутся свободными от блокировок
    struct node_counter {
        unsigned internal_count:30; // помещается в машинное слово и значение может быть велико
        unsigned external_counters:2; // внешних счетчиков может быть не более двух
    };
    
    struct node {
        std::atomic<T*> data;
        std::atomic<node_counter> count;
        std::atomic<counted_node_ptr> next;
        
        void release_ref() {
            node_counter old_counter = count.load(std::memory_order_relaxed);
            node_counter new_counter;
            do {
                new_counter = old_counter;
                --new_counter.internal_count;
            }
            while (!count.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed));
            //атомарно обновляем всю структуру count, хотим модифицировать поле internal_count
            
            //если после уменьшения internal_count оказалось, что и внутренний и внешний счетчик равны нулю, то это была последняя ссылка, и можно удалить узел
            if (!new_counter.internal_count && !new_counter.external_counters) {
                delete this;
            }
        }
        
        node() {
            node_counter new_count;
            new_count.internal_count = 0;
            new_count.external_counters = 2; // сразу есть две ссылки из tail и из указателя next
            count.store(new_count);
            struct counted_node_ptr x;
            x.ptr = nullptr;
            x.external_count = 0;
            next.store(x);
        }
    };
    
    std::atomic<counted_node_ptr> head;
    std::atomic<counted_node_ptr> tail;
    
    static void increase_external_count(std::atomic<counted_node_ptr>& counter,
                                        counted_node_ptr& old_counter) {
        counted_node_ptr new_counter;
        do {
            new_counter = old_counter;
            ++new_counter.external_count;
        } while (!counter.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire,
                                                  std::memory_order_relaxed));
        //получает новую ссылку и увеличиваем внешний счетчик
        old_counter.external_count = new_counter.external_count;
    }
    
    static void free_external_counter(counted_node_ptr& old_node_ptr) {
        node* const ptr = old_node_ptr.ptr;
        int const count_increase = old_node_ptr.external_count - 2;
        node_counter old_counter = ptr->count.load(std::memory_order_relaxed);
        node_counter new_counter;
        do {
            new_counter = old_counter;
            --new_counter.external_counters;
            new_counter.internal_count += count_increase;
            //обновляет оба счетчика для всей структуры count
        } while (!ptr->count.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire,
                                                     std::memory_order_relaxed));
        //если оба значения равны нулю, значит ссылок на узел не осталось, поэтому его можно удалять
        //оба обновления необходимо выполнять в одном действии, чтобы избежать гонки
        if (!new_counter.internal_count && !new_counter.external_counters) {
            delete ptr;
        }
    }
    
    void set_new_tail(counted_node_ptr& old_tail, counted_node_ptr const& new_tail) {
        node* const current_tail_ptr = old_tail.ptr;
        while(!tail.compare_exchange_weak(old_tail, new_tail) && old_tail.ptr == current_tail_ptr);
        // если другие потоки пытаются поместить в очередь новый узел с помощью push, то значение external_count может измениться
        if (old_tail.ptr == current_tail_ptr) { // если ptr не изменился после выхода из цикла, то мы успешно установили tail
            free_external_counter(old_tail);
        } else { // если ptr изменился, то счетчик уже освобожден другим потоком
            current_tail_ptr -> release_ref();
        }
    }
    
public:
    lock_free_queue() {
        head.store({0, new node});
        tail.store({0, new node});
    }
    
    lock_free_queue(const lock_free_queue& other)=delete;
    lock_free_queue& operator=(const lock_free_queue& other)=delete;
    
    std::unique_ptr<T> pop() {
        counted_node_ptr old_head = head.load(std::memory_order_relaxed);
        for(;;) {
            increase_external_count(head, old_head);
            node* const ptr = old_head.ptr;
            if (ptr == tail.load().ptr) { //если узел head совпадает с tail то можно вернуть нулевой указатель
                return std::unique_ptr<T>();
            }
            counted_node_ptr next = ptr->next.load(); // упорядочение по умолчанию
            if (head.compare_exchange_strong(old_head, next)) { // если же в очереди есть данные
                //мы получили в свое распоряжение данные в узле
                T* const res = ptr->data.exchange(nullptr);
                free_external_counter(old_head);
                //возвращаем данные вызывающей программе, освободив внешний счетчик ссылок на извлеченный узел
                return std::unique_ptr<T>(res);
            }
            ptr->release_ref();
        }
    }
    
    void push(T new_value) {
        std::unique_ptr<T> new_data(new T(new_value));
        counted_node_ptr new_next;
        new_next.ptr = new node;
        new_next.external_count = 1;
        counted_node_ptr old_tail = tail.load();
        
        for(;;) {
            increase_external_count(tail, old_tail);
            T* old_data = nullptr;
            // если указатель data действительно установлен
            if (old_tail.ptr -> data.compare_exchange_strong(old_data, new_data.get())) {
                counted_node_ptr old_next = {0};
                if (!old_tail.ptr->next.compare_exchange_strong(old_next, new_next)) { // чтобы избежать цикла
                    delete new_next.ptr; // если обмен неудачный, и нам ни к чему узел выделенный вначале
                    new_next = old_next; // используем next установленный другим потоком до обновления tail
                }
                set_new_tail(old_tail, new_next);
                new_data.release();
                break;
            } else { // один поток помогает другому
                counted_node_ptr old_next = {0};
                if (old_tail.ptr->next.compare_exchange_strong(old_next, new_next)) { // пытаемся записать в next указатель на новый узед, выделенный в этом потоке
                    old_next = new_next; // если получилось, то выделенный узел будет использоваться как новый узел tail
                    new_next.ptr = new node;// следует выделить еще один узел, поместить данные в очередь еще только предстоит
                }
                set_new_tail(old_tail, old_next); // попытаемся установить узел tail до перехода к очередной итерации
            }
        }
    }
};

#endif
