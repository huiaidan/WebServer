#include"Condition.h"
#include"error.h"

void Condition::wait()
{
	int ret=pthread_cond_wait(&this->cond,this->cmutex.get_pthread_mutex());
	if(ret==-1)
		perr_exit("wait error");
}

void Condition::notify()
{
	int ret=pthread_cond_signal(&this->cond);
	if(ret==-1)
		perr_exit("notify error");
}

void Condition::notify_all()
{
	int ret=pthread_cond_broadcast(&this->cond);
	if(ret==-1)
		perr_exit("notify_all error");
}


