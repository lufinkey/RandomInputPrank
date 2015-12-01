
#pragma once

#include <time.h>
#ifdef _WIN32
	#include <winsock.h>
#else
	#include <sys/time.h>
#endif

#if defined(_WIN32)
typedef int suseconds_t;
struct timezone
{
	int tz_minuteswest; /* Minutes west of GMT.  */
	int tz_dsttime; /* Nonzero if DST is ever in effect.  */
};

struct timeval;
#endif

#if defined(_WIN32)
int gettimeofday(struct timeval*tv, struct timezone*tz);
#endif
