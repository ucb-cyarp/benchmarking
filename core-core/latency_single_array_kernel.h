#ifndef _H_LATENCY_SINGLE_ARRAY_KERNEL
    #define _H_LATENCY_SINGLE_ARRAY_KERNEL
    #include "cpucounters.h"

    class LatencySingleArrayKernelArgs
    {
        public:
            int32_t* shared_ptr;
            size_t length;
            int32_t init_counter;
    };

    class LatencySingleArrayKernelResetArgs
    {
        public:
            int32_t* shared_ptr;
            size_t length;
    };

    void* latency_single_array_kernel(void* arg);

    void* latency_single_array_kernel_reset(void* arg);
    
#endif