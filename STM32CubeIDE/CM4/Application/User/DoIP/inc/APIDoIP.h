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

        ECU_DATA_START = EVENT_DATA_UPDATE_START,
        BATTERY_VOLTAGE = EVENT_DATA_UPDATE_BATTERY_VOLTAGE,
        MOTOR_TEMPERATURE = EVENT_DATA_UPDATE_MOTOR_TEMPERATURE,
        AIR_MASS = EVENT_DATA_UPDATE_AIR_MASS,
        INTAKE_AIR_TEMPERATURE = EVENT_DATA_UPDATE_INTAKE_AIR_TEMPERATURE,
        BOOST_PRESSURE_SET = EVENT_DATA_UPDATE_BOOST_PRESSURE_SET,
        BOOST_PRESSURE_ACT = EVENT_DATA_UPDATE_BOOST_PRESSURE_ACT,
        RAIL_PRESSURE_SET = EVENT_DATA_UPDATE_RAIL_PRESSURE_SET,
        RAIL_PRESSURE_ACT = EVENT_DATA_UPDATE_RAIL_PRESSURE_ACT,
        AIR_MASS_SET = EVENT_DATA_UPDATE_AIR_MASS_SET,
        AIR_MASS_ACT = EVENT_DATA_UPDATE_AIR_MASS_ACT,
        AMBIENT_PRESSURE = EVENT_DATA_UPDATE_AMBIENT_PRESSURE,
        FUEL_TEMPERATURE = EVENT_DATA_UPDATE_FUEL_TEMPERATURE,
        TEMP_BEFORE_PART_FILTER = EVENT_DATA_UPDATE_TEMP_BEFORE_PART_FILTER,
        TEMP_BEFORE_CATALYST = EVENT_DATA_UPDATE_TEMP_BEFORE_CATALYST,
        PART_FILTER_DIST_SINCE_REGEN = EVENT_DATA_UPDATE_PART_FILTER_DIST_SINCE_REGEN,
        EXHAUST_BACK_PRESSURE = EVENT_DATA_UPDATE_EXHAUST_BACK_PRESSURE,
        OIL_PRESSURE_SWITCH = EVENT_DATA_UPDATE_OIL_PRESSURE_SWITCH,
        PART_FILTER_REGENERATION_REQUEST = EVENT_DATA_UPDATE_PART_FILTER_REGENERATION_REQUEST,
        PART_FILTER_REGENERATION = EVENT_DATA_UPDATE_PART_FILTER_REGENERATION,
        PART_FILTER_UNBLOCK_REGENERATION = EVENT_DATA_UPDATE_PART_FILTER_UNBLOCK_REGENERATION,
        BREAK_LIGHT = EVENT_DATA_UPDATE_BREAK_LIGHT,
        RPM = EVENT_DATA_UPDATE_RPM,
        SPEED = EVENT_DATA_UPDATE_SPEED,
        ACCELERATION = EVENT_DATA_UPDATE_ACCELERATION,
        OIL_TEMPERATURE = EVENT_DATA_UPDATE_OIL_TEMPERATURE,
        ECU_DATA_END = EVENT_DATA_UPDATE_END
    };

    const std::unordered_map<EDoIPRequest, std::pair<std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>, EECUAddress>> dataIdentifierMap =
    {
        {BATTERY_VOLTAGE, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EDynamicDataIndentifierRequestCode::BATTERY_VOLTAGE), EECUAddress::ECU_DME_INTERNAL_ADDR}},
        {RPM, {std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>(EUDSDID::KOMBI_RPM), EECUAddress::ECU_KOMBI_INTERNAL_ADDR}}
    };

    void prepareDataRequest(uint8_t preparedPayload[], EECUAddress targetEcuAddr, EUDSDID did);
    void prepareFirstRequestForDynamicData(uint8_t preparedPayload[], EECUAddress targetEcuAddr);
    void prepareSecondRequestForDynamicData(uint8_t preparedPayload[], EECUAddress targetEcuAddr, EDynamicDataIndentifierRequestCode ddirCode);

    std::optional<SMessage> extractDataFromResponse(const uint8_t dataPayload[], size_t size, EDoIPRequest dataType);
};

#endif /* __APIDOIP_H__ */