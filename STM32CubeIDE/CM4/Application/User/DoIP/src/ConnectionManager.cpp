#include "ConnectionManager.h"
#include "Logger.h"

#include "ethernetif.h"

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "projdefs.h"

#include "lwip/etharp.h"

#include <cstring>

using namespace APIDoIP;

extern QueueHandle_t queueToEventManagerCM4;
extern QueueHandle_t connectionEventsQueue;

std::unordered_map<EDoIPRequest, const char*> EDoIPRequest_ToCStringMap =
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

static float getSystickInSeconds()
{
    return static_cast<float>(pdTICKS_TO_MS(xTaskGetTickCount())) / 1000.0f;
}

extern "C" err_t tcpRecvCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    ConnectionManager& connMgrInstance = ConnectionManager::instance();
    connMgrInstance.HandleTcpRecvCb(arg, tpcb, p, err);
    return ERR_OK;
}

extern "C" err_t tcpConnectCb(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    ConnectionManager& connMgrInstance = ConnectionManager::instance();
    connMgrInstance.HandleTcpConnectCb(arg, tpcb, err);
    return ERR_OK;
}

extern "C" void tcpErrCb(void *arg, err_t err)
{
    ConnectionManager& connMgrInstance = ConnectionManager::instance();
    connMgrInstance.HandleTcpErrCb(arg, err);
}

extern "C" void udpRecvCb(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    ConnectionManager& connMgrInstance = ConnectionManager::instance();
    connMgrInstance.HandleUdpRecvCb(arg, pcb, p, addr, port);
}

ConnectionManager::ConnectionManager()
{
    MX_LWIP_Init();
}

bool ConnectionManager::sendSSDP()
{
    constexpr const char *ssdpRequest =
        "M-SEARCH * HTTP/1.1\r\n"
        "HOST: 239.255.255.250:1900\r\n"
        "MAN: \"ssdp:discover\"\r\n"
        "ST: ssdp:all\r\n"
        "MX: 3\r\n"
        "\r\n";

    ip_addr_t multicastIp;
    struct pbuf *p;
    
    // Set the multicast address for SSDP
    ipaddr_aton(SSDP_MULTICAST_IP, &multicastIp);

    // Allocate buffer for SSDP data
    p = pbuf_alloc(PBUF_TRANSPORT, strlen(ssdpRequest), PBUF_RAM);
    if (!p)
    {
        LOG_DEBUG("Failed to allocate pbuf for SSDP.");
        return false;
    }

    // Copy data to the buffer
    memcpy(p->payload, ssdpRequest, strlen(ssdpRequest));

    // Send SSDP
    if (udp_sendto(udp_pcb, p, &multicastIp, 1900) != ERR_OK)
    {
        LOG_DEBUG("Failed to send SSDP request.");
        pbuf_free(p);
        return false;
    }

    // Release the buffer
    pbuf_free(p);
    return true;
}

bool ConnectionManager::sendSLP()
{
    uint8_t slpRequest[36] =
    {
        0x02, 0x06, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00,  // Header
        0x00, 0x00, 0x00, 0x01,                          // XID set to 1 (big-endian)
        0x00, 0x02,                                      // Service Type (srvtype)
        0x65, 0x6e, 0x00, 0x00, 0x00, 0x00,              // Lang Tag (en)
        0x00, 0x07, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c,  // Service (default)
        0x74, 0x00, 0x00, 0x00, 0x00                     // Padding
    };
    ip_addr_t broadcastIp;
    struct pbuf *p;

    // Set the broadcast address
    ipaddr_aton(BROADCAST_IP, &broadcastIp);

    // Allocate buffer for SLP data
    p = pbuf_alloc(PBUF_TRANSPORT, sizeof(slpRequest), PBUF_RAM);
    if (!p)
    {
        LOG_DEBUG("Failed to allocate pbuf for SLP.");
        return false;
    }

    // Copy data to the buffer
    memcpy(p->payload, slpRequest, sizeof(slpRequest));

    // Send SLP
    if (udp_sendto(udp_pcb, p, &broadcastIp, SLP_PORT) != ERR_OK)
    {
        LOG_DEBUG("Failed to send SLP request.");
        pbuf_free(p);
        return false;
    }

    // Release the buffer
    pbuf_free(p);
    return true;
}


