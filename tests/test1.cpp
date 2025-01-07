#include "threadSafeQueue.h"
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

constexpr int consumer_num = 4;
constexpr int producer_num = 4;
constexpr int prodecer_iteration = 10000;
constexpr int consumer_iteration = prodecer_iteration*producer_num/consumer_num;

int main(){
    thread_safe_queue<int> queue;
    std::atomic_int32_t cnt(0);
    std::vector<std::thread> threads;
    threads.reserve(consumer_num+producer_num);
    for(int i=0;i<producer_num;i++){
        threads.emplace_back([&queue](){
            for(int j=0;j<prodecer_iteration;j++){
                queue.push_back(1);
            }
        });
    }
    for(int i=0;i<consumer_num;i++){
        threads.emplace_back([&queue,&cnt](){
            for(int j=0;j<consumer_iteration;j++){
                queue.pop_front();
                cnt++;
            }
        });
    }
    for(auto &thread:threads){
        if(thread.joinable())
            thread.join();
    }
    if(cnt==producer_num*prodecer_iteration)
        return 0;
    else
        return -1;
}