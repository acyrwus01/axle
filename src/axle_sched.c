#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "axle_typedefs.h"
#include "axle_sched.h"
#include "axle_hmap.h"

#define TICKS_PER_SECOND 60
#define NANOSECONDS_PER_TICK (NANOSECONDS_PER_SECOND / TICKS_PER_SECOND)

// Task list
#define MAX_TASKS 100
#define SECOND_TO_NANOSECOND(n) (n * 1000000L)

// Task structure
typedef struct {
    void (*function)(void*);   // Pointer to function
    long interval;            // Time in microseconds
    void* data;
    struct timespec next_run; // Next scheduled run time
    bool oneshot;
} Task;


Task task_list[MAX_TASKS];
int num_tasks = 0;
enum {NOT_RUNNING, RUNNING};
static u8 sched_exit = RUNNING;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static inline void set_interval(struct timespec* ts, u64 interval){
    ts->tv_sec += interval / 1000000;
    ts->tv_nsec += (interval % 1000000) * 1000;
}
// Function to add tasks
void schedule_task(void (*function)(void*), long interval, void* data, bool oneshot) {
    if (num_tasks >= MAX_TASKS) {
        printf("Maximum number of tasks reached.\n");
        return;
    }
    pthread_mutex_lock(&mutex);
    task_list[num_tasks].function = function;
    task_list[num_tasks].interval = interval;
    task_list[num_tasks].data     = data;
    task_list[num_tasks].oneshot  = oneshot;

    clock_gettime(CLOCK_MONOTONIC, &task_list[num_tasks].next_run);
    task_list[num_tasks].next_run.tv_sec += interval / 1000000;
    task_list[num_tasks].next_run.tv_nsec += (interval % 1000000) * 1000;

    if (task_list[num_tasks].next_run.tv_nsec >= 1000000000) {
        task_list[num_tasks].next_run.tv_sec++;
        task_list[num_tasks].next_run.tv_nsec -= 1000000000;
    }
    
    num_tasks++;
    pthread_mutex_unlock(&mutex);
}

char* get_current_time() {
    // Allocate memory for the formatted time string
    char *buffer = malloc(80 * sizeof(char));
    if (buffer == NULL) {
        // Handle memory allocation error
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL;
    }

    // Get current time
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Format time into the buffer
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}

// Function to run tasks
void* run_sched(void* _arg) {
    while(sched_exit == RUNNING)
    {
        pthread_mutex_lock(&mutex);

        struct timespec current_time;
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        
        for (u8 i = 0; i < num_tasks; ++i) {
            if ((current_time.tv_sec > task_list[i].next_run.tv_sec) ||
                (current_time.tv_sec == task_list[i].next_run.tv_sec && current_time.tv_nsec >= task_list[i].next_run.tv_nsec)) {

                task_list[i].function(task_list[i].data);

                // Schedule next run
                task_list[i].next_run.tv_sec += task_list[i].interval / 1000000;
                task_list[i].next_run.tv_nsec += (task_list[i].interval % 1000000) * 1000;
                if (task_list[i].next_run.tv_nsec >= 1000000000) {
                    task_list[i].next_run.tv_sec++;
                    task_list[i].next_run.tv_nsec -= 1000000000;
                }
            }
        }

            pthread_mutex_unlock(&mutex);
    }
    return (0);
}
// Example task function
static void task_one(void* data) {
    printf("Task executed at %s \n", get_current_time());
    printf("My data: %d\n", (int)data);
}

void task_two(void* data) {
    printf("Task executed at %s \n", get_current_time());
    printf("My data: %d\n", (int)data);
}

void task_three(void* data) {
    printf("Task executed at %s \n", get_current_time());
    printf("My data: %d\n", (int)data);
}

void task_oneshot(void* data) {
    printf("Task executed at %s \n", get_current_time());
    printf("My data: %d\n", (int)data);
}

static pthread_t sched_thread;

int sched_init()
{
    return pthread_create(&sched_thread, NULL, run_sched, NULL);
}

unsigned int get_concurrency() {
    long n = sysconf(_SC_NPROCESSORS_ONLN);
    return n > 0 ? (unsigned int)n : 1;
}

int sched_cleanup()
{
    pthread_mutex_lock(&mutex);
    sched_exit = NOT_RUNNING;
    pthread_mutex_unlock(&mutex);
    pthread_join(sched_thread, NULL);

    return 0;
}

void test(Task tk)
{
    printf("My data from tk: %d\n", (int)tk.data);
}

int mainnn() {
    printf("Number of concurrent threads supported: %u\n", get_concurrency());

    sched_init();

    /* test((Task) { */
    /*     .function = task_one, */
    /*     .interval = 32488, */
    /*     .data = (void*)12, */
    /*     .next_run = {0}, */
    /*     .oneshot = false, */
    /* }); */

    schedule_task(task_one, 1 * NANOSECONDS_PER_SECOND, (void*)1, false);
    schedule_task(task_oneshot, 2 * NANOSECONDS_PER_SECOND, (void*)2, true);
    schedule_task(task_two, 1 * NANOSECONDS_PER_SECOND, (void*)3, false);
    schedule_task(task_three, 4 * NANOSECONDS_PER_SECOND, (void*)4, false);
    while(1){};


    return 0;
}
