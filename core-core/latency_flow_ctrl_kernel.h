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

    void* latency_flow_ctrl_server_kernel(void* arg);

    void* latency_flow_ctrl_client_kernel(void* arg);

    void* latency_flow_ctrl_kernel_reset(void* arg);
    
#endif