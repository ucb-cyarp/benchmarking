#ifndef _H_MEASURMENT
    #define _H_MEASURMENT

    #include <vector>
    #include <map>

    enum class HW_Granularity{
        SYSTEM, ///<Can measure at the global system level
        SOCKET, ///<Can measure at the socket level
        DIE, ///<Can measure at the die level (for multi-die in socket)
        CORE, ///<Can measure at the core level
        THREAD, ///<Can measure at the SMT thread level
        NONE ///<Cannot measure
    };

    enum class MeasurementType{
        DETECT_FREQ_CHANGE,
        AVG_FREQ,
        AVG_ACTIVE_FREQ,
        ENERGY_USED_CPU,
        ENERGY_USED_DRAM,
        AVG_PWR_CPU,
        AVG_PWR_DRAM,
        VOLTAGE_CPU,
        VOLTAGE_IO,
        TEMPERATURE_CPU_START, //Temp at start of benchmark
        TEMPERATURE_CPU_STOP, //Temp at end of benchmark
        TEMPERATURE_DRAM_START,
        TEMPERATURE_DRAM_STOP,
        THERMAL_HEADROOM_START,
        THERMAL_HEADROOM_STOP
    };

    enum class BaseUnit{
        SECOND,
        JOULE,
        WATT,
        HERTZ,
        DEG_CELSIUS,
        VOLT,
        UNITLESS
    };

    class MeasurementHelper{
    public:
        static std::string HW_Granularity_toString(HW_Granularity granularity);
        static std::string MeasurementType_toString(MeasurementType measurementType);
        static std::string BaseUnit_toString(BaseUnit baseUnit);
        static std::string BaseUnit_abrev(BaseUnit baseUnit);
        static std::string exponentAbrev(int32_t exponent);
    };

    const std::vector<MeasurementType> DEFAULT_REPORT_TYPE_LIST = {MeasurementType::AVG_FREQ, 
                                                                  MeasurementType::AVG_ACTIVE_FREQ, 
                                                                  MeasurementType::ENERGY_USED_CPU,
                                                                  MeasurementType::ENERGY_USED_DRAM,
                                                                  MeasurementType::AVG_PWR_CPU,
                                                                  MeasurementType::AVG_PWR_DRAM,
                                                                  MeasurementType::VOLTAGE_CPU,
                                                                  MeasurementType::VOLTAGE_IO,
                                                                  MeasurementType::TEMPERATURE_CPU_START,
                                                                  MeasurementType::TEMPERATURE_CPU_STOP,
                                                                  MeasurementType::TEMPERATURE_DRAM_START,
                                                                  MeasurementType::TEMPERATURE_DRAM_STOP,
                                                                  MeasurementType::THERMAL_HEADROOM_START,
                                                                  MeasurementType::THERMAL_HEADROOM_STOP};

    const std::vector<HW_Granularity> DEFAULT_GRANULARITY_LIST = {HW_Granularity::SYSTEM, 
                                                                  HW_Granularity::SOCKET,
                                                                  HW_Granularity::DIE,
                                                                  HW_Granularity::CORE, 
                                                                  HW_Granularity::THREAD}; 
    
    class Unit{
        public:
        BaseUnit baseUnit; ///The unit (second, joule, ...)
        int32_t exponent; ///< 10^exponent = unit scaling

        Unit();
        Unit(BaseUnit baseUnit, int32_t exponent);
        bool operator==(const Unit &rhs) const;
    };

    class Measurment{
        public:
        int32_t index; ///< The index of the unit this measurement is taken from.  Pertains to HW_Granulatity. With HW_Granulatity CORE, index 0 refers to core 0
        Unit unit; ///< The unit of the measurement
        std::vector<double> measurement; ///<A vector of values for this measurement.  Can possibly contain multiple values if sampled
        std::vector<double> deltaT; ///A vector of the time deltas between measurments (if sampling was used)

        Measurment();
        Measurment(int32_t index, Unit unit, double measurment);
        Measurment(int32_t index, Unit unit, const std::vector<double> &measurment, const std::vector<double> &deltaT);
    };

    class MeasurmentCapabilities{
        public:
        //All benchmarks need the ability to time

        //Benchmarks can be present at multiple levels (ex. energy use may be availible at the socket, die, and )
        std::map<MeasurementType, std::vector<HW_Granularity>> measurmentCapabilities;
    };

#endif