#ifndef TELEMETRY_HELPERS_H
#define TELEMETRY_HELPERS_H

#include <time.h>

typedef struct timespec timespec_t;
double difftimespec(timespec_t* a, timespec_t* b);
double timespecToDouble(timespec_t* a);

#endif
