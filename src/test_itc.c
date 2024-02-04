#include "axle_itc.h"
#include "axle_sched.h"
#include "axle_typedefs.h"


void cb(const itc_msg msg)
{
    int* my_data = (int*)msg.data;
    *my_data = 234;
    printf("Message Recieved: %d\n", *my_data);
}

void task_one(void* data) {
    printf("My data: %d\n", *(int*)data);
    add_msg("test", data);
}

int main(void)
{
    printf("Test\n");

    int ret = sched_init();
    int* value = malloc(sizeof(int));
    *value = 123;
    init_itc();
    add_reciever("test", cb);

    printf("Init %d\n", ret);
    schedule_task(task_one, 1 * NANOSECONDS_PER_SECOND, (void*)(value), false);
    while(1){};

    return 0;
}
