#ifndef __DOIPDAEMONTASK_H__
#define __DOIPDAEMONTASK_H__

#include "EventBus.h"
#include "APIDoIP.h"

#include "ConnectionManager.h"

class DoIPDaemonTask
{
public:

    DoIPDaemonTask();
	DoIPDaemonTask(DoIPDaemonTask const&) = delete;
	DoIPDaemonTask& operator=(DoIPDaemonTask const&) = delete;
	DoIPDaemonTask(DoIPDaemonTask&&) = delete;
	DoIPDaemonTask& operator=(DoIPDaemonTask&&) = delete;

	void process();
	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver);

private:

	enum EConnectionState
	{
		STATE_ETHERNET_DISCONNECTED,
		STATE_SERVICE_DISCOVERY,
		STATE_CONNECTING_TCP,
		STATE_CONNECTED_ECU,
        STATE_WAITING_FOR_DATA_FROM_ECU,
	};

    void onEventDataSubscribe(const UMessageData& data);
    void onEventDataUnsubscribe(const UMessageData& data);

    EConnectionState onConnectionEvent(ConnectionManager::EConnectionEvent event);
    void processConnection();

    EConnectionState onEventEthDisconnected(ConnectionManager::EConnectionEvent event);
    EConnectionState onEventDiscovery(ConnectionManager::EConnectionEvent event);
    EConnectionState onEventConnectingTCP(ConnectionManager::EConnectionEvent event);
    EConnectionState onEventConnectedECUDefault(ConnectionManager::EConnectionEvent event);
    EConnectionState onEventWaitingForDataFromECU(ConnectionManager::EConnectionEvent event);

    void sendNextRequest();

	EventBus event_bus;

	EConnectionState current_connection_state = STATE_ETHERNET_DISCONNECTED;

    uint64_t subscribed_data[2] = {0ULL};
    bool subscribed_for_cycle_time = false;

    APIDoIP::EDoIPRequest currently_processed_request = APIDoIP::REQUEST_INVALID;
    APIDoIP::EDoIPRequest first_in_cycle = APIDoIP::REQUEST_INVALID;
    TickType_t last_start_cycle_timestamp = 0;

    ConnectionManager& conn_mgr;
};

#endif /* __DOIPDAEMONTASK_H__ */