#ifndef _MUTEXLOCK_H
#define _MUTEXLOCK_H
#include<pthread.h>
#include"error.h"
class MutexLock
{
public:
	MutexLock()
	{
	int ret=pthread_mutex_init(&mutex,NULL);
	if(ret==-1)
		perr_exit("mutex_init error");
	}
	~MutexLock()
	{
	int ret=pthread_mutex_destroy(&mutex);
	if(ret==-1)
		perr_exit("mutex_delete error");
	}

	void lock();
	void unlock();
	pthread_mutex_t* get_pthread_mutex();
private:
	pthread_mutex_t mutex;
};

class MutexLockGuard
{
public:
	explicit MutexLockGuard(MutexLock& mutex)
		: mutex_(mutex)
	{
		mutex_.lock();
	}
	~MutexLockGuard()
	{
		mutex_.unlock();
	}
private:
	MutexLock& mutex_;
};



#endif
