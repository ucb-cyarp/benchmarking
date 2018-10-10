#include "intrin_bench_default_defines.h"

#ifdef __AVX__
    //==========_mm256_store_si256 int8==========
    void kernel_asm_mm256_store_si256_int8_local_unroll120( __m256i* a)
    {
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/(32*60); i+=2)
        {
            asm volatile(
                //1
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //2
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //3
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //4
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //5
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //6
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //7
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //8
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //9
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //10
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //11
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //12
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //13
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //14
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //15
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //16
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //17
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //18
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //19
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //20
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //21
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //22
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //23
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //24
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //25
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //26
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //27
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //28
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //29
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //30
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //31
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //32
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //33
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //34
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //35
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //36
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //37
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //38
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //39
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //40
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //41
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //42
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //43
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //44
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //45
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //46
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //47
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //48
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //49
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //50
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //51
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //52
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //53
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //54
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //55
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //56
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //57
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //58
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //59
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //60
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"

                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_si256 int16==========
    void kernel_asm_mm256_store_si256_int16_local_unroll120( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/(16*60); i+=2)
        {
            asm volatile(
                //1
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //2
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //3
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //4
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //5
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //6
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //7
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //8
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //9
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //10
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //11
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //12
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //13
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //14
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //15
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //16
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //17
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //18
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //19
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //20
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //21
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //22
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //23
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //24
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //25
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //26
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //27
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //28
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //29
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //30
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //31
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //32
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //33
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //34
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //35
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //36
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //37
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //38
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //39
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //40
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //41
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //42
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //43
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //44
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //45
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //46
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //47
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //48
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //49
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //50
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //51
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //52
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //53
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //54
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //55
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //56
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //57
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //58
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //59
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //60
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"

                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_si256 int32==========
    void kernel_asm_mm256_store_si256_int32_local_unroll120( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/(8*60); i+=2)
        {
            asm volatile(
                //1
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //2
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //3
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //4
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //5
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //6
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //7
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //8
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //9
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //10
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //11
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //12
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //13
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //14
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //15
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //16
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //17
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //18
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //19
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //20
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //21
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //22
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //23
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //24
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //25
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //26
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //27
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //28
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //29
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //30
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //31
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //32
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //33
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //34
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //35
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //36
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //37
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //38
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //39
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //40
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //41
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //42
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //43
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //44
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //45
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //46
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //47
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //48
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //49
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //50
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //51
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //52
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //53
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //54
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //55
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //56
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //57
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //58
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //59
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //60
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"

                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_si256 int64==========
    void kernel_asm_mm256_store_si256_int64_local_unroll120( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3);
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/(4*60); i+=2)
        {
            asm volatile(
                //1
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //2
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //3
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //4
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //5
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //6
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //7
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //8
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //9
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //10
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //11
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //12
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //13
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //14
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //15
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //16
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //17
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //18
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //19
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //20
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //21
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //22
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //23
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //24
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //25
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //26
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //27
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //28
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //29
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //30
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //31
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //32
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //33
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //34
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //35
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //36
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //37
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //38
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //39
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //40
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //41
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //42
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //43
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //44
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //45
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //46
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //47
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //48
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //49
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //50
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //51
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //52
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //53
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //54
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //55
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //56
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //57
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //58
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //59
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                //60
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"

                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_ps==========
    void kernel_asm_mm256_store_ps_local_unroll120( __m256* a)
    {
        float* a_float = (float*) a;
        // __m256 a_val = _mm256_set_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);
        float* c = a_float+8;
        for(int i = 0; i<STIM_LEN; i+=(16*60))
        {
            asm volatile(
                //1
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //2
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //3
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //4
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //5
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //6
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //7
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //8
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //9
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //10
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //11
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //12
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //13
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //14
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //15
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //16
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //17
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //18
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //19
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //20
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //21
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //22
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //23
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //24
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //25
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //26
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //27
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //28
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //29
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //30
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //31
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //32
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //33
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //34
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //35
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //36
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //37
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //38
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //39
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //40
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //41
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //42
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //43
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //44
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //45
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //46
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //47
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //48
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //49
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //50
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //51
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //52
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //53
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //54
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //55
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //56
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //57
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //58
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //59
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                //60
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"

                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_pd==========
    void kernel_asm_mm256_store_pd_local_unroll120( __m256d* a)
    {
        double* a_double = (double*) a;
        // __m256d a_val = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);
        double* c = a_double+4;

        for(int i = 0; i<STIM_LEN; i+=(8*60))
        {
            asm volatile(
                //1
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //2
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //3
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //4
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //5
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //6
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //7
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //8
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //9
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //10
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //11
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //12
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //13
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //14
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //15
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //16
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //17
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //18
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //19
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //20
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //21
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //22
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //23
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //24
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //25
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //26
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //27
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //28
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //29
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //30
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //31
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //32
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //33
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //34
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //35
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //36
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //37
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //38
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //39
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //40
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //41
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //42
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //43
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //44
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //45
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //46
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //47
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //48
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //49
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //50
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //51
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //52
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //53
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //54
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //55
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //56
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //57
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //58
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //59
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                //60
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"

                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }
#endif