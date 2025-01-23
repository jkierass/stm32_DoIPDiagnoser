#include "APIDoIP.h"
#include "Logger.h"

using namespace APIDoIP;

std::unordered_map<APIDoIP::EDoIPRequest, std::pair<std::variant<EUDSDID, EDynamicDataIndentifierRequestCode>, EECUAddress>> APIDoIP::dataIdentifierMap =
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

void APIDoIP::prepareDataRequest(uint8_t preparedPayload[], EECUAddress targetEcuAddr, EUDSDID did)
{
    // data len
    preparedPayload[0] = 0x00;
    preparedPayload[1] = 0x00;
    preparedPayload[2] = 0x00;
    preparedPayload[3] = dataReqDataSize;

    // "checksum"
    preparedPayload[4] = 0x00;
    preparedPayload[5] = 0x01;

    // addresses
    preparedPayload[6] = DIAGNOSTIC_TOOL_INTERNAL_ADDR;
    preparedPayload[7] = static_cast<uint8_t>(targetEcuAddr);

    // SID
    preparedPayload[8] = static_cast<uint8_t>(EUDSRequestSID::READ_DATA_BY_IDENTIFIER);

    // DID
    preparedPayload[9] = static_cast<uint8_t>((static_cast<uint16_t>(did) & 0xFF00) >> 8);
    preparedPayload[10] = static_cast<uint8_t>(static_cast<uint16_t>(did) & 0x00FF);
}

void APIDoIP::prepareFirstRequestForDynamicData(uint8_t preparedPayload[], EECUAddress targetEcuAddr)
{
    // data len
    preparedPayload[0] = 0x00;
    preparedPayload[1] = 0x00;
    preparedPayload[2] = 0x00;
    preparedPayload[3] = firstReqDynDataSize;

    // "checksum"
    preparedPayload[4] = 0x00;
    preparedPayload[5] = 0x01;

    // addresses
    preparedPayload[6] = DIAGNOSTIC_TOOL_INTERNAL_ADDR;
    preparedPayload[7] = static_cast<uint8_t>(targetEcuAddr);

    // SID
    preparedPayload[8] = static_cast<uint8_t>(EUDSRequestSID::DYNAMICALLY_DEFINE_DATA_IDENTIFIER);
    // sub SID
    preparedPayload[9] = static_cast<uint8_t>(DynamicallyAssignDataSubSID::REQUEST_FIRST);

    // DID
    preparedPayload[10] = static_cast<uint8_t>((static_cast<uint16_t>(EUDSDID::DYNAMICALLY_DEFINED_DATA_IDENTIFIER_0) & 0xFF00) >> 8);
    preparedPayload[11] = static_cast<uint8_t>(static_cast<uint16_t>(EUDSDID::DYNAMICALLY_DEFINED_DATA_IDENTIFIER_0) & 0x00FF);
}

void APIDoIP::prepareSecondRequestForDynamicData(uint8_t preparedPayload[], EECUAddress targetEcuAddr, EDynamicDataIndentifierRequestCode ddirCode)
{
    // data len
    preparedPayload[0] = 0x00;
    preparedPayload[1] = 0x00;
    preparedPayload[2] = 0x00;
    preparedPayload[3] = secondReqDynDataSize;

    // "checksum"
    preparedPayload[4] = 0x00;
    preparedPayload[5] = 0x01;

    // addresses
    preparedPayload[6] = DIAGNOSTIC_TOOL_INTERNAL_ADDR;
    preparedPayload[7] = static_cast<uint8_t>(targetEcuAddr);

    // SID
    preparedPayload[8] = static_cast<uint8_t>(EUDSRequestSID::DYNAMICALLY_DEFINE_DATA_IDENTIFIER);
    // sub SID
    preparedPayload[9] = static_cast<uint8_t>(DynamicallyAssignDataSubSID::REQUEST_ASSIGN);;

    // DID
    preparedPayload[10] = static_cast<uint8_t>((static_cast<uint16_t>(EUDSDID::DYNAMICALLY_DEFINED_DATA_IDENTIFIER_0) & 0xFF00) >> 8);
    preparedPayload[11] = static_cast<uint8_t>(static_cast<uint16_t>(EUDSDID::DYNAMICALLY_DEFINED_DATA_IDENTIFIER_0) & 0x00FF);

    // Dynamic data identifier request code
    preparedPayload[12] = static_cast<uint8_t>((static_cast<uint32_t>(ddirCode) & 0xFF000000) >> 24);
    preparedPayload[13] = static_cast<uint8_t>((static_cast<uint32_t>(ddirCode) & 0x00FF0000) >> 16);
    preparedPayload[14] = static_cast<uint8_t>((static_cast<uint32_t>(ddirCode) & 0x0000FF00) >> 8);
    preparedPayload[15] = static_cast<uint8_t>(static_cast<uint32_t>(ddirCode) & 0x000000FF);
}

