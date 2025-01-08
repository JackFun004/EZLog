#ifndef EZLOG
#define EZLOG
#include "threadSafeQueue.h"
#include<thread>
#include<sstream>
#include<fstream>

class EZLog{
public:
    using Config = std::uint8_t;
    static constexpr Config log_all_on_exit = 1;//ezlog析构前，将消息队列中所有内容输出到目标文件
    static constexpr Config log_console = 1<<1;//消息同时打印到标准输出流
    static constexpr Config console_only = 1<<2;//消息仅打印到标准输出流

    enum class Level{
        Info,Debug,Warning,Error
    };
    enum class Resolution{
        Second,MilliSecond,MicroSecond
    };
public:
   

    class log{
        friend class EZLog;
        log(Level l);
        EZLog::Level level;
        mutable std::stringstream ss;
    public:
        ~log();
        template<typename T>
        const log& operator<<(const T&) const;
    };

private:
    thread_safe_queue<std::string> queue;
    std::thread consumer;
    void task();
    bool check_if_empty_exit;
    
private:
    EZLog();
    ~EZLog();

    EZLog(const EZLog&) = delete;
    EZLog& operator=(const EZLog&) = delete;

    //获取全局单例
    static EZLog& singleton();

    inline static std::string now(Resolution);
private:
    std::ofstream ofs;
    std::mutex f_mutex;
private:
    Level default_level; 
    std::string output_file;
    Resolution time_resolution;
    Config config;
public:
    //interfaces
    static void set_default_level(Level level);
    static void set_output_file(std::string file);
    static void set_time_resolution(Resolution resolution);
    static void set_config(Config config);

    static log ezlog(Level level);
    static log ezlog();
    static log info();
    static log debug();
    static log warning();
    static log error();
};

template<typename T>
const EZLog::log &EZLog::log::operator<<(const T& message) const
{
    ss<<message;
    return *this;
}
#endif