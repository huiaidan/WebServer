#include <stdlib.h>
#include<stdio.h>
#include <errno.h>
#include"error.h"
void perr_exit(const char *s)
{
	perror(s);
	exit(-1);
}
