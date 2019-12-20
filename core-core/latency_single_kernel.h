#ifndef _H_LATENCY_SINGLE_KERNEL
    #define _H_LATENCY_SINGLE_KERNEL
    #include <stdint.h>
    #include <atomic>

    class LatencySingleKernelArgs
    {
        public:
            std::atomic_int32_t* shared_ptr;
            int32_t init_counter;
    };

    class LatencySingleKernelResetArgs
    {
        public:
            std::atomic_int32_t* shared_ptr;
    };

    void* latency_single_kernel(void* arg);

    void* latency_single_kernel_reset(void* shared_ptr);
    
#endif