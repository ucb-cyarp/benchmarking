#ifndef _H_GENERIC_PROFILER
    #define _H_GENERIC_PROFILER

    #include "profiler.h"

    class GenericProfiler : public Profiler{
        public:
            GenericProfiler();
            ~GenericProfiler();

            std::string profilerName() override;

            MeasurementCapabilities findMeasurementCapabilities() override;
            bool checkProfiler() override;

            void init() override;
            void trialSetup() override;
            void startTrialPowerProfile() override;
            void endTrialPowerProfile() override;
            void interTrialReset() override;
            TrialResult computeTrialResult() override;

            bool detectsFreqChange() override; ///<Returns true if the profiler can detect a frequency change event
            bool checkFreqChanged(std::vector<int> socketsOfInterest) override; ///<Returs true if the profiler detected a frequency change event in the last trial
    };

#endif