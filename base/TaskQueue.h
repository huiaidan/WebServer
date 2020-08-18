#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H
#include<pthread.h>
#include<queue>
#include<boost/function.hpp>
#include<algorithm>
#include<iostream>
#include"MutexLock.h"
#include"Condition.h"
using namespace std;

typedef boost::function<void()> Task;
class TaskQueue
{
public:
	TaskQueue();
	~TaskQueue()
	{}
	void put_into(Task task);//向任务队列中添加任务，即入队操作
	Task put_out();//向任务队列中删除任务，即出队操作；
	int size();//获取任务队列中任务的数量
private:
	int queue_max;
	queue<Task> task_queue;
	MutexLock rwmutex;
	Condition wcond;
	Condition rcond;
};


#endif
