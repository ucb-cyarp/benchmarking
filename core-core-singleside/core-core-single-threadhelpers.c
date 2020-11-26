#ifndef _GNU_SOURCE
    #define _GNU_SOURCE //For clock_gettime
#endif

#include <stdatomic.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>

#include "core-core-single-threadhelpers.h"

void* init_atomic_thread(void* args){
   init_atomic_args_t *argsCast = (init_atomic_args_t*) args;

   atomic_init(argsCast->toInit, argsCast->initVal);
   
   return NULL;
}

void init_atomic(_Atomic int8_t *toInit, int8_t initVal, int core){
        cpu_set_t cpuset;
        pthread_t thread;
        pthread_attr_t attr;
        void *res;

        int status;

        //Create pthread attributes
        status = pthread_attr_init(&attr);
        if(status != 0)
        {
            printf("Could not create pthread attributes for init_atomic ... exiting\n");
            exit(1);
        }

        //Set CPU affinity
        CPU_ZERO(&cpuset);
        CPU_SET(core, &cpuset);
        status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
        if(status != 0)
        {
            printf("Could not set thread core affinity for init_atomic ... exiting\n");
            exit(1);
        }

        // - Start Thread
        init_atomic_args_t* args = malloc(sizeof(init_atomic_args_t));
        args->toInit = toInit;
        args->initVal = initVal;

        status = pthread_create(&thread, &attr, init_atomic_thread, args);
        if(status != 0)
        {
            printf("Could not create a thread for init_atomic ... exiting\n");
            perror(NULL);
            exit(1);
        }

        //Wait for thread to finish
        status = pthread_join(thread, &res);
        if(status != 0)
        {
            printf("Could not join a thread for init_atomic ... exiting\n");
            perror(NULL);
            exit(1);
        }

        free(args);
}

void* reset_atomic_thread(void* args){
    init_atomic_args_t *argsCast = (init_atomic_args_t*) args;
    atomic_thread_fence(memory_order_acquire);

    atomic_store_explicit(argsCast->toInit, argsCast->initVal, memory_order_release);
   
    return NULL;
}

void reset_atomic(_Atomic int8_t *toInit, int8_t initVal, int core){
        cpu_set_t cpuset;
        pthread_t thread;
        pthread_attr_t attr;
        void *res;

        int status;

        //Create pthread attributes
        status = pthread_attr_init(&attr);
        if(status != 0)
        {
            printf("Could not create pthread attributes for init_atomic ... exiting\n");
            exit(1);
        }

        //Set CPU affinity
        CPU_ZERO(&cpuset);
        CPU_SET(core, &cpuset);
        status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
        if(status != 0)
        {
            printf("Could not set thread core affinity for init_atomic ... exiting\n");
            exit(1);
        }

        // - Start Thread
        init_atomic_args_t* args = malloc(sizeof(init_atomic_args_t));
        args->toInit = toInit;
        args->initVal = initVal;

        status = pthread_create(&thread, &attr, reset_atomic_thread, args);
        if(status != 0)
        {
            printf("Could not create a thread for init_atomic ... exiting\n");
            perror(NULL);
            exit(1);
        }

        //Wait for thread to finish
        status = pthread_join(thread, &res);
        if(status != 0)
        {
            printf("Could not join a thread for init_atomic ... exiting\n");
            perror(NULL);
            exit(1);
        }

        free(args);
}