#ifndef _H_LATENCY_FLOW_CTRL_KERNEL
    #define _H_LATENCY_FLOW_CTRL_KERNEL
    #include "cpucounters.h"

    class LatencyFlowCtrlKernelArgs
    {
        public:
            int32_t* array_shared_ptr;
            int32_t* ack_shared_ptr;
            size_t length;
    };

    class LatencyFlowCtrlJoinKernelArgs
    {
        public:
            int32_t* array_shared_ptr_a;
            int32_t* ack_shared_ptr_a;
            size_t length_a;

            int32_t* array_shared_ptr_b;
            int32_t* ack_shared_ptr_b;
            size_t length_b;
    };

    void* latency_flow_ctrl_server_kernel(void* arg);

    void* latency_flow_ctrl_server_join_kernel(void* arg);

    void* latency_flow_ctrl_client_kernel(void* arg);

    void* latency_flow_ctrl_client_join_kernel(void* arg);

    void* latency_flow_ctrl_kernel_reset(void* arg);

    void* latency_flow_ctrl_join_kernel_reset(void* arg);
    
#endif