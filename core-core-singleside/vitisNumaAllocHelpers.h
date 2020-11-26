#ifndef VITISNUMAALLOCHELPERS_H
#define VITISNUMAALLOCHELPERS_H
    #include <stdint.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    void* vitis_malloc_core(size_t size, int core);
    void* vitis__mm_malloc_core(size_t size, size_t alignment, int core);
    void* vitis_aligned_alloc_core(size_t alignment, size_t size, int core);
    void* vitis_aligned_alloc(size_t alignment, size_t size);
#endif
