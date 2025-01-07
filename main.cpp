#include "ezlog.h"
#include <string>
#include<iostream>
int main(){

    std::thread thread1([=](){
        for(int i=0;i<100;i++){
            EZLog::info()<<"info "<<i;
        }
    });
    std::thread thread2([=](){
        for(int i=0;i<100;i++){
            EZLog::debug()<<"debug "<<i;
        }
    });
    std::thread thread3([=](){
        for(int i=0;i<100;i++){
            EZLog::error()<<"error "<<i;
        }
    });

    thread1.join();
    thread2.join();
    thread3.join();

    EZLog::set_default_level(EZLog::Level::Debug);
    EZLog::set_output_file("specified.log");
    EZLog::set_time_resolution(EZLog::Resolution::MicroSecond);
    std::thread thread4([=](){
        for(int i=0;i<100;i++){
            EZLog::ezlog()<<"info "<<i;
        }
    });
    std::thread thread5([=](){
        for(int i=0;i<100;i++){
            EZLog::ezlog(EZLog::Level::Warning)<<"warning "<<i;
        }
    });
    std::thread thread6([=](){
        for(int i=0;i<100;i++){
            EZLog::error()<<"error "<<i;
        }
    });

    thread4.join();
    thread5.join();
    thread6.join();

    return 0;
}