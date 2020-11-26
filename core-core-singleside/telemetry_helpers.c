#include "telemetry_helpers.h"
#include <unistd.h>
#include <time.h>

double difftimespec(timespec_t* a, timespec_t* b){
    double a_double = a->tv_sec + (a->tv_nsec)*(0.000000001);
    double b_double = b->tv_sec + (b->tv_nsec)*(0.000000001);
    return a_double - b_double;
}
double timespecToDouble(timespec_t* a){
    double a_double = a->tv_sec + (a->tv_nsec)*(0.000000001);
    return a_double;
}
