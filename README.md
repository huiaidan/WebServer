1,Introduction

本项目为C++11编写的Web服务器，可正确响应get、head请求，支持静态资源的传输，支持HTTP长连接，支持定时清除超时的连接。

2，Envoirment
OS: Ubuntu 20.04
Complier: g++ 9.3.0

3，Technical points
    采用了multiple reactors+thread pool模式； 
    使用了epoll边沿触发+非阻塞IO方式； 
    实现了固定线程数的工作线程池和IO线程池； 
    实现了基于红黑树的定时器； 
    实现了基于互斥锁mutex和条件变量的任务队列； 
    采用eventfd或条件变量的方法实现线程间的唤醒。
4,Model
并发模型为Multiple Reactors+非阻塞IO+线程池，新连接Round Robin分配
