#include "time.h"

int get_current_time()
{
	struct timeval tm;

	gettimeofday(&tm, NULL);

	return tm.tv_usec;
}