bool ConnectionManager::sendDoIPInit()
{
    const char doipRequest[] = "\x00\x00\x00\x00\x00\x11";
    ip_addr_t broadcastIp;
    struct pbuf *p;

    // Set the broadcast address for DoIP
    ipaddr_aton(BROADCAST_IP, &broadcastIp);

    // Allocate buffer for DoIP data
    p = pbuf_alloc(PBUF_TRANSPORT, sizeof(doipRequest) - 1, PBUF_RAM);
    if (!p)
    {
        LOG_DEBUG("Failed to allocate pbuf for DoIP init.");
        return false;
    }

    // Copy data to the buffer
    memcpy(p->payload, doipRequest, sizeof(doipRequest) - 1);

    // Send DoIP
    if (udp_sendto(udp_pcb, p, &broadcastIp, UDP_PORT) != ERR_OK)
    {
        LOG_DEBUG("Failed to send DoIP init request.");
        pbuf_free(p);
        return false;
    }

    // Release the buffer
    pbuf_free(p);
    return true;
}

void ConnectionManager::HandleUdpRecvCb(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    if (!p)
    {
        LOG_DEBUG("No data received, connection might be closed.");

        return;
    }

    LOG_DEBUG("Received UDP message from %s:%d", ipaddr_ntoa(addr), port);

    // Validate payload length
    if (p->len < 6)
    {
        LOG_DEBUG("Payload too short to process (length: %d bytes).", p->len);
        pbuf_free(p);
        return;
    }

    // Cast payload to a uint8_t array for easier processing
    uint8_t *payload = (uint8_t *)p->payload;

    // Validate frame type based on first 4 bytes (we expected a specific DoIP identification frame)
    if (!(payload[0] == 0x00 && payload[1] == 0x00 && payload[2] == 0x00 && payload[3] == 0x32))
    {
        LOG_DEBUG("Unexpected frame type received, ignoring.");
        pbuf_free(p);
        return;
    }

    // Extract length (first 4 bytes) and validate against p->len
    uint32_t dataLength = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    // 6 bytes for header (length + type)
    if (dataLength + 6 > p->len)
    {
        LOG_DEBUG("Data length mismatch: declared %u, received %u.", dataLength, p->len);
        pbuf_free(p);
        return;
    }

    // Process diagnostic data if available
    if (p->len > 14)
    {
        uint8_t *diagnosticData = &payload[14];
        size_t diagnosticDataLen = p->len - 14;

        // Search for MAC and VIN keywords
        char *macStart = (char *)strstr((char *)diagnosticData, "MAC");
        char *vinStart = (char *)strstr((char *)diagnosticData, "VIN");

        if (macStart && vinStart)
        {
            macStart += strlen("MAC");
            vinStart += strlen("VIN");

            // Safely extract MAC Address (12 characters)
            if (macStart + 12 <= (char *)diagnosticData + diagnosticDataLen)
            {
                char macAddressStr[13] = {0};
                strncpy(macAddressStr, macStart, 12);

                LOG_DEBUG("Extracted MAC address: %s", macAddressStr);

                uint8_t ecuMac[6] = {0};
                for (int i = 0; i < 6; i++)
                {
                    char byteStr[3] = { macAddressStr[i * 2], macAddressStr[i * 2 + 1], '\0' };
                    char *endPtr;
                    long byteValue = strtol(byteStr, &endPtr, 16);

                    if (*endPtr != '\0' || byteValue < 0 || byteValue > 0xFF)
                    {
                        LOG_DEBUG("Failed to parse MAC address byte: %s", byteStr);
                        pbuf_free(p);
                        return;
                    }

                    ecuMac[i] = static_cast<uint8_t>(byteValue);
                }

                // Add a static ARP entry for ECU
                struct eth_addr ethMac;
                memcpy(ethMac.addr, ecuMac, 6);
                if (etharp_add_static_entry(addr, &ethMac) == ERR_OK)
                {
                    LOG_DEBUG("ARP entry for ECU added successfully.");
                    ecu_ip_addr = *addr;
                }
                else
                {
                    LOG_DEBUG("Failed to add ARP entry for ECU.");
                }
            }
            else
            {
                LOG_DEBUG("MAC address extraction exceeds data boundaries.");
            }

            // Safely extract VIN (17 characters)
            if (vinStart + 17 <= (char *)diagnosticData + diagnosticDataLen)
            {
                char vin[18] = {0};
                strncpy(vin, vinStart, 17);
                LOG_DEBUG("Extracted VIN: %s", vin);

                EConnectionEvent connEvent = EVENT_DISCOVERY_COMPLETED;
                if(xQueueSend(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(10000)) != pdPASS)
                {
                    //In this case do not notify app and just wait for another message from ecu to try on the next call
                    LOG_DEBUG("[FATAL ERROR] Could not send internal connection event: %d", connEvent);
                    pbuf_free(p);
                    return;
                }
                
                SMessage msg;
                msg.event_type = EVENT_ECU_CONNECTION_INITIALISED;
                msg.event_sender = EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER;
                msg.event_receiver = EVENT_CLIENT_FRONTEND;
                strncpy(msg.message_data.ecu_connected_vin, vinStart, 17);
                xQueueSend(queueToEventManagerCM4, &(msg), static_cast<TickType_t>(10000));
            }
            else
            {
                LOG_DEBUG("VIN extraction exceeds data boundaries.");
            }
        }
        else
        {
            LOG_DEBUG("MAC or VIN markers not found in diagnostic data.");
        }
    }
    else
    {
        LOG_DEBUG("Payload too short to contain diagnostic data (length: %d bytes).", p->len);
    }

    // Free the received buffer
    pbuf_free(p);
}

