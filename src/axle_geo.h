#ifndef AXLE_GEO_H_
#define AXLE_GEO_H_

#include "axle_typedefs.h"

/// Simple Lat Lon structure
typedef struct
{
    double lat;
    double lon;
} LL;

/// Simple Lat Lon Alt structure
typedef struct
{
    double lat;
    double lon;
    u64    alt;
} LLA;

#endif // AXLE_GEO_H_
