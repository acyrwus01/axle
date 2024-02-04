#include "axle_itc.h"
#include "stb_ds.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define ARENA_IMPLEMENTATION
//#define REGION_DEFAULT_CAPACITY (1024)
#include "arena.h"

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Assertion failed: %s\nMessage: %s\nFile: %s\nLine: %d\n", \
                    #condition, message, __FILE__, __LINE__); \
            abort(); \
        } \
    } while(0)

typedef u_int64_t u64;
struct big
{
    u64 big[10000];
};
int main_arena() {
    //return 0;
    Arena ar = {0};
    //char* mystr = (char*)arena_alloc(&ar, 256);
    struct big* mybig = (struct big*)arena_alloc(&ar, sizeof(struct big));
    for(u64 i = 0; i < 10000; i++) mybig->big[i] = i*i;
    //mystr = "Bababooey";
    //printf("Size %zu\n", strlen(mystr));
    printf("Size %zu\n", sizeof(struct big));
    arena_free(&ar);

    return 0;
}

void test_cb_1(const itc_msg i);
void test_cb_2(const itc_msg i);
int current_number1;

itc_msg* msgs[1000];

void test_itc()
{
    init_itc();

    add_reciever("Bababooey", &test_cb_1);
    add_reciever("Bababooey", &test_cb_2);

    for(u64 i = 0; i < 1000; ++i)
        add_msg("Bababooey", (void*)i);

    add_msg("Bababooey", (void*)1234);

    printf("What is current number??? %d\n", current_number1);
    cleanup_itc();
    for(u64 i = 0; i < 1000; ++i) free(msgs[i]);
    ASSERT(current_number1 == 1234, "Cringe");
}
void test_cb_1(const itc_msg i)
{
    current_number1 = (u64)i.data;
    printf("Topic %s\n", i.topic);
    printf("Data: %zu\n", (u64)i.data);
}

int current_number2;
void test_cb_2( const itc_msg i)
{
    current_number2 = (u64)i.data;
    printf("Topic %s\n", i.topic);
    printf("Data: %zu\n", (u64)i.data);
    ASSERT(current_number1 == current_number2, "OR ELSE");
}