void ConnectionManager::udpConnect()
{
    udp_pcb = udp_new();
    if (udp_pcb)
    {
        udp_bind(udp_pcb, IP_ADDR_ANY, 0);  // Bind PCB with any port
        udp_recv(udp_pcb, udpRecvCb, NULL);  // Register the UDP receiver callback (listen for car info)
    }
    else
    {
        LOG_DEBUG("Failed to create UDP PCB.");
    }
}


void ConnectionManager::udpDisconnect()
{
    if (udp_pcb != nullptr)
    {
        // Unregister callbacks
        udp_recv(udp_pcb, nullptr, nullptr); // Wyrejestrowanie callbacka
        // Close connection
        udp_remove(udp_pcb);
        udp_pcb = nullptr;
        LOG_DEBUG("UDP PCB removed and callback unregistered.");
    }
    else
    {
        LOG_DEBUG("udp_pcb is already null, skipping removal.");
    }
}

bool ConnectionManager::tcpConnect()
{
    LOCK_TCPIP_CORE();
	tcp_pcb = tcp_new();
    if (!tcp_pcb)
    {
        LOG_DEBUG("Failed to create TCP PCB.");
        UNLOCK_TCPIP_CORE();
        return false;
    }

    tcp_nagle_disable(tcp_pcb);

    err_t bindErr = tcp_bind(tcp_pcb, IP_ADDR_ANY, 0); // Automatically assign port number
    if (bindErr != ERR_OK)
    {
        LOG_DEBUG("Failed to bind TCP PCB: %d", bindErr);
        tcpDisconnect();
        UNLOCK_TCPIP_CORE();
        return false;
    }
    // callback registration
    tcp_arg(tcp_pcb, NULL);
    tcp_recv(tcp_pcb, tcpRecvCb);
    tcp_err(tcp_pcb, tcpErrCb);

    // establishing connection
    err_t err = tcp_connect(tcp_pcb, &ecu_ip_addr, TCP_PORT_ECU, tcpConnectCb);
    if (err != ERR_OK)
    {
        LOG_DEBUG("Failed to connect to ECU: %d", err);
        tcpDisconnect();
        UNLOCK_TCPIP_CORE();
        return false;
    }
    UNLOCK_TCPIP_CORE();
    return true;
}


void ConnectionManager::tcpDisconnect()
{
    LOCK_TCPIP_CORE();
    if (tcp_pcb != nullptr)
    {
        // Unregister callbacks
        tcp_arg(tcp_pcb, nullptr);
        tcp_recv(tcp_pcb, nullptr);
        tcp_err(tcp_pcb, nullptr);
        // Close connection
        tcp_close(tcp_pcb);
        tcp_pcb = nullptr;
        LOG_DEBUG("TCP PCB closed and callbacks unregistered.");
    }
    else
    {
        LOG_DEBUG("tcp_pcb is already null, skipping disconnection.");
    }
    UNLOCK_TCPIP_CORE();
}