std::optional<SMessage> APIDoIP::extractDataFromResponse(const uint8_t dataPayload[], size_t size, EDoIPRequest dataType)
{
    bool sizeMismatch = false;
    SMessage msg;
    msg.event_sender = EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER;
    msg.event_receiver = EVENT_CLIENT_FRONTEND;

    switch(dataType)
    {
        case DME_ENGINE_OIL_TEMPERATURE:
        {
            if(size < 2)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_DME_ENGINE_OIL_TEMPERATURE;
                uint16_t engineOilTemperatureRaw = (dataPayload[0] << 8) | dataPayload[1];
                msg.message_data.dme_engine_oil_temperature = static_cast<float>(engineOilTemperatureRaw) * 0.10f;
            }
            break;
        }
        case DME_ENGINE_ROTATIONAL_SPEED:
        {
            if(size < 2)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_DME_ENGINE_ROTATIONAL_SPEED;
                msg.message_data.dme_engine_rotational_speed = (dataPayload[0] << 8) | dataPayload[1];
            }
            break;
        }
        case DME_COOLANT_TEMPERATURE:
        {
            if(size < 1)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_DME_COOLANT_TEMPERATURE;
                msg.message_data.dme_coolant_temperature = static_cast<float>(dataPayload[0])*0.75f - 48.00f;
            }
            break;
        }
        case DME_BATTERY_VOLTAGE:
        {
            if(size < 1)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_DME_BATTERY_VOLTAGE;
                msg.message_data.dme_battery_voltage = static_cast<float>(dataPayload[0])*0.102f - 0.039f;
            }
            break;
        }
        case DME_AMBIENT_TEMPERATURE:
        {
            if(size < 1)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_DME_AMBIENT_TEMPERATURE;
                msg.message_data.dme_ambient_temperature = static_cast<float>(dataPayload[0]) * 0.75f - 48.0f;
            }
            break;
        }
        case DME_AIR_MASS:
        {
            if(size < 2)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_DME_AIR_MASS;
                uint16_t rawAirMass = (dataPayload[0] << 8) | dataPayload[1];
                msg.message_data.dme_air_mass = static_cast<float>(rawAirMass) * 0.03f + 0.59f;
            }
            break;
        }
        case DME_RAIL_PRESSURE:
        {
            if(size < 2)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_DME_RAIL_PRESSURE;
                uint16_t rawRailPressure = (dataPayload[0] << 8) | dataPayload[1];
                msg.message_data.dme_rail_pressure = rawRailPressure * 5.015f + 2669.34f;
            }
            break;
        }
        case DME_ACCELERATOR_PEDAL_POSITION:
        {
            if(size < 1)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_DME_ACCELERATOR_PEDAL_POSITION;
                msg.message_data.dme_accelerator_pedal_position = static_cast<float>(dataPayload[0]) / 2.55f;
            }
            break;
        }
        case KOMBI_TOTAL_DISTANCE:
        {
            if(size < 4)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_KOMBI_TOTAL_DISTANCE;
                msg.message_data.kombi_total_distance = (dataPayload[0] << 24) | (dataPayload[1] << 16) | (dataPayload[2] << 8) | dataPayload[3];
            }
            break;
        }
        case KOMBI_SPEED:
        {
            if(size < 2)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_KOMBI_SPEED;
                uint16_t rawSpeed = (dataPayload[0] << 8) | dataPayload[1];
                msg.message_data.kombi_speed = static_cast<float>(rawSpeed) * 0.056f;
            }
            break;
        }
        case KOMBI_OUTSIDE_TEMP_SENSOR:
        {
            if(size < 2)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_KOMBI_OUTSIDE_TEMP_SENSOR;
                uint16_t rawOutsideTemp = (dataPayload[0] << 8) | dataPayload[1];
                msg.message_data.kombi_outside_temp_sensor = static_cast<float>(rawOutsideTemp);
            }
            break;
        }
        case KOMBI_ENGINE_SPEED_ON_DISP:
        {
            if(size < 2)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_KOMBI_ENGINE_SPEED_ON_DISP;
                msg.message_data.kombi_engine_speed_on_disp = (dataPayload[0] << 8) | dataPayload[1];
            }
            break;
        }
        case KOMBI_FUEL_LEVEL:
        {
            if(size < 9)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_KOMBI_FUEL;
                msg.message_data.kombi_fuel_level[0] = (dataPayload[0] << 16) | (dataPayload[1] << 8) | dataPayload[2];
                msg.message_data.kombi_fuel_level[1] = (dataPayload[3] << 16) | (dataPayload[4] << 8) | dataPayload[5];
                msg.message_data.kombi_fuel_level[2] = (dataPayload[6] << 16) | (dataPayload[7] << 8) | dataPayload[8];
            }
            break;
        }
        case IHKA_EVAPORATOR_TEMPERATURE_SENSOR:
        {
            if(size < 2)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_IHKA_EVAPORATOR_TEMPERATURE_SENSOR;
                uint16_t rawEvap = (dataPayload[0] << 8) | dataPayload[1];
                msg.message_data.ihka_evaporator_temperature_sensor = static_cast<float>(rawEvap) * 0.2f - 10.00f;
            }
            break;
        }
        case IHKA_TEMPERATURE_SELECTOR:
        {
            if(size < 4)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_IHKA_TEMPERATURE_SELECTOR;
                //left
                msg.message_data.ihka_temperature_selector[0] = (dataPayload[0] << 8) | dataPayload[1];
                //right
                msg.message_data.ihka_temperature_selector[1] = (dataPayload[2] << 8) | dataPayload[3];
            }
            break;
        }
        default:
            // return, because if data was successfully read then we always need to send a connection event (and in this case we have not)
            return std::nullopt;
    }

    if(sizeMismatch)
    {
        LOG_DEBUG("Mismatching data size! Read data by identifier could not be completed!");
        return std::nullopt;
    }
    else
    {
        return std::optional<SMessage>(msg);
    }
}
