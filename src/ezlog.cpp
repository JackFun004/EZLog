#include "ezlog.h"
#include<chrono>
#include<ctime>
#include<functional>
#include<iostream>

void EZLog::task()
{
    while(true){
        std::string message =  queue.pop_front();
        if(check_if_empty_exit&&queue.empty())
            return;
        if(config&console_only){
            std::cout<<message<<"\n";
        } else {
            if(config&log_console)
                std::cout<<message<<"\n";
            {
                std::lock_guard<std::mutex> lock(f_mutex);
                //文件输出流关闭，任务结束
                if(!ofs.is_open())
                    return;
                ofs<<message<<"\n";
            }
        }
    }
}

EZLog::EZLog()
:check_if_empty_exit(false),
default_level(EZLog::Level::Info),
time_resolution(EZLog::Resolution::MilliSecond),
config(0)
{
    //根据时间创建日志文件
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    tm* localTime = std::localtime(&now_c);
    std::stringstream ss;
    ss<<(localTime->tm_year+1900)<<"-"<<(localTime->tm_mon+1)<<"-"<<localTime->tm_mday<<".log";
    output_file = ss.str();
    ofs.open(output_file,std::ios_base::out|std::ios_base::app);

    //启动写入线程
    consumer = std::thread(std::bind(&EZLog::task,this));

}

EZLog::~EZLog()
{
    if(config&log_all_on_exit){

        check_if_empty_exit = true;
        queue.push_back("ignore");//队列无消息，写入线程pop会阻塞，故有此操作
        if(consumer.joinable())
            consumer.join();
        {
            std::lock_guard<std::mutex> lock(f_mutex);
            if(ofs.is_open())
                ofs.close();
        }
    } else {
        {
            std::lock_guard<std::mutex> lock(f_mutex);
            if(ofs.is_open())
                ofs.close();
        }
        queue.push_back("ignore");//队列无消息，写入线程pop会阻塞，故有此操作
        if(consumer.joinable())
            consumer.join();
    }
    
}

EZLog &EZLog::singleton()
{
    static EZLog obj;
    return obj;
}

inline std::string EZLog::now(EZLog::Resolution res)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    tm* localTime = std::localtime(&now_c);
    std::stringstream ss;
    ss<<localTime->tm_hour<<":"<<localTime->tm_min<<":"<<localTime->tm_sec;
    switch (res)
    {
    case EZLog::Resolution::Second:
        break;
    case EZLog::Resolution::MilliSecond:{
        auto cur_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        ss<<'.'<<cur_ms.time_since_epoch().count()%1000;
        break;
    }
    case EZLog::Resolution::MicroSecond:{
        auto cur_micro = std::chrono::time_point_cast<std::chrono::microseconds>(now);
        ss<<'.'<<cur_micro.time_since_epoch().count()%1000000;
        break;
    }   
    default:
        break;
    }
    return ss.str();
}


void EZLog::set_default_level(Level level)
{
    singleton().default_level = level;
}

void EZLog::set_output_file(std::string file)
{
    EZLog& obj = singleton();
    if(file==obj.output_file)
        return;
    std::ofstream n_ofs(file,std::ios_base::out|std::ios_base::app);
    if(!n_ofs){
        std::cerr<<"specified file "<<file<<" open failed\n";
        return;
    }
    std::lock_guard<std::mutex> lock(obj.f_mutex);
    if(obj.ofs.is_open())
        obj.ofs.close();
    obj.ofs = std::move(n_ofs);

}

void EZLog::set_time_resolution(Resolution resolution)
{
    singleton().time_resolution = resolution;
}

void EZLog::set_config(Config config)
{
    singleton().config = config;
}

EZLog::log EZLog::ezlog(Level level)
{
    return log(level);
}

EZLog::log EZLog::ezlog()
{
    return log(singleton().default_level);
}

EZLog::log EZLog::info()
{
    return log(EZLog::Level::Info);
}

EZLog::log EZLog::debug()
{
    return log(EZLog::Level::Debug);
}

EZLog::log EZLog::warning()
{
    return log(EZLog::Level::Warning);
}

EZLog::log EZLog::error()
{
    return log(EZLog::Level::Error);
}

EZLog::log::log(EZLog::Level l)
:level(l)
{
    std::string pre_msg = now(singleton().time_resolution);
    switch (level)
    {
    case Level::Info:
        pre_msg += "[info]";
        break;
    case Level::Debug:
        pre_msg += "[debug]";
        break;
    case Level::Warning:
        pre_msg += "[warning]";
        break;
    case Level::Error:
        pre_msg += "[error]";
        break;
    default:
        pre_msg += "[unknown]";
        break;
    }
    ss<<pre_msg;
    
}
EZLog::log::~log()
{
    singleton().queue.push_back(ss.str());
}
