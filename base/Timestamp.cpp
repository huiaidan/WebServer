#include"Timestamp.h"
#include"Data.h"
int64_t Timestamp::now()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int64_t seconds = tv.tv_sec;
	return Timestamp(seconds * STOUS + tv.tv_usec).ustime();
}
void Timestamp::timetostring()
{
}
