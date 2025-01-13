#ifndef __APIDOIP_H__
#define __APIDOIP_H__

#include <variant>
#include <unordered_map>
#include <utility>
#include <optional>

#include "MessageDataTypes.h"
#include "UDSDataType.h"

namespace APIDoIP
{
    constexpr uint8_t dataReqDataSize = 0x05;
    constexpr uint8_t firstReqDynDataSize = 0x06;
    constexpr uint8_t secondReqDynDataSize = 0x0a;

    enum EDoIPRequest
    {
        REQUEST_INVALID = -1,

        DME_ENGINE_OIL_TEMPERATURE = EVENT_DATA_UPDATE_DME_ENGINE_OIL_TEMPERATURE,
        DME_ENGINE_ROTATIONAL_SPEED = EVENT_DATA_UPDATE_DME_ENGINE_ROTATIONAL_SPEED,
        DME_COOLANT_TEMPERATURE = EVENT_DATA_UPDATE_DME_COOLANT_TEMPERATURE,
        DME_BATTERY_VOLTAGE = EVENT_DATA_UPDATE_DME_BATTERY_VOLTAGE,
        DME_AMBIENT_TEMPERATURE = EVENT_DATA_UPDATE_DME_AMBIENT_TEMPERATURE,
        DME_AIR_MASS = EVENT_DATA_UPDATE_DME_AIR_MASS,
        DME_RAIL_PRESSURE = EVENT_DATA_UPDATE_DME_RAIL_PRESSURE,
        DME_ACCELERATOR_PEDAL_POSITION = EVENT_DATA_UPDATE_DME_ACCELERATOR_PEDAL_POSITION,
        
        KOMBI_TOTAL_DISTANCE = EVENT_DATA_UPDATE_KOMBI_TOTAL_DISTANCE,
        KOMBI_SPEED = EVENT_DATA_UPDATE_KOMBI_SPEED,
        KOMBI_OUTSIDE_TEMP_SENSOR = EVENT_DATA_UPDATE_KOMBI_OUTSIDE_TEMP_SENSOR,
        KOMBI_ENGINE_SPEED_ON_DISP = EVENT_DATA_UPDATE_KOMBI_ENGINE_SPEED_ON_DISP,
        KOMBI_FUEL_LEVEL = EVENT_DATA_UPDATE_KOMBI_FUEL,
        
        IHKA_EVAPORATOR_TEMPERATURE_SENSOR = EVENT_DATA_UPDATE_IHKA_EVAPORATOR_TEMPERATURE_SENSOR,
        IHKA_TEMPERATURE_SELECTOR = EVENT_DATA_UPDATE_IHKA_TEMPERATURE_SELECTOR,
    };

