#ifndef _H_BANDWIDTH_CIRCULAR_FIFO_BLOCKED_KERNEL
    #define _H_BANDWIDTH_CIRCULAR_FIFO_BLOCKED_KERNEL
    #include <stdint.h>
    #include <cstddef>
    #include <atomic>

    class BandwidthCircularFifoBlockedKernelArgs
    {
        public:
            int32_t* array_shared_ptr;
            std::atomic_int32_t* write_pos_shared_ptr;
            std::atomic_int32_t* read_pos_shared_ptr;
            int32_t block_length; //In elements
            size_t length; //In elements
    };

    void* bandwidth_circular_fifo_blocked_server_kernel(void* arg);

    void* bandwidth_circular_fifo_blocked_client_kernel(void* arg);

    void* bandwidth_circular_fifo_blocked_kernel_reset(void* arg);
    
#endif