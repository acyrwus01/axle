#include "axle_itc.h"
#include "stb_ds.h"
#include <stdio.h>
#include <string.h>

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Assertion failed: %s\nMessage: %s\nFile: %s\nLine: %d\n", \
                    #condition, message, __FILE__, __LINE__); \
            abort(); \
        } \
    } while(0)

void test_cb_1(itc_msg i);
void test_cb_2(itc_msg i);
int main() {
    init_itc();

    add_reciever("Bababooey", &test_cb_1);
    add_reciever("Bababooey", &test_cb_2);

    for(int i = 0; i < 10000; ++i)
    {
        itc_msg* msg = (itc_msg*)malloc(sizeof(itc_msg));

        msg->data = (void*)i;
        strcpy(msg->topic,"Bababooey");

        add_msg(msg);
    }

    itc_msg* msg = (itc_msg*)malloc(sizeof(itc_msg));
    msg->data = (void*)1234;
    strcpy(msg->topic,"Oi Krabs");
    add_msg(msg);

    cleanup_itc();

}
int current_number1;
void test_cb_1(itc_msg i)
{
    current_number1 = (int)i.data;
    printf("Topic %s\n", i.topic);
    printf("Data: %d\n", (int)i.data);
}

int current_number2;
void test_cb_2(itc_msg i)
{
    current_number2 = (int)i.data;
    printf("Topic %s\n", i.topic);
    printf("Data: %d\n", (int)i.data);
    ASSERT(current_number1 == current_number2, "OR ELSE");
}
