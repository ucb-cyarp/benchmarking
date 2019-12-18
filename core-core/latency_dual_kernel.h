#ifndef _H_LATENCY_DUAL_KERNEL
    #define _H_LATENCY_DUAL_KERNEL
    #include <stdint.h>
    #include <atomic>

    // class LatencySingleKernelServerArgs
    // {
    //     public:
    //         PCM* pcm;
    //         int32_t* shared_ptr;
    // };

    // class LatencySingleKernelClientArgs
    // {
    //     public:
    //         int32_t* shared_ptr;
    // };

    class LatencyDualKernelArgs
    {
        public:
            std::atomic_int32_t* my_shared_ptr;
            std::atomic_int32_t* other_shared_ptr;
            int32_t init_counter;
    };

    class LatencyDualKernelResetArgs
    {
        public:
            std::atomic_int32_t* shared_ptr_a;
            std::atomic_int32_t* shared_ptr_b;
    };

    void* latency_dual_kernel(void* arg);

    void* latency_dual_kernel_reset(void* arg);
    
#endif