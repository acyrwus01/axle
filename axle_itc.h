#ifndef AXLE_ITC_H

#ifdef __cplusplus
extern "C" {
#endif

#define AXLE_ITC_H

#include <pthread.h>

#define AXLE_ITC_MAX_MESSAGES 10000
#define AXLE_ITC_MAX_LISTENERS 200


typedef struct
{
    char  topic[16];
    void* data;
} itc_msg;

typedef void (*fcallback_t) (const itc_msg);

typedef struct
{
    char  topic[16];
    void (*callback)(const itc_msg);
} reciever;


void init_itc();
void cleanup_itc();

void add_reciever(const char* id, fcallback_t cb);
void add_msg(const char* id, void* data);

#ifdef __cplusplus
}
#endif

#endif // End of header gaurd
