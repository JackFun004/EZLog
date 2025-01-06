# EZLog
## 简介
> 一个简单的支持多线程的日志库，默认写入当天的日志文件，yyyy-mm-dd.log。可自行指定文件，日志时间精度，日志级别等
## 用法
> 引入头文件，用法如
>> info()<<"message1"<<"message2";
>> ezlog(Level::Debug)<<"debug message";
