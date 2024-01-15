#include "axle_itc.h"
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

static itc_msg msgQueue[AXLE_ITC_MAX_MESSAGES];
static int msgQueueSize = 0;
static int recieverQueueSize = 0;
static reciever recieverQueue[AXLE_ITC_MAX_LISTENERS];
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_t runningQueueThread;
static int itc_exit = 1;

enum {NOT_RUNNING, RUNNING};

void* runningQueue(void* arg)
{

    while(itc_exit == RUNNING) {
        pthread_mutex_lock(&mutex);

        while (msgQueueSize == 0 && itc_exit == RUNNING)
            pthread_cond_wait(&cond, &mutex);

        for(int i = 0; i < msgQueueSize; ++i)
            for(int j = 0; j < recieverQueueSize; ++j)
                if(!strcmp(recieverQueue[j].topic, msgQueue[i].topic))
                    recieverQueue[j].callback(msgQueue[i]);

        msgQueueSize = 0;

        pthread_mutex_unlock(&mutex);


    }
    return 0;
}
/*
** This function will add a message to the running queue.
** The argument is a struct containing the topic and message.
**
** NOTE: A message is consumed once all recievers have recieved the message.
*/
void add_msg(const char* id, void* data)
{
    itc_msg i;
    strcpy(i.topic, id);
    i.data = data;
    pthread_mutex_lock(&mutex);

    msgQueue[msgQueueSize++] = i;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void add_reciever(const char* id, fcallback_t cb)
{
    reciever r;

    strcpy(r.topic, id);
    r.callback = cb;

    pthread_mutex_lock(&mutex);

    recieverQueue[recieverQueueSize++] = r;

    pthread_mutex_unlock(&mutex);

}

 void init_itc(){
    printf("Init Itc\n");
    // Give both addmsg and runningq a thread */
    pthread_create(&runningQueueThread, NULL, runningQueue, NULL);
}

void cleanup_itc() {
    pthread_mutex_lock(&mutex);
    itc_exit = NOT_RUNNING;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    while(msgQueueSize > 0)
    {
        printf("Size: %d\n", msgQueueSize);
        usleep(1);
    }
    pthread_join(runningQueueThread, NULL);
    printf("All done:) \n");
}
