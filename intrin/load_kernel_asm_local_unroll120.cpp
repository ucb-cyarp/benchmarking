#include "intrin_bench_default_defines.h"

#ifdef __AVX__
    //==========_mm256_load_si256 int8==========
    void kernel_asm_mm256_load_si256_int8_local_unroll120( __m256i* a)
    {
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/(32*60); i+=2)
        {
            asm volatile(
                //1
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //2
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //3
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //4
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //5
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //6
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //7
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //8
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //9
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //10
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //11
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //12
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //13
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //14
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //15
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //16
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //17
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //18
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //19
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //20
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //21
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //22
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //23
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //24
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //25
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //26
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //27
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //28
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //29
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //30
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //31
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //32
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //33
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //34
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //35
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //36
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //37
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //38
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //39
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //40
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //41
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //42
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //43
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //44
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //45
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //46
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //47
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //48
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //49
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //50
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //51
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //52
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //53
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //54
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //55
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //56
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //57
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //58
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //59
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //60
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"

                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_si256 int16==========
    void kernel_asm_mm256_load_si256_int16_local_unroll120( __m256i* a)
    {
        __m256i* c = a+1;

        for(int i = 0; i<STIM_LEN/(16*60); i+=2)
        {
            asm volatile(
                //1
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //2
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //3
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //4
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //5
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //6
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //7
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //8
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //9
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //10
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //11
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //12
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //13
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //14
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //15
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //16
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //17
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //18
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //19
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //20
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //21
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //22
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //23
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //24
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //25
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //26
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //27
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //28
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //29
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //30
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //31
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //32
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //33
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //34
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //35
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //36
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //37
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //38
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //39
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //40
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //41
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //42
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //43
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //44
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //45
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //46
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //47
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //48
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //49
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //50
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //51
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //52
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //53
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //54
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //55
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //56
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //57
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //58
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //59
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //60
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                
                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_si256 int32==========
    void kernel_asm_mm256_load_si256_int32_local_unroll120( __m256i* a)
    {
        __m256i* c = a+1;

        for(int i = 0; i<STIM_LEN/(8*60); i+=2)
        {
            asm volatile(
                //1
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //2
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //3
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //4
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //5
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //6
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //7
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //8
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //9
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //10
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //11
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //12
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //13
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //14
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //15
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //16
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //17
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //18
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //19
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //20
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //21
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //22
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //23
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //24
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //25
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //26
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //27
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //28
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //29
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //30
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //31
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //32
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //33
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //34
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //35
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //36
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //37
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //38
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //39
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //40
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //41
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //42
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //43
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //44
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //45
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //46
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //47
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //48
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //49
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //50
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //51
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //52
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //53
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //54
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //55
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //56
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //57
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //58
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //59
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //60
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"

                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_si256 int64==========
    void kernel_asm_mm256_load_si256_int64_local_unroll120( __m256i* a)
    {
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/(4*60); i+=2)
        {
            asm volatile(
                //1
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //2
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //3
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //4
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //5
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //6
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //7
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //8
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //9
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //10
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //11
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //12
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //13
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //14
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //15
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //16
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //17
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //18
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //19
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //20
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //21
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //22
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //23
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //24
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //25
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //26
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //27
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //28
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //29
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //30
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //31
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //32
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //33
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //34
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //35
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //36
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //37
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //38
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //39
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //40
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //41
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //42
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //43
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //44
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //45
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //46
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //47
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //48
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //49
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //50
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //51
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //52
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //53
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //54
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //55
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //56
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //57
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //58
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //59
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                //60
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"

                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_ps==========
    void kernel_asm_mm256_load_ps_local_unroll120( __m256* a)
    {
        float* a_float = (float*) a;
        float* c = a_float+8;

        for(int i = 0; i<STIM_LEN; i+=(16*60))
        {
            asm volatile(
                //1
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //2
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //3
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //4
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //5
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //6
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //7
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //8
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //9
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //10
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //11
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //12
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //13
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //14
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //15
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //16
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //17
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //18
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //19
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //20
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //21
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //22
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //23
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //24
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //25
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //26
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //27
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //28
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //29
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //30
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //31
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //32
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //33
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //34
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //35
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //36
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //37
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //38
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //39
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //40
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //41
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //42
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //43
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //44
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //45
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //46
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //47
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //48
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //49
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //50
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //51
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //52
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //53
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //54
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //55
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //56
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //57
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //58
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //59
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                //60
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"

                :
                : "m" (*a_float), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_pd==========
    void kernel_asm_mm256_load_pd_local_unroll120( __m256d* a)
    {
        double* a_double = (double*) a;
        double* c = a_double+4;

        for(int i = 0; i<STIM_LEN; i+=(8*60))
        {
            asm volatile(
                //1
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //2
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //3
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //4
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //5
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //6
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //7
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //8
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //9
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //10
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //11
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //12
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //13
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //14
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //15
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //16
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //17
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //18
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //19
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //20
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //21
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //22
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //23
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //24
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //25
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //26
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //27
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //28
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //29
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //30
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //31
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //32
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //33
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //34
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //35
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //36
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //37
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //38
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //39
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //40
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //41
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //42
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //43
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //44
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //45
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //46
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //47
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //48
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //49
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //50
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //51
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //52
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //53
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //54
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //55
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //56
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //57
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //58
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //59
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                //60
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"

                :
                : "m" (*a_double), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }
#endif