#ifndef __CALCTASK_H__
#define __CALCTASK_H__

#include "lwip/tcp.h"
#include "lwip/udp.h"

#include "EventBus.h"

class EthernetConnectionManagerTask final
{
private:

    static constexpr u16_t SLP_PORT = 427;
    static constexpr u16_t SSDP_PORT = 1900;
    static constexpr u16_t UDP_PORT = 6811;

    static constexpr u16_t TCP_PORT_ECU = 6801;

    static constexpr const char* BROADCAST_IP = "255.255.255.255";
    static constexpr const char* SSDP_MULTICAST_IP = "239.255.255.250";

public:

	enum EConnectionState
	{
		STATE_ETHERNET_DISCONNECTED,
		STATE_SERVICE_DISCOVERY,
		STATE_CONNECTING_TCP,
		STATE_CONNECTED_ECU_DEFAULT,  // is this not an ecu also (but some default one?)
		STATE_CONNECTED_ECU_MOTOR // optional???? add other ecus
	};

	enum EConnectionEvent
	{
		EVENT_ETHERNET_CONNECTED,
		EVENT_ETHERNET_DISCONNECTED,
		EVENT_DISCOVERY_COMPLETED,
		EVENT_TCP_CONNECTED,
        EVENT_TCP_CONNECTION_FAILED,
		EVENT_REQUESTED_DEFAULT_ECU,
		EVENT_REQUESTED_MOTOR_ECU
	};

    static EthernetConnectionManagerTask& instance()
    {
        static EthernetConnectionManagerTask ecmt;
        return ecmt;
    }


	void process();
	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver);

    void HandleTcpRecvCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
    void HandleTcpConnectCb(void *arg, struct tcp_pcb *tpcb, err_t err);
    void HandleTcpErrCb(void *arg, err_t err);
    void HandleUdpRecvCb(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

private:

    EthernetConnectionManagerTask();
	EthernetConnectionManagerTask(EthernetConnectionManagerTask const&) = delete;
	EthernetConnectionManagerTask& operator=(EthernetConnectionManagerTask const&) = delete;
	EthernetConnectionManagerTask(EthernetConnectionManagerTask&&) = delete;
	EthernetConnectionManagerTask& operator=(EthernetConnectionManagerTask&&) = delete;

    EConnectionState onConnectionEvent(EConnectionEvent event);
    void processConnection();

    EConnectionState transitionEthDisconnected(EConnectionEvent event);
    EConnectionState transitionDiscovery(EConnectionEvent event);
    EConnectionState transitionConnectingTCP(EConnectionEvent event);
    EConnectionState transitionConnectedECUDefault(EConnectionEvent event);
    EConnectionState transitionConnectedECUMotor(EConnectionEvent event);

    void processStateDiscovery();
    void processStateConnectedECUDefault();
    void processStateConnectedECUMotor();

    bool sendSSDP();
    bool sendSLP();
    bool sendDoIPInit();

    void udpDisconnect();
    void udpConnect();

    bool tcpConnect();
    void tcpDisconnect();
    bool tcpSend(uint8_t udsRequest[], size_t sizeOfUdsRequest);

	EventBus event_bus;
    QueueHandle_t connection_events_internal_queue;

	EConnectionState current_connection_state;

    ip_addr_t ecu_ip_addr;
    struct udp_pcb* udp_pcb;
    struct tcp_pcb* tcp_pcb;
};

#endif /*__CALCTASK_H__*/
