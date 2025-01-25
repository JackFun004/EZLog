#include "ezlog.h"
#include <string>
#include<iostream>
int main(){

    std::thread thread1([=](){
        for(int i=0;i<1000;i++){
            EZ_INFO()<<"info "<<i;
        }
    });
    std::thread thread2([=](){
        for(int i=0;i<1000;i++){
            EZ_DEBUG()<<"debug "<<i;
        }
    });
    std::thread thread3([=](){
        for(int i=0;i<1000;i++){
            EZ_ERR()<<"error "<<i;
        }
    });

    thread1.join();
    thread2.join();
    thread3.join();

    EZLog::set_output_file("specified.log");
    EZLog::set_time_resolution(EZLog::Resolution::MicroSecond);

    std::thread thread4([=](){
        for(int i=0;i<1000;i++){
            EZ_INFO()<<"info "<<i;
        }
    });
    std::thread thread5([=](){
        for(int i=0;i<1000;i++){
            EZ_DEBUG()<<"debug "<<i;
        }
    });
    std::thread thread6([=](){
        for(int i=0;i<1000;i++){
            EZ_ERR()<<"error "<<i;
        }
    });

    EZ_FATAL()<<"a fatal occurs and makes the system crashed";

    std::cout<<"after_fatal";

    thread4.join();
    thread5.join();
    thread6.join();

    return 0;
}