#ifndef _H_EDU_BERKELEY_EECS_CYARP_BENCH_FIR5
    #define _H_EDU_BERKELEY_EECS_CYARP_BENCH_FIR5

    //TODO: Not now

    #include <cstdlib>

    //Assuming:
    //    * Number of input/output samples per call to filter is variable
    //    * Coefficients are stored into an array durring the constructor, but size is not known at compile time
    //    * Filter initial values are set durring constructor
    //    * Filter topology is direct form

    //Interested in:
    //    * Characteristics of calls to "filter"

    //Not interested in:
    //    * Time required to construct/teardown Fir object
    
    template <typename T> class Fir
    {
        private:
            T* working_array;
            T* coef_array;

        public:
            Fir(const T* coef, const T* init, size_t coef_len);
            void filter(const T* in, T* out, size_t len);
    };

#endif