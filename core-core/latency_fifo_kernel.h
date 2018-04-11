#ifndef _H_LATENCY_FIFO_KERNEL
    #define _H_LATENCY_FIFO_KERNEL
    #include "cpucounters.h"

    class LatencyFIFOKernelArgs
    {
        public:
            int32_t* array_shared_ptr;
            int32_t* ack_shared_ptr;
            size_t length;
    };

    void* latency_fifo_server_kernel(void* arg);

    void* latency_fifo_client_kernel(void* arg);

    void* latency_fifo_kernel_reset(void* arg);
    
#endif