#ifndef _H_BANDWIDTH_CIRCULAR_FIFO_READ_LIMIT_KERNEL
    #define _H_BANDWIDTH_CIRCULAR_FIFO_READ_LIMIT_KERNEL
    #include <stdint.h>
    #include <cstddef>
    #include <atomic>

    class BandwidthCircularFifoReadLimitKernelArgs
    {
        public:
            int32_t* array_shared_ptr;
            std::atomic_int32_t* write_pos_shared_ptr;
            std::atomic_int32_t* read_pos_shared_ptr;
            int32_t max_elements_per_transaction;
            size_t length;
    };

    void* bandwidth_circular_fifo_read_limit_server_kernel(void* arg);

    void* bandwidth_circular_fifo_read_limit_client_kernel(void* arg);

    void* bandwidth_circular_fifo_read_limit_kernel_reset(void* arg);
    
#endif