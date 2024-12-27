#include <lwip.h>
#include "ethernetif.h"
#include "lwip/etharp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "EthernetConnMgrTask.h"
#include "Logger.h"

#include <cstring>

extern QueueHandle_t queueToEthernetConnMgr;

void StartTask_EthernetConnMgr(void *argument)
{
    EthernetConnectionManagerTask& connMgrInstance = EthernetConnectionManagerTask::instance();
    /* Infinite loop */
    for (;;) {
        connMgrInstance.process();
        osDelay(1);
    }
}

extern "C" err_t tcpRecvCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    EthernetConnectionManagerTask& connMgrInstance = EthernetConnectionManagerTask::instance();
    connMgrInstance.HandleTcpRecvCb(arg, tpcb, p, err);
    return ERR_OK;
}

extern "C" err_t tcpConnectCb(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    EthernetConnectionManagerTask& connMgrInstance = EthernetConnectionManagerTask::instance();
    connMgrInstance.HandleTcpConnectCb(arg, tpcb, err);
    return ERR_OK;
}

extern "C" void tcpErrCb(void *arg, err_t err)
{
    EthernetConnectionManagerTask& connMgrInstance = EthernetConnectionManagerTask::instance();
    connMgrInstance.HandleTcpErrCb(arg, err);
}

extern "C" void udpRecvCb(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    EthernetConnectionManagerTask& connMgrInstance = EthernetConnectionManagerTask::instance();
    connMgrInstance.HandleUdpRecvCb(arg, pcb, p, addr, port);
}


EthernetConnectionManagerTask::EthernetConnectionManagerTask() : event_bus(EventBus(queueToEthernetConnMgr, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER))
{
	MX_LWIP_Init();
	current_connection_state = STATE_ETHERNET_DISCONNECTED;
    connection_events_internal_queue = xQueueCreate(16, sizeof(EConnectionEvent));
}

void EthernetConnectionManagerTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
{
	switch(event)
	{
		case EVENT_ETHERNET_PLUGGED:
        {
            EConnectionEvent connEvent = msg.ethernet_plugged ? EVENT_ETHERNET_CONNECTED : EVENT_ETHERNET_DISCONNECTED;
            xQueueSend(connection_events_internal_queue, &(connEvent), static_cast<TickType_t>(10000));
            break;
        }
        case EVENT_DATA_SUBSCRIBE:
        {
            break;
        }
	    case EVENT_DATA_UNSUBSCRIBE:
        {
            break;
        }
		default:
			break;
	}
}

void EthernetConnectionManagerTask::processConnection()
{
    switch(current_connection_state)
    {
        case STATE_ETHERNET_DISCONNECTED:
        {
            //wait for connection event, do nothing until that happens
            break;
        }
        case STATE_SERVICE_DISCOVERY:
        {
            processStateDiscovery();
            break;
        }
        case STATE_CONNECTING_TCP:
        {
            // do not do anything, wait for events and info if connection is successfull (it is handled in transition code)
            break;
        }
        case STATE_CONNECTED_ECU_DEFAULT:
        {
            processStateConnectedECUDefault();
            break;
        }
        case STATE_CONNECTED_ECU_MOTOR:
        {
            processStateConnectedECUMotor();
            break;
        }
    }
}

void EthernetConnectionManagerTask::process()
{
	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver){this->OnEvent(event, msg, eventSender, eventReceiver);});
    processConnection();
    
    EConnectionState newState;
    EConnectionEvent connEvent;
    BaseType_t received = pdTRUE;

	do
	{
		received = xQueueReceive(connection_events_internal_queue, &(connEvent), static_cast<TickType_t>(0));
		if(received == pdTRUE)
		{
			newState = onConnectionEvent(connEvent);
            if(newState != current_connection_state)
            {
                // transtition happened
                current_connection_state = newState;
                // Process again so that after every transition the processing state code is called at least once.
                // The chance of this happening is minimal, but not zero. 
                processConnection();
            }
		}
	} while(received == pdTRUE);
}

void EthernetConnectionManagerTask::processStateDiscovery()
{
    bool ret = false;

    uint8_t tryCnt = 0;
    do
    {
        ret = sendSSDP();
        tryCnt++;
        osDelay(10);
    } while (ret == false && tryCnt < 4); // try 3 times

    tryCnt = 0;
    do
    {
        ret = sendSLP();
        tryCnt++;
        osDelay(10);
    } while (ret == false && tryCnt < 4); // try 3 times

    tryCnt = 0;
    do
    {
        ret = sendDoIPInit();
        tryCnt++;
        osDelay(10);
    } while (ret == false && tryCnt < 4); // try 3 times
    osDelay(100);

}

