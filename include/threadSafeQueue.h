#ifndef THREAD_SAFE_QUEUE
#define THREAD_SAFE_QUEUE
#include<mutex>
#include<condition_variable>
#include<memory>
template<typename T>
class thread_safe_queue{
    
    struct node{
        T data;
        std::unique_ptr<node> next;
    };
    std::unique_ptr<node> head;
    node * tail;

    std::mutex mtx_tail;
    std::mutex mtx_head;
    std::condition_variable cv;
public:
    thread_safe_queue();

    thread_safe_queue(const thread_safe_queue&) = delete;
    thread_safe_queue& operator=(const thread_safe_queue&) = delete;

    void push_back(T&&);
    void push_back(const T&);
    T pop_front();
    bool empty();
};

#endif