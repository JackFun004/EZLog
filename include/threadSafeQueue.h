#ifndef THREAD_SAFE_QUEUE
#define THREAD_SAFE_QUEUE
#include<atomic>
#include<mutex>
#include<condition_variable>
#include<memory>
template<typename T>
class thread_safe_queue{

    
    struct node{
        T data;
        std::unique_ptr<node> next;
        node(const T& data):data(data){};
        ~node(){release_cnt++;};
    };
    std::unique_ptr<node> head;
    node * tail;

    std::mutex mtx_tail;
    std::mutex mtx_head;
    std::condition_variable cv;
public:
    static std::atomic_int32_t release_cnt;

    thread_safe_queue() = default;

    thread_safe_queue(const thread_safe_queue&) = delete;
    thread_safe_queue& operator=(const thread_safe_queue&) = delete;

    void push_back(const T&);
    T pop_front();
    bool empty();
};

#endif