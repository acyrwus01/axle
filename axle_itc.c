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
static pthread_t runningQueueThread;
static int itc_exit = 0;

void* runningQueue(void* arg)
{

    for(;;) {
        pthread_mutex_lock(&mutex);

        for(int i = 0; i < msgQueueSize; ++i)
            for(int j = 0; j < recieverQueueSize; ++j)
                if(!strcmp(recieverQueue[j].topic, msgQueue[i].topic))
                    recieverQueue[j].callback(msgQueue[i]);

        msgQueueSize = 0;

        pthread_mutex_unlock(&mutex);
        if(itc_exit) return 0;

    }
}
/*
** This function will add a message to the running queue.
** The argument is a struct containing the topic and message.
**
** NOTE: A message is consumed once all recievers have recieved the message.
*/
void add_msg(const itc_msg* msg)
{
    pthread_mutex_lock(&mutex);

    printf("Before setting msg queue %d\n", msgQueueSize);
    msgQueue[msgQueueSize++] = *msg;
    printf("After setting msg queue\n");

    pthread_mutex_unlock(&mutex);
}

void add_reciever(const char* id, void (*callback)(itc_msg))
{
    reciever r;

    strcpy(r.topic, id);
    r.callback = callback;

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
    itc_exit = 1;
    pthread_join(runningQueueThread, NULL);
}
