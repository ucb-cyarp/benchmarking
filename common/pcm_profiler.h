#ifndef _H_PCM_PROFILER
    #define _H_PCM_PROFILER

    #include "profiler.h"
    #include "cpucounters.h"

    class PCM_Profiler : public Profiler{
        PCM* pcm;

        ServerUncorePowerState* startPowerState;
        ServerUncorePowerState* endPowerState;
        std::vector<CoreCounterState> startCstates, endCstates;
        std::vector<SocketCounterState> startSktstate, endSktstate;
        SystemCounterState startSstate, endSstate;

        public:
            PCM_Profiler();
            ~PCM_Profiler();

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
            bool supportsMultipleInstances() override;
    };

#endif