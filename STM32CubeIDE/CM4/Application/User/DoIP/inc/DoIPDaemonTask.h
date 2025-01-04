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

    unsigned long long subscribed_data[2] = {0ULL};

    APIDoIP::EDoIPRequest currently_processed_request = APIDoIP::REQUEST_INVALID;

    ConnectionManager& conn_mgr;
};

#endif /* __DOIPDAEMONTASK_H__ */