#ifndef _Condition_H
#define _Condition_H
#include<pthread.h>
#include"MutexLock.h"
#include<iostream>
using namespace std;
class Condition
{
public:
	Condition(MutexLock& mutex_):cmutex(mutex_)
	{
	int ret=pthread_cond_init(&cond,NULL);
	if(ret==-1)
		perr_exit("cond_init error");
	}
	~Condition()
	{
	int ret=pthread_cond_destroy(&cond);
	if(ret==-1)
		perr_exit("cond_delete error");
	}

	void wait();
	void notify();
	void notify_all();
private:
	pthread_cond_t cond;
	MutexLock& cmutex;
};


#endif
