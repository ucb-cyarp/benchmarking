#ifndef _H_LATENCY_FLOW_CTRL_KERNEL
    #define _H_LATENCY_FLOW_CTRL_KERNEL
    #include <stdint.h>
    #include <cstddef>
    #include <atomic>

    class LatencyFlowCtrlKernelArgs
    {
        public:
            std::atomic_int32_t* array_shared_ptr;
            std::atomic_int32_t* ack_shared_ptr;
            size_t length;
    };

    class LatencyFlowCtrlJoinKernelArgs
    {
        public:
            std::atomic_int32_t* array_shared_ptr_a;
            std::atomic_int32_t* ack_shared_ptr_a;
            size_t length_a;

            std::atomic_int32_t* array_shared_ptr_b;
            std::atomic_int32_t* ack_shared_ptr_b;
            size_t length_b;
    };

    void* latency_flow_ctrl_server_kernel(void* arg);

    void* latency_flow_ctrl_server_join_kernel(void* arg);

    void* latency_flow_ctrl_client_kernel(void* arg);

    void* latency_flow_ctrl_client_join_kernel(void* arg);

    void* latency_flow_ctrl_kernel_reset(void* arg);

    void* latency_flow_ctrl_join_kernel_reset(void* arg);
    
#endif