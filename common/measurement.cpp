#include "measurement.h"
#include <cmath>

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

std::string MeasurementHelper::MeasurementType_toString(MeasurementType measurementType){
    switch (measurementType)
    {
        case MeasurementType::DETECT_FREQ_CHANGE:
            return "DetectFreqChange";
        case MeasurementType::AVG_FREQ:
            return "AvgFreq";
        case MeasurementType::AVG_ACTIVE_FREQ:
            return "AvgActiveFreq";
        case MeasurementType::ENERGY_USED_CPU:
            return "EnergyUsedCPU";
        case MeasurementType::ENERGY_USED_DRAM:
            return "EnergyUsedDRAM";
        case MeasurementType::AVG_PWR_CPU:
            return "AvgPwrCPU";
        case MeasurementType::AVG_PWR_DRAM:
            return "AvgPwrDRAM";
        case MeasurementType::VOLTAGE_CPU:
            return "VoltageCPU";
        case MeasurementType::VOLTAGE_IO:
            return "VoltageIO";
        case MeasurementType::CURRENT_CPU:
            return "CurrentCPU";
        case MeasurementType::CURRENT_IO:
            return "CurrentIO";
        case MeasurementType::P_STATE:
            return "PState";
        case MeasurementType::C_STATE:
            return "CState";
        case MeasurementType::TEMPERATURE:
            return "TEMPERATURE";
        case MeasurementType::TEMPERATURE_CPU_START:
            return "TemperatureCPUStart";
        case MeasurementType::TEMPERATURE_CPU_STOP:
            return "TemperatureCPUStop";
        case MeasurementType::TEMPERATURE_DRAM_START:
            return "TemperatureDRAMStart";
        case MeasurementType::TEMPERATURE_DRAM_STOP:
            return "TemperatureDRAMStop";
        case MeasurementType::THERMAL_HEADROOM_START:
            return "ThermalHeadroomStart";
        case MeasurementType::THERMAL_HEADROOM_STOP:
            return "ThermalHeadroomStop";
        default:
            throw std::runtime_error("Unknown MeasurementType");
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
        case BaseUnit::AMP:
            return "Amp";
        case BaseUnit::UNITLESS:
            return "Unitless";
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
        case BaseUnit::AMP:
            return "A";
        case BaseUnit::UNITLESS:
            return "";
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

Unit::Unit() : baseUnit(BaseUnit::SECOND), exponent(0) {}
Unit::Unit(BaseUnit baseUnit, int32_t exponent) : baseUnit(baseUnit), exponent(exponent) {}
bool Unit::operator==(const Unit &rhs) const {
    return baseUnit == rhs.baseUnit &&
           exponent == rhs.exponent;
}

double Unit::scaleFactor(Unit &from, Unit &to){
    if(from.baseUnit != to.baseUnit){
        throw std::runtime_error("Incompatible units found durring unit conversion: " + MeasurementHelper::BaseUnit_toString(from.baseUnit) + " -> " + MeasurementHelper::BaseUnit_toString(to.baseUnit));
    }
    bool needsScaling = from.exponent != to.exponent;
    double scaleFactor = needsScaling ? pow(10.0, from.exponent - to.exponent) : 1;

    return scaleFactor;
}

double Unit::scale(Unit &from, Unit &to, double val){
    double scaleFactor = Unit::scaleFactor(from, to);
    return val*scaleFactor;
}

Measurement::Measurement():index(-1) {}
Measurement::Measurement(int32_t index, Unit unit, MeasurementCollectionType collectionType, double measurement) : index(index), unit(unit), collectionType(collectionType), measurement({measurement}), deltaTUnit(Unit(BaseUnit::SECOND, 0)){}
Measurement::Measurement(int32_t index, Unit unit, MeasurementCollectionType collectionType, const std::vector<double> &measurement, Unit deltaTUnit, const std::vector<double> &deltaT) : index(index), unit(unit), collectionType(collectionType), measurement(measurement), deltaT(deltaT), deltaTUnit(deltaTUnit) {}

MeasurementCollectionPoint::MeasurementCollectionPoint() : measurementType(MeasurementType::AVG_FREQ), granularity(HW_Granularity::NONE), index(-1), collectionType(MeasurementCollectionType::INSTANTANEOUS), unit(Unit(BaseUnit::UNITLESS, 0)) {};
MeasurementCollectionPoint::MeasurementCollectionPoint(MeasurementType &measurementType, HW_Granularity &granularity, int index, MeasurementCollectionType &collectionType, Unit &unit) : measurementType(measurementType), granularity(granularity), index(index), collectionType(collectionType), unit(unit) {};