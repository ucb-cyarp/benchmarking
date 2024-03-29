#ifndef _H_LATENCY_FLOW_CTRL_BLOCKED_READ_KERNEL
    #define _H_LATENCY_FLOW_CTRL_BLOCKED_READ_KERNEL
    #include <stdint.h>
    #include <cstddef>
    #include <atomic>

    class LatencyFlowCtrlBlockedReadKernelArgs
    {
        public:
            int32_t* array_shared_ptr;
            std::atomic_int32_t* valid_shared_ptr;
            std::atomic_int32_t* ack_shared_ptr;
            size_t length;
    };

        class LatencyFlowCtrlBlockedReadJoinKernelArgs
    {
        public:
            int32_t* array_shared_ptr_a;
            std::atomic_int32_t* valid_shared_ptr_a;
            std::atomic_int32_t* ack_shared_ptr_a;
            size_t length_a;

            int32_t* array_shared_ptr_b;
            std::atomic_int32_t* valid_shared_ptr_b;
            std::atomic_int32_t* ack_shared_ptr_b;
            size_t length_b;
    };

    void* latency_flow_ctrl_blocked_read_server_kernel(void* arg);

    void* latency_flow_ctrl_blocked_read_server_join_kernel(void* arg);

    void* latency_flow_ctrl_blocked_read_client_kernel(void* arg);

    void* latency_flow_ctrl_blocked_read_client_join_kernel(void* arg);

    void* latency_flow_ctrl_blocked_read_kernel_reset(void* arg);

    void* latency_flow_ctrl_blocked_read_join_kernel_reset(void* arg);
    
#endif