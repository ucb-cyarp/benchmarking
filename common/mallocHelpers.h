#ifndef MALLOC_HELPERS_H
#define MALLOC_HELPERS_H

#include <memory>

void* malloc_core(size_t size, int core);

void* _mm_malloc_core(size_t size, size_t alignment, int core);

//The order of the arguments is reversed compared to _mm_malloc
void* aligned_alloc_core(size_t alignment, size_t size, int core); 

#endif