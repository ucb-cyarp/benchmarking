#ifndef _H_EDU_BERKELEY_EECS_CYARP_BENCH_FIR2
    #define _H_EDU_BERKELEY_EECS_CYARP_BENCH_FIR2

    //TODO: Not now

    #include <cstdlib>

    //Assuming:
    //    * Number of input/output samples per call to filter is constant
    //    * Coefficients are passed as template parameters
    //    * Filter initial values are set durring constructor
    //    * Filter topology is direct form

    //Interested in:
    //    * Characteristics of calls to "filter"

    //Not interested in:
    //    * Time required to construct/teardown Fir object
    
    template <typename T, size_t coef_len, size_t io_len, T[coef_len] coef_array> class Fir
    {
        private:
            T working_array[coef_len];

        public:
            void filter(const T* in, T* out);
    };

#endif