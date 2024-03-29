#ifndef _H_LATENCY_DUAL_ARRAY_KERNEL
    #define _H_LATENCY_DUAL_ARRAY_KERNEL
    #include <stdint.h>
    #include <cstddef>
    #include <atomic>

    class LatencyDualArrayKernelArgs
    {
        public:
            std::atomic_int32_t* my_shared_ptr;
            std::atomic_int32_t* other_shared_ptr;
            size_t length;
            int32_t init_counter;
    };

    class LatencyDualArrayJoinKernelArgs
    {
        public:
            std::atomic_int32_t* my_shared_ptr_a;
            std::atomic_int32_t* other_shared_ptr_a;
            size_t length_a;
            int32_t init_counter_a;

            std::atomic_int32_t* my_shared_ptr_b;
            std::atomic_int32_t* other_shared_ptr_b;
            size_t length_b;
            int32_t init_counter_b;
    };

    class LatencyDualArrayKernelResetArgs
    {
        public:
            std::atomic_int32_t* shared_ptr_a;
            std::atomic_int32_t* shared_ptr_b;
            size_t length;
    };

    class LatencyDualArrayJoinKernelResetArgs
    {
        public:
            std::atomic_int32_t* shared_ptr_a;
            std::atomic_int32_t* shared_ptr_b;

            std::atomic_int32_t* shared_ptr_c;
            std::atomic_int32_t* shared_ptr_d;

            size_t length_ab;

            size_t length_cd;
    };

    void* latency_dual_array_kernel(void* arg);

    void* latency_dual_array_join_kernel(void* arg);

    void* latency_dual_array_kernel_reset(void* arg);

    void* latency_dual_array_join_kernel_reset(void* arg);
    
#endif