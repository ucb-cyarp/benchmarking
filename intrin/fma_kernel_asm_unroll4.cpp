#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __FMA__
    //==========_mm256_fmadd_ps==========
    void kernel_only_asm_mm256_fmadd_ps_unroll4()
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            //NOTE: There are multiple vfmadd instructions.  Three varients change the meaning of the operands and which is replaced
            asm volatile(
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_fmadd_pd==========
    void kernel_only_asm_mm256_fmadd_pd_unroll4()
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            //NOTE: There are multiple vfmadd instructions.  Three varients change the meaning of the operands and which is replaced
            asm volatile(
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif
