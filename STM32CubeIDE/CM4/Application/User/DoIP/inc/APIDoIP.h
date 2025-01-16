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

    extern std::unordered_map<EDoIPRequest, std::pair<std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>, EECUAddress>> dataIdentifierMap;

    void prepareDataRequest(uint8_t preparedPayload[], EECUAddress targetEcuAddr, EUDSDID did);
    void prepareFirstRequestForDynamicData(uint8_t preparedPayload[], EECUAddress targetEcuAddr);
    void prepareSecondRequestForDynamicData(uint8_t preparedPayload[], EECUAddress targetEcuAddr, EDynamicDataIndentifierRequestCode ddirCode);

    std::optional<SMessage> extractDataFromResponse(const uint8_t dataPayload[], size_t size, EDoIPRequest dataType);
};

#endif /* __APIDOIP_H__ */
