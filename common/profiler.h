#ifndef _H_PROFILER
    #define _H_PROFILER
    
    #include <string>
    #include <vector>
    #include "results.h"
    #include "measurement.h"
    #include <chrono>

    // //Only include PCM headers if PCM is used
    // #if USE_PCM == 1
    //     #include "cpucounters.h"
    // #endif

    // #if USE_AMD_UPROF == 1
    //     #include "AMDTPowerProfileApi.h"
    // #endif

    //This class describes a generic profiler which replaces calls to specific profiler libraries (such as PCM or AMDuProf)

    

    class Profiler{
        private:
        std::chrono::high_resolution_clock::time_point start_hrc;
        std::chrono::high_resolution_clock::time_point stop_hrc;
        clock_t start_clock;
        clock_t stop_clock;
        uint64_t start_rdtsc;
        uint64_t stop_rdtsc;

        public:
        enum class CPUVendor{
            INTEL, ///<Intel
            AMD, ///<AMD
            UNKNOWN ///<No profiler is used
        };

        enum class EnergyProfiler{
            PCM, ///<Used for Intel
            AMDuPROF, ///<Used for AMD
            NONE ///<No profiler is used
        };

        enum class OS{
            BSD,
            UNIX,
            LINUX,
            MACOS,
            WINDOWS,
            UNKNOWN
        };

        Results results;

        static CPUVendor findCPUVendor(); ///< Find CPU vendor using CPUID
        static std::string findCPUModelStr(); ///<Find CPU vendor string
        static OS findOS(); ///<Find the OS currently being used

        static std::string CPUVendor_toString();
        static std::string EnergyProfiler_toString();
        static std::string OS_toString();

        Profiler();
        ~Profiler();

        virtual bool checkProfiler() = 0; ///<Check that the profiler (subclass) works on the given processor and OS
        virtual MeasurmentCapabilities findMeasurementCapabilities() = 0; ///<Determine what can be measured

        virtual bool isSampling(); ///<Returns true if the driver uses a sampling methodology
        virtual int64_t samplingPeriod(); ///<Returns the sampling period

        void startTrialTimer();
        void endTrialTimer();

        virtual void init() = 0; ///<Initialize the profiler lib (if nessisary)
        virtual void startTrial(); ///<Start a trial (record counter values)
        virtual void endTrial(); ///<End the trial (record counter values)
        virtual void startTrialPowerProfile() = 0; ///<Start a trial (record counter values)
        virtual void endTrialPowerProfile() = 0; ///<End the trial (record counter values)
        virtual void interTrialReset() = 0; ///<Reset between trials (if required)
        virtual TrialResult computeTrialResult(); ///<Compute the trial result for the last trial
        virtual TrialResult* computeTrialResultAndAddToResults();///<Compute the trial result and add to the results.  Returns trialResult if added, null otherwise

        virtual bool detectsFreqChange() = 0; ///<Returns true if the profiler can detect a frequency change event
        virtual bool checkFreqChanged() = 0; ///<Returs true if the profiler detected a frequency change event in the last trial
    };

#endif