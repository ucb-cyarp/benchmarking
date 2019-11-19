#define _GNU_SOURCE
#include "mallocHelpers.h"
#include <mm_malloc.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <cstring>
#include <sched.h>
#include <unistd.h>
#include <pthread.h>

typedef struct{
    size_t size;
    size_t alignment;
} aligned_malloc_args_t;

#if __APPLE__
//Apple does not support the type of core affinity assignment we use
void* malloc_core(size_t size, int core){
    std::cerr << "Warning, cannot allocate on specific core on Mac" << std::endl;
    return malloc(size);
}

void* _mm_malloc_core(size_t size, size_t alignment, int core){
    std::cerr << "Warning, cannot allocate on specific core on Mac" << std::endl;
    return _mm_malloc(size, alignment);
}

void* aligned_alloc_core(size_t alignment, size_t size, int core){
    std::cerr << "Warning, cannot allocate on specific core on Mac" << std::endl;
    std::cerr << "Warning, Mac does not support aligned_alloc, using posix_memalign instead)" << std::endl;

    size_t allocSize = size + (size%alignment == 0 ? 0 : alignment-(size%alignment));

    void* ptr;
    int status = posix_memalign(&ptr, alignment, allocSize);

    if(status != 0){
        ptr = NULL;
    }

    return ptr;
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
        size_t alignment = arg->alignment;

        void* rtnVal = _mm_malloc(size, alignment);
        return rtnVal;
    }

    void* aligned_alloc_core_thread(void* arg_uncast){
        aligned_malloc_args_t* arg = (aligned_malloc_args_t*) arg_uncast;
        size_t size = arg->size;
        size_t alignment = arg->alignment;

        size_t allocSize = size + (size%alignment == 0 ? 0 : alignment-(size%alignment));

        //There is a condition on aligned_alloc that the size must be a
        //multiple of the alignment
        void* rtnVal = aligned_alloc(alignment, allocSize);
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

    void* _mm_malloc_core(size_t size, size_t alignment, int core){
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
        args->alignment = alignment;

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

    void* aligned_alloc_core(size_t alignment, size_t size, int core){
        cpu_set_t cpuset;
        pthread_t thread;
        pthread_attr_t attr;
        void *res;

        int status;

        //Create pthread attributes
        status = pthread_attr_init(&attr);
        if(status != 0)
        {
            printf("Could not create pthread attributes for aligned_alloc_core ... exiting\n");
            exit(1);
        }

        //Set CPU affinity
        CPU_ZERO(&cpuset);
        CPU_SET(core, &cpuset);
        status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
        if(status != 0)
        {
            printf("Could not set thread core affinity for aligned_alloc_core ... exiting\n");
            exit(1);
        }

        // - Start Thread
        aligned_malloc_args_t* args = new aligned_malloc_args_t;
        args->size = size;
        args->alignment = alignment;

        status = pthread_create(&thread, &attr, aligned_alloc_core_thread, args);
        if(status != 0)
        {
            printf("Could not create a thread for aligned_alloc_core ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        //Wait for thread to finish
        status = pthread_join(thread, &res);
        if(status != 0)
        {
            printf("Could not join a thread for aligned_alloc_core ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        delete args;

        return res;
    }

#endif
