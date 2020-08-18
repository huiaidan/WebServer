#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include<sys/time.h>
#include<iostream>
using namespace std;
class Timestamp
{
public:
	Timestamp(int ustime_=0):us_time(ustime_)
	{}
	~Timestamp()
	{}
	int64_t now();
	void timetostring();
	int64_t ustime()
	{
		return us_time;
	}
private:
	int64_t us_time;
};

#endif