void EthernetConnectionManagerTask::processStateConnectedECUDefault()
{
    // TODO
}

void EthernetConnectionManagerTask::processStateConnectedECUMotor()
{
    // TODO (if needed?????)
}

EthernetConnectionManagerTask::EConnectionState EthernetConnectionManagerTask::onConnectionEvent(EConnectionEvent event)
{
    switch(current_connection_state)
    {
        case STATE_ETHERNET_DISCONNECTED:
        {
            return transitionEthDisconnected(event);
            break;
        }
        case STATE_SERVICE_DISCOVERY:
        {
            return transitionDiscovery(event);
            break;
        }
        case STATE_CONNECTING_TCP:
        {
            return transitionConnectingTCP(event);
            break;
        }
        case STATE_CONNECTED_ECU_DEFAULT:
        {
            return transitionConnectedECUDefault(event);
            break;
        }
        case STATE_CONNECTED_ECU_MOTOR:
        {
            return transitionConnectedECUMotor(event);
            break;
        }
        default:
            return STATE_ETHERNET_DISCONNECTED;
    }
}

EthernetConnectionManagerTask::EConnectionState EthernetConnectionManagerTask::transitionEthDisconnected(EConnectionEvent event)
{
    switch(event)
    {
        case EVENT_ETHERNET_CONNECTED:
        {
            udpConnect();
            return STATE_SERVICE_DISCOVERY;
        }
        default:
        {
            //no transition
            return current_connection_state;
            break;
        }
    }
}

EthernetConnectionManagerTask::EConnectionState EthernetConnectionManagerTask::transitionDiscovery(EConnectionEvent event)
{
    switch(event)
    {
        case EVENT_ETHERNET_DISCONNECTED:
        {
            udpDisconnect();
            return STATE_ETHERNET_DISCONNECTED;
        }
        case EVENT_DISCOVERY_COMPLETED:
        {
            udpDisconnect();

            if(!tcpConnect())
            {
                // if connection not successfull, go back to discovery
                return STATE_SERVICE_DISCOVERY;
            }

            return STATE_CONNECTING_TCP;
        }
        default:
        {
            //no transition
            return current_connection_state;
            break;
        }
    }
}

EthernetConnectionManagerTask::EConnectionState EthernetConnectionManagerTask::transitionConnectingTCP(EConnectionEvent event)
{
    switch(event)
    {
        case EVENT_ETHERNET_DISCONNECTED:
        {
            tcpDisconnect();
            return STATE_ETHERNET_DISCONNECTED;
        }
        case EVENT_TCP_CONNECTION_FAILED:
        {
            if(!tcpConnect())
            {
                // if connection not successfull, go back to discovery
                return STATE_SERVICE_DISCOVERY;
            }
            return STATE_CONNECTING_TCP;
        }
        case EVENT_TCP_CONNECTED:
        {
            return STATE_CONNECTED_ECU_DEFAULT;
        }
        default:
        {
            //no transition
            return current_connection_state;
            break;
        }
    }
}

EthernetConnectionManagerTask::EConnectionState EthernetConnectionManagerTask::transitionConnectedECUDefault(EConnectionEvent event)
{
    switch(event)
    {
        case EVENT_ETHERNET_DISCONNECTED:
        {
            return STATE_ETHERNET_DISCONNECTED;
        }
        default:
        {
            //no transition
            return current_connection_state;
            break;
        }
    }
}

EthernetConnectionManagerTask::EConnectionState EthernetConnectionManagerTask::transitionConnectedECUMotor(EConnectionEvent event)
{
    switch(event)
    {
        case EVENT_ETHERNET_DISCONNECTED:
        {
            return STATE_ETHERNET_DISCONNECTED;
        }
        default:
        {
            //no transition
            return current_connection_state;
            break;
        }
    }
    
}

