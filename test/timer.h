#ifndef TIMER_H
#define TIMER_H

#define NO_INLINE __attribute__((noinline))

#ifdef __MACH__
#include <mach/mach_time.h>

double ns_conversion_factor;
double us_conversion_factor;
double ms_conversion_factor;

NO_INLINE void timeinit() {
    mach_timebase_info_data_t timebase;
    mach_timebase_info(&timebase);

    ns_conversion_factor = (double)timebase.numer / (double)timebase.denom;
    us_conversion_factor = (double)timebase.numer / (double)timebase.denom / 1000;
    ms_conversion_factor = (double)timebase.numer / (double)timebase.denom / 1000000;
}

NO_INLINE double nsticks() {
    return mach_absolute_time() * ns_conversion_factor;
}

NO_INLINE double msticks() {
    return mach_absolute_time() * ms_conversion_factor;
}

#else
#include <time.h>

void timeinit() {
    /* do nothing */
}

double nsticks() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ((double)ts.tv_sec) / 1000000000 + ((double)ts.tv_nsec);
}

double msticks() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ((double)ts.tv_sec) / 1000 + ((double)ts.tv_nsec) * 1000000;
}

#endif

#endif