void ConnectionManager::HandleTcpRecvCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (!p || err != ERR_OK)
    {
        tcp_arg(tcp_pcb, nullptr);
        tcp_recv(tcp_pcb, nullptr);
        tcp_err(tcp_pcb, nullptr);
        // Close connection
        tcp_close(tcp_pcb);
        tcp_pcb = nullptr;
        LOG_DEBUG("Connection closed or error occurred.");
        EConnectionEvent connEvent = EVENT_TCP_CONNECTION_FAILED;
        if(xQueueSend(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(100000)) != pdPASS)
        {
            LOG_DEBUG("[FATAL ERROR] Could not send internal connection event: %d", connEvent);
        }
        return;
    }

    size_t payloadLen = p->len;
    uint8_t data[payloadLen];
    std::memcpy(data, p->payload, payloadLen);

    // mark data as received and free buffer to improve performance
    tcp_recved(tcp_pcb, p->len);
    pbuf_free(p);

    const uint32_t udsPayloadLen = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];

    // check if declared payload length is correct (4 bytes length, 2 bytes checksum(?))
    if (udsPayloadLen + 6 > payloadLen)
    {
        LOG_DEBUG("Data length mismatch: declared %u, received %u.", udsPayloadLen, payloadLen);
        return;
    }

    // we skip 2 bytes becuase it is a checksum (?) used in confirmation frame
    const EECUAddress sourceAddr = static_cast<EECUAddress>(data[6]);
    const EECUAddress targetAddr = static_cast<EECUAddress>(data[7]);

    if(static_cast<uint8_t>(targetAddr) != DIAGNOSTIC_TOOL_INTERNAL_ADDR)
    {
        // it means that it is echoed confirmation frame, discard.
        return;
    }

    EUDSResponseSID SID = static_cast<EUDSResponseSID>(data[8]);
    switch(SID)
    {
        case EUDSResponseSID::READ_DATA_BY_IDENTIFIER:
        {
            // the size refers to 2 bytes of source and target addr + 1 byte of SID + 2 bytes of DID, which gives 5 bytes that we are not interested in 
            handleReadDataByIdentifierCB(&data[11], udsPayloadLen - 5); 
            negative_response_count = 0;
            break;
        }
        case EUDSResponseSID::DYNAMICALLY_DEFINE_DATA_IDENTIFIER:
        {
            handleDynamicallyDefineDataIdentifierCB(&data[9], sourceAddr);
            negative_response_count = 0;
            break;
        }
        case EUDSResponseSID::NEGATIVE_RESPONSE:
        {
            // in case of negative response just move on, could be some temporary error.
            // only if negative response count reaches 10, then try to reset the whole connection
            negative_response_count++;
            if(negative_response_count >= 10)
            {
                EConnectionEvent connEvent = EVENT_TCP_CONNECTION_FAILED;
                xQueueSend(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(10000));
            }
        }
        default:
            // none other messages types are needed to be handled, so discard
            break;
    }
}

void ConnectionManager::HandleTcpConnectCb(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    // accoring to documentation this always call with err == ERR_OK, so this impl is in case of LWiP updates
    if (err != ERR_OK)
    {
        LOG_DEBUG("Connection failed with error: %d", err);
        EConnectionEvent connEvent = EVENT_TCP_CONNECTION_FAILED;
        if(xQueueSend(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(100000)) != pdPASS)
        {
            LOG_DEBUG("[FATAL ERROR] Could not send internal connection event: %d", connEvent);
        }
    }
    else
    {
        LOG_DEBUG("Connected to ECU.");
        EConnectionEvent connEvent = EVENT_TCP_CONNECTED;
        if(xQueueSend(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(100000)) != pdPASS)
        {
            LOG_DEBUG("[FATAL ERROR] Could not send internal connection event: %d", connEvent);
        }
    }
}

void ConnectionManager::HandleTcpErrCb(void *arg, err_t err)
{
    LOG_DEBUG("TCP connection error: %d", err);
    EConnectionEvent connEvent = EVENT_TCP_CONNECTION_FAILED;
    if(xQueueSend(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(100000)) != pdPASS)
    {
        LOG_DEBUG("[FATAL ERROR] Could not send internal connection event: %d", connEvent);
    }
}

