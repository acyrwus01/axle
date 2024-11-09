#include "mem_defer.h"
#include "stdint.h"
#include <cstdio>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

void some_func(void) {
    FILE *file = fopen("f.txt", "w");
    if (file == nullptr)
        return;
    defer (fclose(file));

    defer ({
        printf("You can even defer ");
        printf("an entire block too!");
    });

    // Do whatever...
}
