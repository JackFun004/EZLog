#ifndef EZLOG
#define EZLOG

// #define DEBUG
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
    static constexpr Config show_function = 1<<3;//打印消息调用所在函数
    static constexpr Config show_file = 1<<4;//打印所处文件名及行数
    static constexpr Config show_tid = 1<<5;//打印所在线程号

    enum class Level{
        Info,
#ifdef DEBUG
        Debug,
#endif
        Warning,Error,Fatal
    };
    enum class Resolution{
        Second,MilliSecond,MicroSecond
    };
public:
   
    class log{
        friend class EZLog;
        log(Level l,const char* func,const char* file,uint32_t line);
        EZLog::Level level;
        mutable std::stringstream ss;
    public:
        ~log();
        template<typename T>
        const log& operator<<(const T&) const;
    };

    struct log_nothing{
        template<typename T>
        const log_nothing& operator<<(const T&) const{
            return *this;
        };
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
    std::string output_file;
    Resolution time_resolution;
    Config config;
    
public:
    //interfaces
    static void set_output_file(std::string file);
    static void set_time_resolution(Resolution resolution);
    static void set_config(Config config);
    static log ezlog(Level level,const char* func = nullptr,const char* file = nullptr,uint32_t line = 0);
};

template<typename T>
const EZLog::log &EZLog::log::operator<<(const T& message) const
{
    ss<<message;
    return *this;
}
#define EZ_LOG(lv) EZLog::ezlog(lv,__PRETTY_FUNCTION__,__FILE__,__LINE__)
#define EZ_INFO()  EZ_LOG(EZLog::Level::Info)
#ifdef DEBUG
#define EZ_DEBUG() EZ_LOG(EZLog::Level::Debug)
#else
#define EZ_DEBUG() EZLog::log_nothing()
#endif
#define EZ_WARN()  EZ_LOG(EZLog::Level::Warning)
#define EZ_ERR()   EZ_LOG(EZLog::Level::Error)
#define EZ_FATAL() EZ_LOG(EZLog::Level::Fatal)


#endif