bool ConnectionManager::tcpSend(uint8_t udsRequest[], size_t sizeOfUdsRequest, const bool tcpipThreadContext)
{
    if(!tcpipThreadContext)
    {
        LOCK_TCPIP_CORE();
    }
    err_t res = tcp_write(tcp_pcb, udsRequest, sizeOfUdsRequest, TCP_WRITE_FLAG_COPY);
    if (res != ERR_OK)
    {
        LOG_DEBUG("Failed to send UDS request: %d", res);
        if(!tcpipThreadContext)
        {
            UNLOCK_TCPIP_CORE();
        }
        return false;
    }

    tcp_output(tcp_pcb);
    if(!tcpipThreadContext)
    {
        UNLOCK_TCPIP_CORE();
    }
    return true;
}

void ConnectionManager::handleReadDataByIdentifierCB(uint8_t dataPayload[], uint32_t size)
{
    std::optional<SMessage> optMsg = extractDataFromResponse(dataPayload, size, currently_processed_request);
    if(optMsg.has_value())
    {
        SMessage msg = optMsg.value();
        xQueueSend(queueToEventManagerCM4, &(msg), static_cast<TickType_t>(10000));
        if(sending_data_by_uart)
        {
            float currentTick = getSystickInSeconds();
            float diff = currentTick - starting_timestamp_for_data_uart;

            const char* paramStr = EDoIPRequest_ToCStringMap.at(static_cast<EDoIPRequest>(msg.event_type));
            switch(msg.event_type)
            {
                case EVENT_DATA_UPDATE_DME_ENGINE_OIL_TEMPERATURE:
                [[fallthrough]];
                case EVENT_DATA_UPDATE_DME_AIR_MASS:
                [[fallthrough]];
                case EVENT_DATA_UPDATE_DME_RAIL_PRESSURE:
                [[fallthrough]];
                case EVENT_DATA_UPDATE_KOMBI_SPEED:
                [[fallthrough]];
                case EVENT_DATA_UPDATE_KOMBI_OUTSIDE_TEMP_SENSOR:
                [[fallthrough]];
                case EVENT_DATA_UPDATE_DME_COOLANT_TEMPERATURE:
                [[fallthrough]];
                case EVENT_DATA_UPDATE_DME_BATTERY_VOLTAGE:
                [[fallthrough]];
                case EVENT_DATA_UPDATE_DME_AMBIENT_TEMPERATURE:
                [[fallthrough]];
                case EVENT_DATA_UPDATE_IHKA_EVAPORATOR_TEMPERATURE_SENSOR:
                [[fallthrough]];
                case EVENT_DATA_UPDATE_DME_ACCELERATOR_PEDAL_POSITION:                
                {
                    // we can take any float type from the message
                    SEND_DATA_UART("%s,%.2f,%.2f", paramStr, diff, msg.message_data.dme_battery_voltage);
                    break;
                }
                case EVENT_DATA_UPDATE_DME_ENGINE_ROTATIONAL_SPEED:
                [[fallthrough]];
                case EVENT_DATA_UPDATE_KOMBI_ENGINE_SPEED_ON_DISP:
                {
                    SEND_DATA_UART("%s,%.2f,%u", paramStr, diff, msg.message_data.kombi_engine_speed_on_disp);
                    break;
                }
                case EVENT_DATA_UPDATE_KOMBI_TOTAL_DISTANCE:
                {
                    SEND_DATA_UART("%s,%.2f,%u", paramStr, diff, msg.message_data.kombi_total_distance);
                    break;
                }
                case EVENT_DATA_UPDATE_KOMBI_FUEL:
                {
                    SEND_DATA_UART("%s,%.2f,%u,%u,%u", paramStr, diff, msg.message_data.kombi_fuel_level[0], msg.message_data.kombi_fuel_level[1], msg.message_data.kombi_fuel_level[2]);
                    break;
                }
                case EVENT_DATA_UPDATE_IHKA_TEMPERATURE_SELECTOR:
                {
                    SEND_DATA_UART("%s,%.2f,%u,%u", paramStr, diff, msg.message_data.ihka_temperature_selector[0], msg.message_data.ihka_temperature_selector[1]);
                    break;
                }
                default:
                    break;
            }
        }
    }
    EConnectionEvent connEvent = EVENT_TCP_READ_DATA_BY_ID_RECEIVED;
    if(xQueueSend(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(100000)) != pdPASS)
    {
        LOG_DEBUG("[FATAL ERROR] Could not send internal connection event: %d", connEvent);
    }
}

