#include "threadSafeQueue.h"
#include<iostream>
template <typename T>
std::atomic_int32_t thread_safe_queue<T>::release_cnt;

template <typename T>
void thread_safe_queue<T>::push_back(const T & data)
{
    {
        auto temp = std::make_unique<node>(data);
        std::lock(mtx_head,mtx_tail);
        std::lock_guard<std::mutex> lock_head(mtx_head,std::adopt_lock);
        std::lock_guard<std::mutex> lock_tail(mtx_tail,std::adopt_lock);
        if(empty()){
            head = std::move(temp);
            tail = head.get();
            cv.notify_one();
        } else {
            tail->next = std::move(temp);
            tail = tail->next.get(); 
        }
    }
    std::cout<<"in:"<<data<<"\n";
    
    
}

template <typename T>
T thread_safe_queue<T>::pop_front()
{
    std::unique_lock<std::mutex> lock(mtx_head);
    cv.wait(lock,[=](){return !empty();});
    T ret = head->data;
    
    auto p = head->next.release();
    head.reset(p);
    
    std::cout<<"out:"<<ret<<"  next:"<<(head?head->data:-1)<<"\n";
    return ret;
}

template <typename T>
inline bool thread_safe_queue<T>::empty()
{
    return !head;
}

template class thread_safe_queue<std::string>;