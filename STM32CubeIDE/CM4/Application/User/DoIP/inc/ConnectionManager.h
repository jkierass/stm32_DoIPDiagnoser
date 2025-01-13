#ifndef __CONNECTIONMANAGER_H__
#define __CONNECTIONMANAGER_H__

#include <lwip.h>
#include <lwip/tcp.h>
#include <lwip/udp.h>

#include "APIDoIP.h"

class ConnectionManager final
{
private:

    static constexpr u16_t SLP_PORT = 427;
    static constexpr u16_t SSDP_PORT = 1900;
    static constexpr u16_t UDP_PORT = 6811;

    static constexpr u16_t TCP_PORT_ECU = 6801;

    static constexpr const char* BROADCAST_IP = "255.255.255.255";
    static constexpr const char* SSDP_MULTICAST_IP = "239.255.255.250";

public:

	enum EConnectionEvent
	{
		EVENT_ETHERNET_CONNECTED,
		EVENT_ETHERNET_DISCONNECTED,
		EVENT_DISCOVERY_COMPLETED,
		EVENT_TCP_CONNECTED,
        EVENT_TCP_CONNECTION_FAILED,
		EVENT_REQUESTED_DEFAULT_ECU,

        EVENT_DATA_REQUESTED,
        EVENT_NO_DATA_TO_REQUEST,
        EVENT_TCP_READ_DATA_BY_ID_RECEIVED
	};

    static ConnectionManager& instance()
    {
        static ConnectionManager connMgrInstance;
        return connMgrInstance;
    }

    void HandleTcpRecvCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
    void HandleTcpConnectCb(void *arg, struct tcp_pcb *tpcb, err_t err);
    void HandleTcpErrCb(void *arg, err_t err);
    void HandleUdpRecvCb(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

    void sendRequestForData(APIDoIP::EDoIPRequest request);

    void udpDisconnect();
    void udpConnect();
    bool tcpConnect();
    void tcpDisconnect();

    void processUdpDicovery();

    void setSendingDataByUART(bool onoff);

private:

    ConnectionManager(){};
	ConnectionManager(ConnectionManager const&) = delete;
	ConnectionManager& operator=(ConnectionManager const&) = delete;
	ConnectionManager(ConnectionManager&&) = delete;
	ConnectionManager& operator=(ConnectionManager&&) = delete;

    bool sendSSDP();
    bool sendSLP();
    bool sendDoIPInit();

    bool tcpSend(uint8_t udsRequest[], size_t sizeOfUdsRequest, const bool tcpipThreadContext);

    void handleReadDataByIdentifier(uint8_t dataPayload[], uint32_t size);
    void handleDynamicallyDefineDataIdentifier(uint8_t dataPayload[], EECUAddress sourceEcuAddr);

    ip_addr_t ecu_ip_addr;
    struct udp_pcb* udp_pcb = nullptr;
    struct tcp_pcb* tcp_pcb = nullptr;

    APIDoIP::EDoIPRequest currently_processed_request = APIDoIP::REQUEST_INVALID;

    uint8_t negativeResponseCount = 0;

    bool sending_data_by_uart = false;
    TickType_t starting_timestamp_for_data_UART = 0;
};

#endif /*__CONNECTIONMANAGER_H__*/