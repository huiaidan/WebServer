#include"TaskQueue.h"
#include<unistd.h>
//typedef void (*Task)(void);
TaskQueue::TaskQueue():
		queue_max(100),
		rwmutex(),
		wcond(rwmutex),
		rcond(rwmutex)
	{}
void TaskQueue::put_into(Task task)
{
	MutexLockGuard lock(this->rwmutex);
	while(this->size()>=this->queue_max){
		this->wcond.wait();}
	task_queue.push(task);
	this->rcond.notify_all();
}

Task TaskQueue::put_out()
{
	MutexLockGuard lock(this->rwmutex);
	while(task_queue.empty()){
		this->rcond.wait();
	}
	Task task_=task_queue.front();
	task_queue.pop();
	this->wcond.notify_all();
	return task_;
}

int TaskQueue::size()
{
	return task_queue.size();
}

