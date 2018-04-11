#ifndef _H_LATENCY_DUAL_ARRAY_KERNEL
    #define _H_LATENCY_DUAL_ARRAY_KERNEL
    #include "cpucounters.h"

    class LatencyDualArrayKernelArgs
    {
        public:
            int32_t* my_shared_ptr;
            int32_t* other_shared_ptr;
            size_t length;
            int32_t init_counter;
    };

    class LatencyDualArrayKernelResetArgs
    {
        public:
            int32_t* shared_ptr_a;
            int32_t* shared_ptr_b;
            size_t length;
    };

    void* latency_dual_array_kernel(void* arg);

    void* latency_dual_array_kernel_reset(void* arg);
    
#endif