bool EthernetConnectionManagerTask::sendSSDP()
{
    constexpr const char *ssdp_request =
        "M-SEARCH * HTTP/1.1\r\n"
        "HOST: 239.255.255.250:1900\r\n"
        "MAN: \"ssdp:discover\"\r\n"
        "ST: ssdp:all\r\n"
        "MX: 3\r\n"
        "\r\n";

    ip_addr_t multicast_ip;
    struct pbuf *p;
    
    // Set the multicast address for SSDP
    ipaddr_aton(SSDP_MULTICAST_IP, &multicast_ip);

    // Allocate buffer for SSDP data
    p = pbuf_alloc(PBUF_TRANSPORT, strlen(ssdp_request), PBUF_RAM);
    if (!p)
    {
        LOG_DEBUG("Failed to allocate pbuf for SSDP.");
        return false;
    }

    // Copy data to the buffer
    memcpy(p->payload, ssdp_request, strlen(ssdp_request));

    // Send SSDP
    if (udp_sendto(udp_pcb, p, &multicast_ip, 1900) != ERR_OK)
    {
        LOG_DEBUG("Failed to send SSDP request.");
        pbuf_free(p);
        return false;
    }

    // Release the buffer
    pbuf_free(p);
    return true;
}

bool EthernetConnectionManagerTask::sendSLP()
{
    uint8_t slp_request[36] =
    {
        0x02, 0x06, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00,  // Header
        0x00, 0x00, 0x00, 0x01,                          // XID set to 1 (big-endian)
        0x00, 0x02,                                      // Service Type (srvtype)
        0x65, 0x6e, 0x00, 0x00, 0x00, 0x00,              // Lang Tag (en)
        0x00, 0x07, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c,  // Service (default)
        0x74, 0x00, 0x00, 0x00, 0x00                     // Padding
    };
    ip_addr_t broadcast_ip;
    struct pbuf *p;

    // Set the broadcast address
    ipaddr_aton(BROADCAST_IP, &broadcast_ip);

    // Allocate buffer for SLP data
    p = pbuf_alloc(PBUF_TRANSPORT, sizeof(slp_request), PBUF_RAM);
    if (!p)
    {
        LOG_DEBUG("Failed to allocate pbuf for SLP.");
        return false;
    }

    // Copy data to the buffer
    memcpy(p->payload, slp_request, sizeof(slp_request));

    // Send SLP
    if (udp_sendto(udp_pcb, p, &broadcast_ip, SLP_PORT) != ERR_OK)
    {
        LOG_DEBUG("Failed to send SLP request.");
        pbuf_free(p);
        return false;
    }

    // Release the buffer
    pbuf_free(p);
    return true;
}


bool EthernetConnectionManagerTask::sendDoIPInit()
{
const char doip_request[] = "\x00\x00\x00\x00\x00\x11";
    ip_addr_t broadcast_ip;
    struct pbuf *p;

    // Set the broadcast address for DoIP
    ipaddr_aton(BROADCAST_IP, &broadcast_ip);

    // Allocate buffer for DoIP data
    p = pbuf_alloc(PBUF_TRANSPORT, sizeof(doip_request) - 1, PBUF_RAM);
    if (!p)
    {
        LOG_DEBUG("Failed to allocate pbuf for DoIP init.");
        return false;
    }

    // Copy data to the buffer
    memcpy(p->payload, doip_request, sizeof(doip_request) - 1);

    // Send DoIP
    if (udp_sendto(udp_pcb, p, &broadcast_ip, UDP_PORT) != ERR_OK)
    {
        LOG_DEBUG("Failed to send DoIP init request.");
        pbuf_free(p);
        return false;
    }

    // Release the buffer
    pbuf_free(p);
    return true;
}

