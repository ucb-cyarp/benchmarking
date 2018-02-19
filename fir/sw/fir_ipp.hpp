#include "fir1_ipp.hpp"

/*
 * FIR - Intel Integrated Performance Primitives (IPP)
 * 
 * Based on the "Standard FIR Filtering with a Not-in-place 
 * Destination and Threading" example from 
 * https://software.intel.com/en-us/ipp-dev-reference-examples-of-using-fir-functions
 * This example was chosen because it shows how different call types of the FIR function
 * work.
 * 
 * Documentation can be found at https://software.intel.com/en-us/ipp-dev-reference-fir-filter-functions
 * 
 * Note that the 3 call are required to use the FIR function.
 * The FIR filter can be fed with an optional array which contains the initial state
 * of the filter taps (pDlySrc).  The function can also optionally copy the filter taps
 * into another array to be used in future calls to the function (pDlyDst).
 * 
 * Note: It is unclear if the new tap values are coppied after the function has
 * finished executing.  The examples show two seperate buffers being used.  
 */

template <size_t coef_len, size_t io_len>
Fir<double, coef_len, io_len>::Fir(const double* coef, const double* init)
{
    for(size_t i = 0; i<coef_len; i++)
    {
        coef_array[i] = coef[i];
    }

    for(size_t i = 0; i<coef_len-1; i++)
    {
        working_array1[i] = init[i];
    }

    pDlySrc = working_array1;
    pDlyDst = working_array2;

    int specSize, bufSize;

    ippsFIRSRGetSize(coef_len, ipp64f, &specSize, &bufSize);

    pSpec  = (IppsFIRSpec_64f*)ippsMalloc_8u(specSize);
    buf    = ippsMalloc_8u(bufSize);

    ippsFIRSRInit_64f(coef_array, coef_len, ippAlgDirect, pSpec);
}

template <size_t coef_len, size_t io_len>
void Fir<double, coef_len, io_len>::filter(const double* in, double* out)
{
    ippsFIRSR_64f(in, out, io_len, pSpec, pDlySrc, pDlyDst, buf);

    //Swap pointer
    double* tmp = pDlySrc;
    pDlySrc = pDlyDst;
    pDlyDst = tmp;
}

template <size_t coef_len, size_t io_len>
Fir<float, coef_len, io_len>::Fir(const float* coef, const float* init)
{
    for(size_t i = 0; i<coef_len; i++)
    {
        coef_array[i] = coef[i];
    }

    for(size_t i = 0; i<coef_len-1; i++)
    {
        working_array1[i] = init[i];
    }

    pDlySrc = working_array1;
    pDlyDst = working_array2;

    int specSize, bufSize;

    ippsFIRSRGetSize(coef_len, ipp32f, &specSize, &bufSize);

    pSpec  = (IppsFIRSpec_32f*)ippsMalloc_8u(specSize);
    buf    = ippsMalloc_8u(bufSize);

    ippsFIRSRInit_32f(coef_array, coef_len, ippAlgDirect, pSpec);
}

template <size_t coef_len, size_t io_len>
void Fir<float, coef_len, io_len>::filter(const float* in, float* out)
{
    ippsFIRSR_32f(in, out, io_len, pSpec, pDlySrc, pDlyDst, buf);

    //Swap pointer
    float* tmp = pDlySrc;
    pDlySrc = pDlyDst;
    pDlyDst = tmp;
}