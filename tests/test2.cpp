#include "threadSafeQueue.h"
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>


constexpr int producer_num = 4;
constexpr int consumer_num = producer_num;
constexpr int iteration = 10000;

int main(){
    thread_safe_queue<int> queue;
    std::atomic_int32_t cnt(0);
    std::vector<std::thread> threads;
    threads.reserve(consumer_num+producer_num);
    for(int i=0;i<producer_num;i++){
        threads.emplace_back([&queue,&cnt](){
            for(int j=0;j<iteration;j++){
                queue.pop_front();
                cnt++;
            }
        });
        threads.emplace_back([&queue](){
            for(int j=0;j<iteration;j++){
                queue.push_back(1);
            }
        });
    }

    for(auto &thread:threads){
        if(thread.joinable())
            thread.join();
    }
    if(cnt==producer_num*iteration)
        return 0;
    else
        return -1;
}