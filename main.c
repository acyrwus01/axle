#include "stb_ds.h"
#include "axle_itc.h"
#include "axle_hmap.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void testCallback(itc_msg i);
void testCallback1(itc_msg i);

typedef struct data_t {char c[64];} data_t;
typedef struct imap {char *key; int value; } imap;
int main1()
{

    init_itc();
    itc_msg* imap_msg = (itc_msg*)malloc(sizeof(itc_msg));
    HMAP_TYPE(char*,int)* int_map = malloc(sizeof(HMAP_TYPE(char*, int)));
    int_map = NULL;

    // Insert key-value pairs into the hash map
    shput(int_map, "one", 1);
    shput(int_map, "two", 2);
    shput(int_map, "three", 3);

    strcpy(imap_msg->topic, "IntMap");
    imap_msg->data = (void*)int_map;

    add_reciever("IntMap", &testCallback1);
    add_msg(imap_msg);
    sleep(1);
    shput(int_map, "two", 4);
    add_msg(imap_msg);


    cleanup_itc();

    hmfree(int_map);
    free(int_map);
    return 0;
};



void testCallback1(itc_msg i)
{
    imap* map = (imap*)i.data;
    printf("In callback1: %s\n", i.topic);
    int value = shget(map, "two");
    printf("In callback1: %d\n", value);
}
