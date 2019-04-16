#ifndef _H_MEASURMENT
    #define _H_MEASURMENT

    #include <vector>
    #include <map>

    class MeasurementHelper{
        static std::string HW_Granularity_toString(HW_Granularity granularity);
        static std::string MeasurmentType_toString(MeasurmentType measurmentType);
        static std::string BaseUnit_toString(BaseUnit baseUnit);
        static std::string BaseUnit_abrev(BaseUnit baseUnit);
        static std::string exponentAbrev(int32_t exponent);
    };

    enum class HW_Granularity{
        SYSTEM, ///<Can measure at the global system level
        SOCKET, ///<Can measure at the socket level
        DIE, ///<Can measure at the die level (for multi-die in socket)
        CORE, ///<Can measure at the core level
        THREAD, ///<Can measure at the SMT thread level
        NONE ///<Cannot measure
    };

    enum class MeasurmentType{
        DETECT_FREQ_CHANGE,
        AVG_FREQ,
        AVG_ACTIVE_FREQ,
        ENERGY_USED_CPU,
        ENERGY_USED_DRAM,
        AVG_PWR_CPU,
        AVG_PWR_DRAM,
        VOLTAGE_CPU,
        VOLTAGE_IO,
        TEMPERATURE_CPU,
        TEMPERATURE_DRAM,
        THERMAL_HEADROOM_START,
        THERMAL_HEADROOM_STOP
    };
    
    enum class BaseUnit{
        SECOND,
        JOULE,
        WATT,
        HERTZ,
        DEG_CELSIUS,
        VOLT
    };

    class Unit{
        public:
        BaseUnit BaseUnit; ///
        int32_t exponent; ///< 10^exponent = unit scaling
    };

    class Measurment{
        public:
        int32_t index; ///< The index of the unit this measurement is taken from.  Pertains to HW_Granulatity. With HW_Granulatity CORE, index 0 refers to core 0
        Unit unit; ///< The unit of the measurement
        std::vector<double> measurement; ///<A vector of values for this measurement.  Can possibly contain multiple values if sampled
        std::vector<double> deltaT; ///A vector of the time deltas between measurments (if sampling was used)
    };

    class MeasurmentCapabilities{
        public:
        //All benchmarks need the ability to time

        //Benchmarks can be present at multiple levels (ex. energy use may be availible at the socket, die, and )
        std::map<MeasurmentType, std::vector<HW_Granularity>> measurmentCapabilities;
    };

#endif