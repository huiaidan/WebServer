#include"EventLoop.h"
#include<thread>
#include<sys/types.h>
#include<sys/eventfd.h>
#include"Channel.h"
#include"Acceptor.h"
#include"Server.h"
using namespace std;
typedef void(*func)(void);
__thread EventLoop* loopInThisThread = 0;
MutexLock EventLoop::mutex_test;
EventLoop::EventLoop():
        threadId_(gettid()),
	channel(new Channel(this)),
	wakeupfd(eventfd(0,EFD_NONBLOCK)),
	callingPendingFunctors_(false)
	{
		if (loopInThisThread)
		{
			cout<<"重复创建EventLoop"<<endl;
		}
		else
		{
			loopInThisThread = this;
		}
	}

EventLoop::~EventLoop()
{
	loopInThisThread=0;
}

//循环监听
void EventLoop::loop()
{
	mutex_test.lock();
	shared_ptr<TcpConnection> conn= Server(this).newfd_conn(this->wakeupfd);
	this->channel->tfd_event_init(conn);
	mutex_test.unlock();
	while(1){
		this->channel->do_event_array();
		doPendingFunctors();
	}
}

//确保cb不在其他线程中被调用
void EventLoop::runInLoop(const func& cb)
{
	if (isInLoopThread())
	{			        // 如果是当前IO线程调用runInLoop，则同步调用cb//
		cb();
	}
	else
	{
		queueInLoop(cb);
	}
}

//将cb加入线程的函数队列
void EventLoop::queueInLoop(const func& cb)
{
	{
		mutex_.lock();
		functor.push_back(cb);
		mutex_.unlock();
	}
	if (!isInLoopThread() || callingPendingFunctors_)
	{
		wakeup();
	}
}

//唤醒应该调用cb函数的线程，使其即使调用cb
void EventLoop::wakeup()
{
	uint64_t one = 1;//占8字节
	ssize_t n = write(wakeupfd, &one, sizeof one);
	if (n != sizeof one)
	{
		cout << "EventLoop::wakeup() writes " << n << " bytes instead of 8"<<endl;
	}
}

//处理线程函数队列中的函数
void EventLoop::doPendingFunctors()//处理functor数组中的函数
{
	std::vector<func> functors;
	callingPendingFunctors_ = true;
	{
		mutex_.lock();
		functors.swap(functor);
		mutex_.unlock();
	}

	for (size_t i = 0; i < functors.size(); ++i)
	{
		functors[i]();
	}
	callingPendingFunctors_ = false;
}
