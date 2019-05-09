#ifndef _H_AMD_UPROF_PROFILER
    #define _H_AMD_UPROF_PROFILER

    #include "profiler.h"

    #include <AMDTPowerProfileApi.h>

    #define SAMPLING_DURATION_WARNING_THRESHOLD 0.1

    class AMDuProfProfiler : public Profiler{
        bool initialized;

        AMDTUInt32 amdCounterSize;
        AMDTPwrCounterDesc* amdCounterDescrs;

        MeasurementCapabilities capabilities;
        std::map<AMDTPwrDeviceId, MeasurementCollectionPoint> amdCounterIDMap;

        //Sampling Parameters
        AMDTUInt32 samplingInterval; //Power Counter sampling interval in milliseconds

        /**
         * Decode AMDTPwrCounterDesc to a MeasurementType.  This is returned through a reference
         */
        static void AMDMeasurmentType2MeasurementType(AMDTPwrCounterDesc amdCounterDesc, MeasurementType &measurementType, bool &foundMeasurementType);
        
        /**
         * Decode AMDTPwrCounterDesc to a HW_Granularity.  This is returned through a reference
         */
        static void AMDDeviceType2HW_Granularity(AMDTPwrCounterDesc amdCounterDesc, HW_Granularity &hwGranularity, bool &foundHwGranularity);

        static MeasurementCollectionType AMDTPwrAggregation2MeasurementCollectionType(AMDTPwrAggregation amdPwrAggregation);

        static void AMDTPwrUnit2Unit(AMDTPwrUnit amdPwrUnit, Unit &unit, bool &validUnit);

        public:
            AMDuProfProfiler();
            ~AMDuProfProfiler();

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
            bool checkFreqChanged() override; ///<Returs true if the profiler detected a frequency change event in the last trial
    };

#endif