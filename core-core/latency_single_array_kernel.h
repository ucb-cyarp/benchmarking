#ifndef _H_LATENCY_SINGLE_ARRAY_KERNEL
    #define _H_LATENCY_SINGLE_ARRAY_KERNEL
    #include <stdint.h>
    #include <cstddef>
    #include <atomic>

    class LatencySingleArrayKernelArgs
    {
        public:
            std::atomic_int32_t* shared_ptr;
            size_t length;
            int32_t init_counter;
    };

    class LatencySingleArrayJoinKernelArgs
    {
        public:
            std::atomic_int32_t* shared_ptr_a;
            std::atomic_int32_t* shared_ptr_b;
            size_t length_a;
            size_t length_b;
            int32_t init_counter_a;
            int32_t init_counter_b;
    };

    class LatencySingleArrayKernelResetArgs
    {
        public:
            std::atomic_int32_t* shared_ptr;
            size_t length;
    };

    class LatencySingleArrayJoinKernelResetArgs
    {
        public:
            std::atomic_int32_t* shared_ptr_a;
            std::atomic_int32_t* shared_ptr_b;
            size_t length_a;
            size_t length_b;
    };

    void* latency_single_array_kernel(void* arg);

    void* latency_single_array_join_kernel(void* arg);

    void* latency_single_array_kernel_reset(void* arg);

    void* latency_single_array_join_kernel_reset(void* arg);
    
#endif