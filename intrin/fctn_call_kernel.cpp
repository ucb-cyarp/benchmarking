#include "intrin_bench_default_defines_and_imports_cpp.h"
#include "zero_arg_fctn.h"

//==========0 arg function call==========
void kernel_zero_arg_fctn()
{
    for(int i = 0; i<STIM_LEN; i++)
    {
        zero_arg_fctn();
    }
}