#ifndef AXLE_ITC_H
#define AXLE_ITC_H

#include <pthread.h>

#define AXLE_ITC_MAX_MESSAGES 10000
#define AXLE_ITC_MAX_LISTENERS 200

typedef struct
{
    char  topic[16];
    void* data;
} itc_msg;

typedef struct
{
    char  topic[16];
    void (*callback)(itc_msg);
} reciever;


void init_itc();
void cleanup_itc();

void add_reciever(const char* id, void (*callback)(itc_msg));
void add_msg(const itc_msg* msg);

#endif
