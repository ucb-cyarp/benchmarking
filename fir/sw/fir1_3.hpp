#include "fir1.hpp"

/*
 * Naieve Implementation of FIR Filter
 * 
 * Filter uses 2 nested loops
 * No explicit vectorization/intrinsics
 * **Explicit unrolling/blocking ** Unroll Factor 2 ** - Block Size must be in multiples of 2
 * No pragma use
 * No threading
 * Working buffer implemented using shifts (not circular buffer)
 *     -Adds another nested loop
 */

template <typename T, size_t coef_len, size_t io_len>
Fir<T, coef_len, io_len>::Fir(const T* coef, const T* init)
{
    for(size_t i = 0; i<coef_len; i++)
    {
        coef_array[i] = coef[i];
    }

    for(size_t i = 0; i<coef_len; i++)
    {
        working_array[i] = init[i];
    }
}

template <typename T, size_t coef_len, size_t io_len>
void Fir<T, coef_len, io_len>::filter(const T* in, T* out)
{
    T working_array_1[coef_len];

    //For each input sample
    for(size_t i = 0; i<io_len-1; i+=2)
    {
        //Shift over previous samples
        for(size_t j = coef_len-1; j>1; j--)
        {
            working_array[j]   = working_array[j-2];
            working_array_1[j] = working_array[j-1];
        }
        //Clean up the last shift for working_array_1
        working_array_1[1] = working_array[0];
        working_array_1[0] = in[i];

        working_array[1] = in[i];
        working_array[0] = in[i+1];
        
        //Compute FIR output
        T output_1 = 0;
        T output = 0;
        for(size_t j = 0; j<coef_len; j++)
        {
            output_1 += working_array_1[j] * coef_array[j];
            output   +=  working_array[j]  * coef_array[j];
        }

        out[i] = output_1;
        out[i+1] = output;
    }
}