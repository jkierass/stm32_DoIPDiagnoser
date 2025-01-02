#ifndef __CALCTASK_H__
#define __CALCTASK_H__

#include "lwip/tcp.h"
#include "lwip/udp.h"

#include "EventBus.h"
#include "UDSDataType.h"

class EthernetConnectionManagerTask final
{
private:

    static constexpr u16_t SLP_PORT = 427;
    static constexpr u16_t SSDP_PORT = 1900;
    static constexpr u16_t UDP_PORT = 6811;

    static constexpr u16_t TCP_PORT_ECU = 6801;

    static constexpr const char* BROADCAST_IP = "255.255.255.255";
    static constexpr const char* SSDP_MULTICAST_IP = "239.255.255.250";

    static constexpr uint8_t dataReqDataSize = 0x05;
    static constexpr uint8_t firstReqDynDataSize = 0x06;
    static constexpr uint8_t secondReqDynDataSize = 0x0a;

public:

	enum EConnectionState
	{
		STATE_ETHERNET_DISCONNECTED,
		STATE_SERVICE_DISCOVERY,
		STATE_CONNECTING_TCP,
		STATE_CONNECTED_ECU,  // is this not an ecu also (but some default one?)
        STATE_WAITING_FOR_DATA_FROM_ECU,  // is this not an ecu also (but some default one?)
	};

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

    void onEventDataSubscribe(const UMessageData& data);
    void onEventDataUnsubscribe(const UMessageData& data);

    EConnectionState onConnectionEvent(EConnectionEvent event);
    void processConnection();

    // TODO zmień onEvent na onEvent (nie wiadomo czy bedzie tranzycja czy nie)
    EConnectionState onEventEthDisconnected(EConnectionEvent event);
    EConnectionState onEventDiscovery(EConnectionEvent event);
    EConnectionState onEventConnectingTCP(EConnectionEvent event);
    EConnectionState onEventConnectedECUDefault(EConnectionEvent event);
    EConnectionState onEventWaitingForDataFromECU(EConnectionEvent event);

    void processStateDiscovery();
    void processStateConnectedECUDefault();
    void processStateWaitingForDataFromECU();

    bool sendSSDP();
    bool sendSLP();
    bool sendDoIPInit();

    void udpDisconnect();
    void udpConnect();

    bool tcpConnect();
    void tcpDisconnect();
    bool tcpSend(uint8_t udsRequest[], size_t sizeOfUdsRequest, const bool tcpipThreadContext);

    void handleReadDataByIdentifier(uint8_t dataPayload[], uint32_t size);
    void handleDynamicallyDefineDataIdentifier(uint8_t dataPayload[], EECUAddress sourceEcuAddr);

    void prepareDataRequest(uint8_t preparedPayload[], EECUAddress targetEcuAddr, EUDSDID did);

    void prepareFirstRequestForDynamicData(uint8_t preparedPayload[], EECUAddress targetEcuAddr);
    void prepareSecondRequestForDynamicData(uint8_t preparedPayload[], EECUAddress targetEcuAddr, EDynamicDataIndentifierRequestCode ddirCode);

    void sendNextRequest();
    void sendRequestForData(EDoIPRequest request);

	EventBus event_bus;
    QueueHandle_t connection_events_internal_queue;

	EConnectionState current_connection_state = STATE_ETHERNET_DISCONNECTED;

    ip_addr_t ecu_ip_addr;
    struct udp_pcb* udp_pcb = nullptr;
    struct tcp_pcb* tcp_pcb = nullptr;

    unsigned long long subscribed_data[2] = {0ULL};

    EDoIPRequest currentlyProcessedRequest = REQUEST_INVALID;

    uint8_t negativeResponseCount = 0;
};

#endif /*__CALCTASK_H__*/
