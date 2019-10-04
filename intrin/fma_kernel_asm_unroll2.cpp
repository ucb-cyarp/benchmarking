#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __FMA__
    //==========_mm256_fmadd_ps==========
    void kernel_only_asm_mm256_fmadd_ps_unroll2()
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            //NOTE: There are multiple vfmadd instructions.  Three varients change the meaning of the operands and which is replaced
            asm volatile(
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                :
                :
                : MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_fmadd_pd==========
    void kernel_only_asm_mm256_fmadd_pd_unroll2()
    {
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            //NOTE: There are multiple vfmadd instructions.  Three varients change the meaning of the operands and which is replaced
            asm volatile(
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                :
                :
                : MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif
