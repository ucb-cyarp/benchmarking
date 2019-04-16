#include "measurement.h"

std::string MeasurementHelper::HW_Granularity_toString(HW_Granularity granularity){
    switch(granularity){
        case HW_Granularity::SYSTEM:
            return "System";
        case HW_Granularity::SOCKET:
            return "Socket";
        case HW_Granularity::DIE:
            return "Die";
        case HW_Granularity::CORE:
            return "Core";
        case HW_Granularity::THREAD:
            return "Thread";
        case HW_Granularity::NONE:
            return "None";
        default:
            throw std::runtime_error("Unknown HW_Granularity");
            return "Unknown";
    }
}

std::string MeasurementHelper::MeasurmentType_toString(MeasurmentType measurmentType){
    switch (measurmentType)
    {
        case MeasurmentType::DETECT_FREQ_CHANGE:
            return "DetectFreqChange";
        case MeasurmentType::AVG_FREQ:
            return "AvgFreq";
        case MeasurmentType::AVG_ACTIVE_FREQ:
            return "AvgActiveFreq";
        case MeasurmentType::ENERGY_USED_CPU:
            return "EnergyUsedCPU";
        case MeasurmentType::ENERGY_USED_DRAM:
            return "EnergyUsedDRAM";
        case MeasurmentType::AVG_PWR_CPU:
            return "AvgPwrCPU";
        case MeasurmentType::AVG_PWR_DRAM:
            return "AvgPwrDRAM";
        case MeasurmentType::VOLTAGE_CPU:
            return "VoltageCPU";
        case MeasurmentType::VOLTAGE_IO:
            return "VoltageIO";
        case MeasurmentType::TEMPERATURE_CPU:
            return "TemperatureCPU";
        case MeasurmentType::TEMPERATURE_DRAM:
            return "TemperatureDRAM";
        case MeasurmentType::THERMAL_HEADROOM_START:
            return "ThermalHeadroomStart";
        case MeasurmentType::THERMAL_HEADROOM_STOP:
            return "ThermalHeadroomStop";
        default:
            throw std::runtime_error("Unknown MeasurmentType");
            return "Unknown";
    }
}

std::string MeasurementHelper::BaseUnit_toString(BaseUnit baseUnit){
    switch (baseUnit)
    {
        case BaseUnit::SECOND:
            return "Second";
        case BaseUnit::JOULE:
            return "Joule";
        case BaseUnit::WATT:
            return "Watt";
        case BaseUnit::HERTZ:
            return "Hertz";
        case BaseUnit::DEG_CELSIUS:
            return "DegCelsius";
        case BaseUnit::VOLT:
            return "Volt";
        default:
            throw std::runtime_error("Unknown BaseUnit");
            return "Unknown";
    }
}

std::string MeasurementHelper::BaseUnit_abrev(BaseUnit baseUnit){
    switch (baseUnit)
    {
        case BaseUnit::SECOND:
            return "s";
        case BaseUnit::JOULE:
            return "J";
        case BaseUnit::WATT:
            return "W";
        case BaseUnit::HERTZ:
            return "Hz";
        case BaseUnit::DEG_CELSIUS:
            return "DegC";
        case BaseUnit::VOLT:
            return "V";
        default:
            throw std::runtime_error("Unknown BaseUnit");
            return "Unknown";
    }
}

std::string MeasurementHelper::exponentAbrev(int32_t exponent){
    switch(exponent){
        case 15:
            return "P";
        case 12:
            return "T";
        case 9:
            return "G";
        case 6:
            return "M";
        case 3:
            return "k";
        case 0:
            return "";
        case -3:
            return "m";
        case -6:
            return "u";
        case -9:
            return "n";
        case -12:
            return "p";
        case -15:
            return "f";
        default:
            return "e"+std::to_string(exponent);
    }
}