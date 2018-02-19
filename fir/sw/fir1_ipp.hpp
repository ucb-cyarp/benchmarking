#ifndef _H_EDU_BERKELEY_EECS_CYARP_BENCH_FIR1
    #define _H_EDU_BERKELEY_EECS_CYARP_BENCH_FIR1

    #include <cstdlib>
    #include <ipp.h>

    //Assuming:
    //    * Number of input/output samples per call to filter is constant
    //    * Coefficients are stored into an array durring the constructor
    //    * Filter initial values are set durring constructor
    //    * Filter topology is direct form

    //Interested in:
    //    * Characteristics of calls to "filter"

    //Not interested in:
    //    * Time required to construct/teardown Fir object
    
    template <typename T, size_t coef_len, size_t io_len> class Fir
    {
        private:
            T working_array1[coef_len-1];
            T working_array2[coef_len-1];

            T coef_array[coef_len];

            unsigned char *buf;
            IppsFIRSpec_64f* pSpec;

            T* pDlySrc;
            T* pDlyDst;

        public:
            Fir(const T* coef, const T* init);
            void filter(const T* in, T* out);
    };


    template <size_t coef_len, size_t io_len> class Fir <double, coef_len, io_len>
    {
        private:
            double working_array1[coef_len-1];
            double working_array2[coef_len-1];

            double coef_array[coef_len];

            unsigned char *buf;
            IppsFIRSpec_64f* pSpec;

            double* pDlySrc;
            double* pDlyDst;

        public:
            Fir(const double* coef, const double* init);
            void filter(const double* in, double* out);
    };

    template <size_t coef_len, size_t io_len> class Fir <float, coef_len, io_len>
    {
        private:
            float working_array1[coef_len-1];
            float working_array2[coef_len-1];

            float coef_array[coef_len];

            unsigned char *buf;
            IppsFIRSpec_32f* pSpec;

            float* pDlySrc;
            float* pDlyDst;

        public:
            Fir(const float* coef, const float* init);
            void filter(const float* in, float* out);
    };

#endif