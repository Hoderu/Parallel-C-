//
//  Cyclic_Barrier.h
//  Barrier
//
//  Created by Андрей Решетников on 10.03.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//
#include <atomic>
#include <condition_variable>
#include <mutex>


class Cyclic_Barrier {
private:
    int count, epoch;
    std::condition_variable epoch_changed;
    std::mutex mtx;
    std::atomic_int count_of_thread_which_are_waiting;
public:
    Cyclic_Barrier(int _count) {
        count = _count;
        count_of_thread_which_are_waiting = 0;
        epoch = 0;
    }
    /*
     all_arrived - все дошли до барьера
     
     all_passed - все прошли барьер
     
     while(!all_passed)
     
     while(not_arrived)
     
     все потоки пришли и подошли к барьеру - щелкает all_arrived и все потоки выходят
     когда последний проснулся - он щелкает all_passed используем счетчики
     
     второе решение
     завести счетчик epoch - текущая итерация барьера
     он увеличивается когда количество потоков пришедших к барьеру равно N
     arrived = N
     поток увидел что arrived < N, допустим он последний он прошел барьер и ждет новую эпоху, а все остальные ждут другую
     нужно сохранить старую эпоху
     */
    void enter() {
        std::unique_lock<std::mutex> lock(mtx);
        count_of_thread_which_are_waiting++;
        if (count_of_thread_which_are_waiting < count) {
            int local_epoch = epoch;
            while (local_epoch == epoch) {
                epoch_changed.wait(lock);
            }
        } else {
            count_of_thread_which_are_waiting -= count;
            epoch++;
            epoch_changed.notify_all();
        }
    }
};
