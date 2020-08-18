#include"MutexLock.h"
#include<iostream>
using namespace std;
void MutexLock::lock()
{
	int ret=pthread_mutex_lock(&this->mutex);
	if(ret==-1)
		perr_exit("lock error");
}

void MutexLock::unlock()
{
	int ret=pthread_mutex_unlock(&this->mutex);
	if(ret==-1)
		perr_exit("unlock error");
}


pthread_mutex_t*  MutexLock::get_pthread_mutex()
{
	return &mutex;
}