    const std::unordered_map<EDoIPRequest, std::pair<std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>, EECUAddress>> dataIdentifierMap =
    {
        // DME
        {DME_ENGINE_OIL_TEMPERATURE, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EDynamicDataIndentifierRequestCode::ENGINE_OIL_TEMPERATURE), EECUAddress::ECU_DME_INTERNAL_ADDR}},
        {DME_ENGINE_ROTATIONAL_SPEED, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EDynamicDataIndentifierRequestCode::ENGINE_SPEED), EECUAddress::ECU_DME_INTERNAL_ADDR}},
        {DME_COOLANT_TEMPERATURE, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EDynamicDataIndentifierRequestCode::COOLANT_TEMPERATURE), EECUAddress::ECU_DME_INTERNAL_ADDR}},
        {DME_BATTERY_VOLTAGE, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EDynamicDataIndentifierRequestCode::BATTERY_VOLTAGE), EECUAddress::ECU_DME_INTERNAL_ADDR}},
        {DME_AMBIENT_TEMPERATURE, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EDynamicDataIndentifierRequestCode::AMBIENT_TEMPERATURE), EECUAddress::ECU_DME_INTERNAL_ADDR}},
        {DME_AIR_MASS, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EDynamicDataIndentifierRequestCode::AIR_MASS), EECUAddress::ECU_DME_INTERNAL_ADDR}},
        {DME_RAIL_PRESSURE, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EDynamicDataIndentifierRequestCode::RAIL_PRESSURE), EECUAddress::ECU_DME_INTERNAL_ADDR}},
        {DME_ACCELERATOR_PEDAL_POSITION, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EDynamicDataIndentifierRequestCode::ACCELERATOR_PEDAL_POSITION), EECUAddress::ECU_DME_INTERNAL_ADDR}},
        // KOMBI
        {KOMBI_TOTAL_DISTANCE, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EUDSDID::KOMBI_DISTANCE), EECUAddress::ECU_KOMBI_INTERNAL_ADDR}},
        {KOMBI_SPEED, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EUDSDID::KOMBI_SPEED), EECUAddress::ECU_KOMBI_INTERNAL_ADDR}},
        {KOMBI_OUTSIDE_TEMP_SENSOR, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EUDSDID::KOMBI_TEMP), EECUAddress::ECU_KOMBI_INTERNAL_ADDR}},
        {KOMBI_ENGINE_SPEED_ON_DISP, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EUDSDID::KOMBI_RPM), EECUAddress::ECU_KOMBI_INTERNAL_ADDR}},
        {KOMBI_FUEL_LEVEL, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EUDSDID::KOMBI_FUEL), EECUAddress::ECU_KOMBI_INTERNAL_ADDR}},
        // IHKA
        {IHKA_EVAPORATOR_TEMPERATURE_SENSOR, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EUDSDID::IHKA_EVAPORATOR), EECUAddress::ECU_IHKA_INTERNAL_ADDR}},
        {IHKA_TEMPERATURE_SELECTOR, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EUDSDID::IHKA_TEMP_SELECTOR), EECUAddress::ECU_IHKA_INTERNAL_ADDR}}
    };

    void prepareDataRequest(uint8_t preparedPayload[], EECUAddress targetEcuAddr, EUDSDID did);
    void prepareFirstRequestForDynamicData(uint8_t preparedPayload[], EECUAddress targetEcuAddr);
    void prepareSecondRequestForDynamicData(uint8_t preparedPayload[], EECUAddress targetEcuAddr, EDynamicDataIndentifierRequestCode ddirCode);

    std::optional<SMessage> extractDataFromResponse(const uint8_t dataPayload[], size_t size, EDoIPRequest dataType);

    const std::unordered_map<EDoIPRequest, const char*> EDoIPRequest_ToCStringMap = 
    {
        {DME_ENGINE_OIL_TEMPERATURE, "DME_ENG_OIL_TEMP"},
        {DME_ENGINE_ROTATIONAL_SPEED, "DME_ENG_ROT_SPEED"},
        {DME_COOLANT_TEMPERATURE, "DME_COOL_TEMP"},
        {DME_BATTERY_VOLTAGE, "DME_BAT_VOL"},
        {DME_AMBIENT_TEMPERATURE, "DME_AMB_TEMP"},
        {DME_AIR_MASS, "DME_AIR_MASS"},
        {DME_RAIL_PRESSURE, "DME_RAIL_PRES"},
        {DME_ACCELERATOR_PEDAL_POSITION, "DME_ACC_PEDAL_POS"},
        {KOMBI_TOTAL_DISTANCE, "KOMBI_DIST"},
        {KOMBI_SPEED, "KOMBI_SPEED"},
        {KOMBI_OUTSIDE_TEMP_SENSOR, "KOMBI_OUT_TEMP"},
        {KOMBI_ENGINE_SPEED_ON_DISP, "KOMBI_ENG_SPEED_DISP"},
        {KOMBI_FUEL_LEVEL, "KOMBI_FUEL_LEVEL"},
        {IHKA_EVAPORATOR_TEMPERATURE_SENSOR, "IHKA_EVAP_TEMP"},
        {IHKA_TEMPERATURE_SELECTOR, "IHKA_TEMP_SEL"}
    };
};

#endif /* __APIDOIP_H__ */