void EthernetConnectionManagerTask::HandleUdpRecvCb(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
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
    uint32_t data_length = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    // 6 bytes for header (length + type)
    if (data_length + 6 > p->len)
    {
        LOG_DEBUG("Data length mismatch: declared %u, received %u.", data_length, p->len);
        pbuf_free(p);
        return;
    }
    LOG_DEBUG("Data Length: %u", data_length);

    // Extract message type (next 2 bytes)
    uint16_t message_type = (payload[4] << 8) | payload[5];
    LOG_DEBUG("Message Type: 0x%04X", message_type);

    // Process diagnostic data if available
    if (p->len > 14)
    {
        uint8_t *diagnostic_data = &payload[14];
        size_t diagnostic_data_len = p->len - 14;

        // Search for MAC and VIN keywords
        char *mac_start = (char *)strstr((char *)diagnostic_data, "MAC");
        char *vin_start = (char *)strstr((char *)diagnostic_data, "VIN");

        if (mac_start && vin_start)
        {
            mac_start += strlen("MAC");
            vin_start += strlen("VIN");

            // Safely extract MAC Address (12 characters)
            if (mac_start + 12 <= (char *)diagnostic_data + diagnostic_data_len)
            {
                char mac_address_str[13] = {0};
                strncpy(mac_address_str, mac_start, 12);

                //convert to format accepted by lwip arp
                uint8_t ecu_mac[6] = {0};
                for (int i = 0; i < 6; i++)
                {
                    if (sscanf(&mac_address_str[i * 2], "%2hhx", &ecu_mac[i]) != 1)
                    {
                        LOG_DEBUG("Failed to parse MAC address.");
                        pbuf_free(p);
                        return;
                    }
                }
                LOG_DEBUG("Extracted MAC Address: %s", mac_address_str);

                // Add a static ARP entry for ECU
                struct eth_addr eth_mac;
                memcpy(eth_mac.addr, ecu_mac, 6);
                if (etharp_add_static_entry(addr, &eth_mac) == ERR_OK)
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
            if (vin_start + 17 <= (char *)diagnostic_data + diagnostic_data_len)
            {
                char vin[18] = {0};
                strncpy(vin, vin_start, 17);
                LOG_DEBUG("Extracted VIN: %s", vin);

                EConnectionEvent connEvent = EVENT_DISCOVERY_COMPLETED;
                if(xQueueSend(connection_events_internal_queue, &(connEvent), static_cast<TickType_t>(10000)) != pdPASS)
                {
                    //In this case do not notify app and just wait for another message from ecu to try on the next call
                    LOG_DEBUG("[FATAL ERROR] Could not send internal connection event: %d", connEvent);
                    return;
                }
                
                UMessageData msgData;
                strncpy(msgData.ecu_connected_vin, vin_start, 17);
                event_bus.send(EVENT_ECU_CONNECTION_INITIALISED, msgData, EVENT_CLIENT_FRONTEND);
                
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

void EthernetConnectionManagerTask::udpConnect()
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


void EthernetConnectionManagerTask::udpDisconnect()
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

bool EthernetConnectionManagerTask::tcpConnect()
{
	tcp_pcb = tcp_new();
    LOCK_TCPIP_CORE();
    if (!tcp_pcb)
    {
        LOG_DEBUG("Failed to create TCP PCB.");
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
    else
    {
        LOG_DEBUG("Connecting to ECU.");
    }
    UNLOCK_TCPIP_CORE();
    return true;
}


void EthernetConnectionManagerTask::tcpDisconnect()
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

void EthernetConnectionManagerTask::HandleTcpRecvCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (!p || err != ERR_OK)
    {
        tcp_close(tpcb);
        LOG_DEBUG("Connection closed or error occurred.");
        return;
    }

    // TODO process data
    LOG_DEBUG("Received %d bytes of data:", p->len);
    uint8_t *data = (uint8_t *)p->payload;
    for (int i = 0; i < p->len; i++)
    {
        LOG_DEBUG("%02X ", data[i]);
    }

    pbuf_free(p);
}

void EthernetConnectionManagerTask::HandleTcpConnectCb(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    // accoring to documentation this always call with err == ERR_OK, so this impl is in case of LWiP updates
    if (err != ERR_OK)
    {
        LOG_DEBUG("Connection failed with error: %d", err);
        EConnectionEvent connEvent = EVENT_TCP_CONNECTION_FAILED;
        if(xQueueSend(connection_events_internal_queue, &(connEvent), static_cast<TickType_t>(100000)) != pdPASS)
        {
            LOG_DEBUG("[FATAL ERROR] Could not send internal connection event: %d", connEvent);
            configASSERT(0);
        }
    }
    else
    {
        LOG_DEBUG("Connected to ECU.");
        EConnectionEvent connEvent = EVENT_TCP_CONNECTED;
        if(xQueueSend(connection_events_internal_queue, &(connEvent), static_cast<TickType_t>(100000)) != pdPASS)
        {
            LOG_DEBUG("[FATAL ERROR] Could not send internal connection event: %d", connEvent);
            configASSERT(0);
        }
    }
}

void EthernetConnectionManagerTask::HandleTcpErrCb(void *arg, err_t err)
{
    LOG_DEBUG("TCP connection error: %d", err);
}

bool EthernetConnectionManagerTask::tcpSend(uint8_t udsRequest[], size_t sizeOfUdsRequest)
{
    LOCK_TCPIP_CORE();
    err_t res = tcp_write(tcp_pcb, udsRequest, sizeOfUdsRequest, TCP_WRITE_FLAG_COPY);
    if (res != ERR_OK)
    {
        LOG_DEBUG("Failed to send UDS request: %d", res);
        UNLOCK_TCPIP_CORE();
        return false;
    }

    tcp_output(tcp_pcb);
    UNLOCK_TCPIP_CORE();
    return true;
}
