#include "fir1a.hpp"

/*
 * Implementation of FIR Filter with Circular Buffer
 *     Direction Reversed
 * 
 * Filter uses 2 nested loops
 * No explicit vectorization/intrinsics
 * No explicit unrolling/blocking
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

    buffer_pos = 0;
}

template <typename T, size_t coef_len, size_t io_len>
void Fir<T, coef_len, io_len>::filter(const T* in, T* out)
{
    //For each input sample
    for(int64_t i = 0; i<io_len; i++)
    {
        working_array[buffer_pos] = in[i];

        //Compute FIR output
        T output = 0;
        for(size_t j = coef_len-1; j>0; j--)
        {

            size_t buffer_ind = (buffer_pos+j)%coef_len;
            output += working_array[buffer_ind] * coef_array[j];
        }
        //Last run
        {
            size_t j = 0;
            size_t buffer_ind = (buffer_pos+j)%coef_len;
            output += working_array[buffer_ind] * coef_array[j];
        }

        out[i] = output;

        //Need to Decrement Buffer_pos and have it wrap
        //Since it can go negative, or wrap on an unsigned
        //datatype, the behavior of % is not what we would like
        //Instead, we will simply check for the overflow condition.
        //Behavior can be observed with different datatypes (signed
        //and unsigned) in modTest.cpp
        if(buffer_pos != 0)
        {
            buffer_pos--;
        }
        else
        {
            buffer_pos = coef_len-1;
        }
    }
}