#ifndef AXLE_SCHED_H_
#define AXLE_SCHED_H_

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "axle_typedefs.h"

#define NANOSECONDS_PER_SECOND 1000000L

int sched_init();
int sched_cleanup();
void schedule_task(void (*function)(void*), long interval, void* data, bool oneshot);

#endif // AXLE_SCHED_H_