void ConnectionManager::handleDynamicallyDefineDataIdentifierCB(uint8_t dataPayload[], EECUAddress sourceEcuAddr)
{
    const auto subSID = static_cast<DynamicallyAssignDataSubSID>(dataPayload[0]);
    switch(subSID)
    {
        case DynamicallyAssignDataSubSID::REQUEST_FIRST:
        {
            const size_t totalUDSRequestSize = secondReqDynDataSize + 6;
            uint8_t payloadToSend[totalUDSRequestSize] = {0}; // + 4 for data len and +2 for "checksum" 
            // response for the first dynamic assign sequence, so send next dynamic assign sequence packet

            auto identifierEntry = dataIdentifierMap.find(currently_processed_request);
            if(identifierEntry == dataIdentifierMap.end())
            {
                LOG_DEBUG("[FATAL ERROR] currently processed request invalid: %d", currently_processed_request);
                return;
            }
            auto idenidentifierAttributes = identifierEntry->second;
            if(!std::holds_alternative<EDynamicDataIndentifierRequestCode>(idenidentifierAttributes.first))
            {
                LOG_DEBUG("[FATAL ERROR] currently processed request has wrong DID assignment: %d", currently_processed_request);
                return;
            }
            prepareSecondRequestForDynamicData(payloadToSend, sourceEcuAddr, std::get<EDynamicDataIndentifierRequestCode>(idenidentifierAttributes.first)); //TODO BATTERY_VOLTAGE na sztywno
            tcpSend(payloadToSend, totalUDSRequestSize, true);
            break;
        }
        case DynamicallyAssignDataSubSID::REQUEST_ASSIGN:
        {
            const size_t totalUDSRequestSize = dataReqDataSize + 6;
            uint8_t payloadToSend[totalUDSRequestSize] = {0}; // + 4 for data len and +2 for "checksum" 
            // response for the second dynamic assign sequence, so send request for data
            prepareDataRequest(payloadToSend, sourceEcuAddr, EUDSDID::DYNAMICALLY_DEFINED_DATA_IDENTIFIER_0);
            tcpSend(payloadToSend, totalUDSRequestSize, true);
            break;
        }
        default:
            break;
    }
}

void ConnectionManager::sendRequestForData(EDoIPRequest request)
{
    currently_processed_request = request;
    auto identifierEntry = dataIdentifierMap.find(request);
    if(identifierEntry == dataIdentifierMap.end())
    {
        LOG_DEBUG("Requested data identifier invalid: %d", request);
        return;
    }

    auto idenidentifierAttributes = identifierEntry->second;
    if(std::holds_alternative<EUDSDID>(idenidentifierAttributes.first))
    {
        const size_t totalUDSRequestSize = dataReqDataSize + 6;
        uint8_t payloadToSend[totalUDSRequestSize] = {0}; // + 4 for data len and +2 for "checksum" 
        prepareDataRequest(payloadToSend, idenidentifierAttributes.second, std::get<EUDSDID>(idenidentifierAttributes.first));
        tcpSend(payloadToSend, totalUDSRequestSize, false);
    }
    else
    {
        const size_t totalUDSRequestSize = firstReqDynDataSize + 6;
        uint8_t payloadToSend[totalUDSRequestSize] = {0}; // + 4 for data len and +2 for "checksum" 
        prepareFirstRequestForDynamicData(payloadToSend, idenidentifierAttributes.second);
        tcpSend(payloadToSend, totalUDSRequestSize, false);
    }
}

void ConnectionManager::processUdpDicovery()
{
    sendSSDP();
    sendSLP();
    sendDoIPInit();
    osDelay(300);
}

void ConnectionManager::setSendingDataByUART(bool onoff)
{
    sending_data_by_uart = onoff;
    if(onoff)
    {
        starting_timestamp_for_data_uart = getSystickInSeconds();
    }
}
