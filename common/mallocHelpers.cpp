#include "mallocHelpers.h"
#include <mm_malloc.h>
#include <stdio.h>

typedef struct{
    size_t size;
    size_t allignment;
} aligned_malloc_args_t;

#if __APPLE__
//Apple does not support the type of core affinity assignment we use
void* malloc_core(size_t size, int core){
    std::cerr << "Warning, cannot allocate on specific core on Mac" << std::endl;
    return malloc(size);
}

void* _mm_malloc_core(size_t size, size_t allignment, int core){
    std::cerr << "Warning, cannot allocate on specific core on Mac" << std::endl;
    return _mm_malloc(size, allignment);
}

void* aligned_alloc_core(size_t allignment, size_t size, int core){
    std::cerr << "Warning, cannot allocate on specific core on Mac" << std::endl;
    return aligned_alloc(size_t allignment, size_t size);
}

#else
    //Worker Threads
    void* malloc_core_thread(void* arg_uncast){
        size_t* size = (size_t*) arg_uncast;

        void* rtnVal = malloc(*size);
        return rtnVal;
    }

    void* _mm_malloc_core_thread(void* arg_uncast){
        aligned_malloc_args_t* arg = (aligned_malloc_args_t*) arg_uncast;
        size_t size = arg->size;
        size_t allignment = arg->allignment;

        void* rtnVal = _mm_malloc(size, allignment);
        return rtnVal;
    }

    void* alligned_alloc_core_thread(void* arg_uncast){
        aligned_malloc_args_t* arg = (aligned_malloc_args_t*) arg_uncast;
        size_t size = arg->size;
        size_t allignment = arg->allignment;

        void* rtnVal = aligned_alloc(allignment, size);
        return rtnVal;
    }

    void* malloc_core(size_t size, int core){
        cpu_set_t cpuset;
        pthread_t thread;
        pthread_attr_t attr;
        void *res;

        int status;

        //Create pthread attributes
        status = pthread_attr_init(&attr);
        if(status != 0)
        {
            printf("Could not create pthread attributes for malloc_core ... exiting\n");
            exit(1);
        }

        //Set CPU affinity
        CPU_ZERO(&cpuset);
        CPU_SET(core, &cpuset);
        status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
        if(status != 0)
        {
            printf("Could not set thread core affinity for malloc_core ... exiting\n");
            exit(1);
        }

        // - Start Thread
        size_t* newSize = new size_t;
        *newSize = size;

        status = pthread_create(&thread, &attr, malloc_core_thread, newSize);
        if(status != 0)
        {
            printf("Could not create a thread for malloc_core ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        //Wait for thread to finish
        status = pthread_join(thread, &res);
        if(status != 0)
        {
            printf("Could not join a thread for malloc_core ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        delete newSize;

        return res;
    }

    void* _mm_malloc_core(size_t size, size_t allignment, int core){
        cpu_set_t cpuset;
        pthread_t thread;
        pthread_attr_t attr;
        void *res;

        int status;

        //Create pthread attributes
        status = pthread_attr_init(&attr);
        if(status != 0)
        {
            printf("Could not create pthread attributes for malloc_core ... exiting\n");
            exit(1);
        }

        //Set CPU affinity
        CPU_ZERO(&cpuset);
        CPU_SET(core, &cpuset);
        status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
        if(status != 0)
        {
            printf("Could not set thread core affinity for malloc_core ... exiting\n");
            exit(1);
        }

        // - Start Thread
        aligned_malloc_args_t* args = new aligned_malloc_args_t;
        args->size = size;
        args->allignment = allignment;

        status = pthread_create(&thread, &attr, _mm_malloc_core_thread, args);
        if(status != 0)
        {
            printf("Could not create a thread for malloc_core ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        //Wait for thread to finish
        status = pthread_join(thread, &res);
        if(status != 0)
        {
            printf("Could not join a thread for malloc_core ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        delete args;

        return res;
    }

    void* aligned_alloc_core(size_t allignment, size_t size, int core){
        cpu_set_t cpuset;
        pthread_t thread;
        pthread_attr_t attr;
        void *res;

        int status;

        //Create pthread attributes
        status = pthread_attr_init(&attr);
        if(status != 0)
        {
            printf("Could not create pthread attributes for alligned_alloc_core ... exiting\n");
            exit(1);
        }

        //Set CPU affinity
        CPU_ZERO(&cpuset);
        CPU_SET(core, &cpuset);
        status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
        if(status != 0)
        {
            printf("Could not set thread core affinity for alligned_alloc_core ... exiting\n");
            exit(1);
        }

        // - Start Thread
        aligned_malloc_args_t* args = new aligned_malloc_args_t;
        args->size = size;
        args->allignment = allignment;

        status = pthread_create(&thread, &attr, alligned_alloc_core_thread, args);
        if(status != 0)
        {
            printf("Could not create a thread for alligned_alloc_core ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        //Wait for thread to finish
        status = pthread_join(thread, &res);
        if(status != 0)
        {
            printf("Could not join a thread for alligned_alloc_core ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        delete args;

        return res;
    }

#endif
