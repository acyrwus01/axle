#ifndef AXLE_HMAP_H_
#define AXLE_HMAP_H_

#ifndef AXLE_HMAP_MACRO
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define HMAP_TYPE(T1,T2) struct {T1 key; T2 value;}
#define HMAP_NEW(T1,T2)  ()
#define HMAP_GET hmget

#endif
#endif // AXLE_HMAP_H_
