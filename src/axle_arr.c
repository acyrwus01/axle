#include "axle_arr.h"
#include "axle_typedefs.h"

typedef struct
{
    u16 uid;
    void* data;
} item;
typedef struct
{
    item arr[512];
    u16 uids;
    u16 size;
} static_arr;

void static_arr_add(static_arr* arr, void* data)
{
    item i;

    arr->size++;
    i.uid = arr->uids++;
    i.data = data;
    arr->arr[arr->size] = i;
}

void static_arr_del(static_arr* arr, u16 uid)
{
    for(u16 i; i < arr->size; ++i)
    {
        if( uid == arr->arr[i].uid )
        {
            arr->arr[i] = arr->arr[arr->size - 1];
            arr->size--;
        }
    }
}
