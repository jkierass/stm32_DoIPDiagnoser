#include "APIDoIP.h"
#include "Logger.h"

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
    preparedPayload[9] = static_cast<uint8_t>(DynamicallyAssignDataSubSID::REQUEST_CLEAR);

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
        case BATTERY_VOLTAGE:
        {
            if(size != 1)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_BATTERY_VOLTAGE;
                msg.message_data.battery_voltage = dataPayload[0];
            }
            break;
        }
        case RPM:
        {
            if(size != 2)
            {
                sizeMismatch = true;
            }
            else
            {
                msg.event_type = EVENT_DATA_UPDATE_RPM;
                msg.message_data.rpm = (dataPayload[0] << 8) | dataPayload[1];
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