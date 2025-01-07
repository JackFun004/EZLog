#include "threadSafeQueue.h"

template <typename T>
thread_safe_queue<T>::thread_safe_queue()
:head(new node),tail(head.get())
{
}

template <typename T>
void thread_safe_queue<T>::push_back(T&& data)
{ 
    {
        auto p = std::make_unique<node>();
        auto new_tail = p.get();
        std::lock_guard<std::mutex> lock_tail(mtx_tail);
        tail->data = std::move(data);
        tail->next = std::move(p);
        tail = new_tail;
    }
    cv.notify_one();
}

template <typename T>
void thread_safe_queue<T>::push_back(const T &data)
{
    {
        auto p = std::make_unique<node>();
        auto new_tail = p.get();
        std::lock_guard<std::mutex> lock_tail(mtx_tail);
        tail->data = data;
        tail->next = std::move(p);
        tail = new_tail;
    }
    cv.notify_one();
}

template <typename T>
T thread_safe_queue<T>::pop_front()
{
    std::unique_lock<std::mutex> lock_head(mtx_head);
    cv.wait(lock_head,[this](){
        return !this->empty();
    });
    T ret = std::move(head->data);
    head.reset(head->next.release());
    return ret; 
}

template <typename T>
inline bool thread_safe_queue<T>::empty()
{
    // std::lock_guard<std::mutex> lock_tail(mtx_tail);
    return head.get()==tail;
}

template class thread_safe_queue<std::string>;
template class thread_safe_queue<int>;