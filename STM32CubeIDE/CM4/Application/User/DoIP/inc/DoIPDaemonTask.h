/**
  * @file           : DoIPDaemonTask.h
  * @brief          : Header for DoIPDaemonTask class used for handling DoIP communication state machine.
  * @author         : Jakub Kierasinski
  */


#ifndef __DOIPDAEMONTASK_H__
#define __DOIPDAEMONTASK_H__

#include "EventBus.h"
#include "APIDoIP.h"

#include "ConnectionManager.h"


/**
 * @brief Class for handling DoIP communication state machine
 */
class DoIPDaemonTask
{
public:

    /**
     * @brief Construct a new DoIPDaemonTask object
     */
    DoIPDaemonTask();
	DoIPDaemonTask(DoIPDaemonTask const&) = delete;
	DoIPDaemonTask& operator=(DoIPDaemonTask const&) = delete;
	DoIPDaemonTask(DoIPDaemonTask&&) = delete;
	DoIPDaemonTask& operator=(DoIPDaemonTask&&) = delete;

    /**
     * @brief Process the DoIP task
     */
	void process();

    /**
     * @brief Handle the event
     * 
     * @param event event type
     * @param msg message data
     * @param eventSender event sender
     * @param eventReceiver event receiver
     */
	void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver);

private:

    /**
     * @brief Enum for connection states
     */
	enum EConnectionState
	{
		STATE_ETHERNET_DISCONNECTED,
		STATE_SERVICE_DISCOVERY,
		STATE_CONNECTING_TCP,
		STATE_CONNECTED_ECU,
        STATE_WAITING_FOR_DATA_FROM_ECU,
	};

    /**
     * @brief Handle the event for data subscription
     * 
     * @param data message data
     */
    void onEventDataSubscribe(const UMessageData& data);

    /**
     * @brief Handle the event for data unsubscription
     * 
     * @param data message data
     */
    void onEventDataUnsubscribe(const UMessageData& data);

    /**
     * @brief Handle the connection event
     * 
     * @param event event type
     */
    EConnectionState onConnectionEvent(ConnectionManager::EConnectionEvent event);

    /**
     * @brief Process the connection
     */
    void processConnection();

    /**
     * @brief Handle the event by Ethernet disconnected state
     * 
     * @param event event type
     */
    EConnectionState onEventEthDisconnected(ConnectionManager::EConnectionEvent event);

    /**
     * @brief Handle the event by service discovery state
     * 
     * @param event event type
     */
    EConnectionState onEventDiscovery(ConnectionManager::EConnectionEvent event);

    /**
     * @brief Handle the event by connecting TCP state
     * 
     * @param event event type
     */
    EConnectionState onEventConnectingTCP(ConnectionManager::EConnectionEvent event);

    /**
     * @brief Handle the event by connected ECU state
     * 
     * @param event event type
     */
    EConnectionState onEventConnectedECUDefault(ConnectionManager::EConnectionEvent event);

    /**
     * @brief Handle the event by waiting for data from ECU state
     * 
     * @param event event type
     */
    EConnectionState onEventWaitingForDataFromECU(ConnectionManager::EConnectionEvent event);

    /**
     * @brief Send next request that the user has subscribed for
     */
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