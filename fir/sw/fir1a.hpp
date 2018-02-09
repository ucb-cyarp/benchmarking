#ifndef _H_EDU_BERKELEY_EECS_CYARP_BENCH_FIR1
    #define _H_EDU_BERKELEY_EECS_CYARP_BENCH_FIR1

    #include <cstdlib>
    #include <cstdint>

    //Assuming:
    //    * Number of input/output samples per call to filter is constant
    //    * Coefficients are stored into an array durring the constructor
    //    * Filter initial values are set durring constructor
    //    * Filter topology is direct form
    //    * Value for position in circular buffer

    //Interested in:
    //    * Characteristics of calls to "filter"

    //Not interested in:
    //    * Time required to construct/teardown Fir object
    
    template <typename T, size_t coef_len, size_t io_len> class Fir
    {
        private:
            T working_array[coef_len];
            T coef_array[coef_len];
            size_t buffer_pos;

        public:
            Fir(const T* coef, const T* init);
            void filter(const T* in, T* out);
    };

#endif