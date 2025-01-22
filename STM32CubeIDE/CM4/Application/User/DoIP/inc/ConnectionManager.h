#ifndef __CONNECTIONMANAGER_H__
#define __CONNECTIONMANAGER_H__

#include <lwip.h>
#include <lwip/tcp.h>
#include <lwip/udp.h>

#include "APIDoIP.h"

/**
 * @brief Singleton class for handling connection with ECU
 */
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

    /**
     * @brief Enum for connection events
     */
	enum EConnectionEvent
	{
		EVENT_ETHERNET_CONNECTED,
		EVENT_ETHERNET_DISCONNECTED,
		EVENT_DISCOVERY_COMPLETED,
		EVENT_TCP_CONNECTED,
        EVENT_TCP_CONNECTION_FAILED,

        EVENT_DATA_REQUESTED,
        EVENT_NO_DATA_TO_REQUEST,
        EVENT_TCP_READ_DATA_BY_ID_RECEIVED
	};

    /**
     * @brief Get the instance of the ConnectionManager
     * 
     * @return instance of the ConnectionManager
     */
    static ConnectionManager& instance()
    {
        static ConnectionManager connMgrInstance;
        return connMgrInstance;
    }

    /**
     * @brief Handle the callback from LwIP for receiving data over TCP
     * 
     * @param arg argument
     * @param tpcb pointer to the TCP connection control block
     * @param p pointer to the received data
     * @param err error
     */
    void HandleTcpRecvCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

    /**
     * @brief Handle the callback from LwIP for connecting TCP
     * 
     * @param arg argument
     * @param tpcb pointer to the TCP connection control block
     * @param err error
     */
    void HandleTcpConnectCb(void *arg, struct tcp_pcb *tpcb, err_t err);

    /**
     * @brief Handle the callback from LwIP for error in TCP
     * 
     * @param arg argument
     * @param err error
     */
    void HandleTcpErrCb(void *arg, err_t err);

    /**
     * @brief Handle the callback from LwIP for receiving data over UDP
     * 
     * @param arg argument
     * @param pcb pointer to the UDP connection control block
     * @param p pointer to the received data
     * @param addr pointer to the IP address
     * @param port port number
     */
    void HandleUdpRecvCb(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

    /**
     * @brief Send request for UDP data
     * 
     * @param request request type
     */
    void sendRequestForData(APIDoIP::EDoIPRequest request);

    /**
     * @brief Shutdown udp connection
     */
    void udpDisconnect();

    /**
     * @brief Setup udp connection
     */
    void udpConnect();

    /**
     * @brief Setup tcp connection
     */
    bool tcpConnect();

    /**
     * @brief Shutdown tcp connection
     */
    void tcpDisconnect();

    /**
     * @brief Process UDP discovery (DoIP)
     */
    void processUdpDicovery();

    /**
     * @brief Set flag for sending data by UART
     * 
     * @param onoff true to start, false to stop
     */
    void setSendingDataByUART(bool onoff);

private:

    /**
     * @brief Construct a new Connection Manager object and inits LwIP
     */
    ConnectionManager();
	ConnectionManager(ConnectionManager const&) = delete;
	ConnectionManager& operator=(ConnectionManager const&) = delete;
	ConnectionManager(ConnectionManager&&) = delete;
	ConnectionManager& operator=(ConnectionManager&&) = delete;

    /**
     * @brief Send SSDP request
     * @return true if success
     */
    bool sendSSDP();

    /**
     * @brief Send SLP request
     * @return true if success
     */
    bool sendSLP();

    /**
     * @brief Send DoIP init request
     * @return true if success
     */
    bool sendDoIPInit();

    /**
     * @brief Send data over TCP
     * 
     * @param udsRequest UDS request (payload)
     * @param sizeOfUdsRequest size of the UDS request (size of payload)
     * @param tcpipThreadContext true if called from tcpip thread
     * @return true if success
     */
    bool tcpSend(uint8_t udsRequest[], size_t sizeOfUdsRequest, const bool tcpipThreadContext);

    /**
     * @brief Handle read data by identifier callback
     * 
     * @param dataPayload payload of the response
     * @param size size of the payload
     */
    void handleReadDataByIdentifierCB(uint8_t dataPayload[], uint32_t size);

    /**
     * @brief Handle dynamically define data identifier callback
     * 
     * @param dataPayload payload of the response
     * @param sourceEcuAddr source ECU address
     */
    void handleDynamicallyDefineDataIdentifierCB(uint8_t dataPayload[], EECUAddress sourceEcuAddr);

    ip_addr_t ecu_ip_addr;
    struct udp_pcb* udp_pcb = nullptr;
    struct tcp_pcb* tcp_pcb = nullptr;

    APIDoIP::EDoIPRequest currently_processed_request = APIDoIP::REQUEST_INVALID;

    uint8_t negative_response_count = 0;

    bool sending_data_by_uart = false;
    float starting_timestamp_for_data_uart = 0;
};

#endif /*__CONNECTIONMANAGER_H__*/