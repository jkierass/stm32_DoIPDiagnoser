/**
  * @file           : DoIPDaemonTask.cpp
  * @brief          : Implementation of class for handling DoIP communication state machine.
  * @author         : Jakub Kierasinski
  */

#include "DoIPDaemonTask.h"
#include "projdefs.h"

#include "Logger.h"

#include <cstring>

using namespace APIDoIP;
using CM = ConnectionManager;

extern QueueHandle_t queueToDoIPDaemonTask;
QueueHandle_t connectionEventsQueue = xQueueCreate(16, sizeof(SMessage));

void StartTask_DoIPDaemonTask(void *argument)
{
    DoIPDaemonTask doipDaemonTask = DoIPDaemonTask();
    /* Infinite loop */
    for (;;) {
        doipDaemonTask.process();
        osDelay(1);
    }
}


DoIPDaemonTask::DoIPDaemonTask() : event_bus(EventBus(queueToDoIPDaemonTask, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER)), conn_mgr(CM::instance())
{
}

void DoIPDaemonTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
{
	switch(event)
	{
		case EVENT_ETHERNET_PLUGGED:
        {
            CM::EConnectionEvent connEvent = msg.ethernet_plugged ? CM::EConnectionEvent::EVENT_ETHERNET_CONNECTED : CM::EConnectionEvent::EVENT_ETHERNET_DISCONNECTED;
            xQueueSend(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(10000));
            break;
        }
        case EVENT_DATA_SUBSCRIBE:
        {
            onEventDataSubscribe(msg);
            break;
        }
	    case EVENT_DATA_UNSUBSCRIBE:
        {
            onEventDataUnsubscribe(msg);
            break;
        }
        case EVENT_REQUEST_CYCLE_SUSBCRIBE:
        {
            subscribed_for_cycle_time = true;
            last_start_cycle_timestamp = pdTICKS_TO_MS(xTaskGetTickCount());
            break;
        }
        case EVENT_REQUEST_CYCLE_UNSUSBCRIBE:
        {
            subscribed_for_cycle_time = false;
            break;
        }
        case EVENT_START_SENDING_DATA_UART:
        {
            conn_mgr.setSendingDataByUART(true);
            break;
        }
        case EVENT_STOP_SENDING_DATA_UART:
        {
            conn_mgr.setSendingDataByUART(false);
            break;
        }
		default:
			break;
	}
}

void DoIPDaemonTask::processConnection()
{
    switch(current_connection_state)
    {
        case STATE_SERVICE_DISCOVERY:
        {
            conn_mgr.processUdpDicovery();
            break;
        }
        default:
        {
            // do not do anything, wait for events
            break;
        }

    }
}

void DoIPDaemonTask::process()
{
	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver){this->OnEvent(event, msg, eventSender, eventReceiver);});
    processConnection();
    
    EConnectionState newState;
    CM::EConnectionEvent connEvent;
    BaseType_t received = pdTRUE;

	do
	{
		received = xQueueReceive(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(0));
		if(received == pdTRUE)
		{
			newState = onConnectionEvent(connEvent);
            if(newState != current_connection_state)
            {
                // transtition happened
                current_connection_state = newState;
                // Process again so that after every onEvent the processing state code is called at least once.
                // The chance of this happening is minimal, but not zero. 
                processConnection();
            }
		}
	} while(received == pdTRUE);
}

DoIPDaemonTask::EConnectionState DoIPDaemonTask::onConnectionEvent(CM::EConnectionEvent event)
{
    switch(current_connection_state)
    {
        case STATE_ETHERNET_DISCONNECTED:
        {
            return onEventEthDisconnected(event);
        }
        case STATE_SERVICE_DISCOVERY:
        {
            return onEventDiscovery(event);
        }
        case STATE_CONNECTING_TCP:
        {
            return onEventConnectingTCP(event);
        }
        case STATE_CONNECTED_ECU:
        {
            return onEventConnectedECUDefault(event);
        }
        case STATE_WAITING_FOR_DATA_FROM_ECU:
        {
            return onEventWaitingForDataFromECU(event);
        }
        default:
        {
            return current_connection_state;
        }
    }
}

DoIPDaemonTask::EConnectionState DoIPDaemonTask::onEventEthDisconnected(CM::EConnectionEvent event)
{
    switch(event)
    {
        case CM::EConnectionEvent::EVENT_ETHERNET_CONNECTED:
        {
            conn_mgr.udpConnect();
            return STATE_SERVICE_DISCOVERY;
        }
        default:
        {
            //no transition
            return current_connection_state;
        }
    }
}

DoIPDaemonTask::EConnectionState DoIPDaemonTask::onEventDiscovery(CM::EConnectionEvent event)
{
    switch(event)
    {
        case CM::EConnectionEvent::EVENT_ETHERNET_DISCONNECTED:
        {
            conn_mgr.udpDisconnect();
            return STATE_ETHERNET_DISCONNECTED;
        }
        case CM::EConnectionEvent::EVENT_DISCOVERY_COMPLETED:
        {
            conn_mgr.udpDisconnect();
            conn_mgr.tcpConnect();
            return STATE_CONNECTING_TCP;
        }
        default:
        {
            //no transition
            return current_connection_state;
        }
    }
}

DoIPDaemonTask::EConnectionState DoIPDaemonTask::onEventConnectingTCP(CM::EConnectionEvent event)
{
    switch(event)
    {
        case CM::EConnectionEvent::EVENT_ETHERNET_DISCONNECTED:
        {
            conn_mgr.tcpDisconnect();
            return STATE_ETHERNET_DISCONNECTED;
        }
        case CM::EConnectionEvent::EVENT_TCP_CONNECTION_FAILED:
        {
            conn_mgr.tcpDisconnect();
            conn_mgr.udpConnect();
            return STATE_SERVICE_DISCOVERY;
        }
        case CM::EConnectionEvent::EVENT_TCP_CONNECTED:
        {
        	if((subscribed_data[0] != 0ULL) || (subscribed_data[1] != 0ULL))
        	{
        		CM::EConnectionEvent connEvent = CM::EConnectionEvent::EVENT_DATA_REQUESTED;
        		xQueueSend(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(10000));
        	}
            return STATE_CONNECTED_ECU;
        }
        default:
        {
            //no transition
            return current_connection_state;
        }
    }
}

DoIPDaemonTask::EConnectionState DoIPDaemonTask::onEventConnectedECUDefault(CM::EConnectionEvent event)
{
    switch(event)
    {
		case CM::EConnectionEvent::EVENT_ETHERNET_DISCONNECTED:
		{
			conn_mgr.tcpDisconnect();
			return STATE_ETHERNET_DISCONNECTED;
		}
        case CM::EConnectionEvent::EVENT_TCP_CONNECTION_FAILED:
        {
            conn_mgr.tcpDisconnect();
            conn_mgr.udpConnect();
            return STATE_SERVICE_DISCOVERY;
        }
        case CM::EConnectionEvent::EVENT_DATA_REQUESTED:
        {
            if(subscribed_data[0] == 0ULL && subscribed_data[1] == 0ULL)
            {
                return STATE_CONNECTED_ECU;
            }
            else
            {
                sendNextRequest();
            }
            return STATE_WAITING_FOR_DATA_FROM_ECU;
        }
        default:
        {
            //no transition
            return current_connection_state;
        }
    }
}

DoIPDaemonTask::EConnectionState DoIPDaemonTask::onEventWaitingForDataFromECU(CM::EConnectionEvent event)
{
    switch(event)
    {
        case CM::EConnectionEvent::EVENT_ETHERNET_DISCONNECTED:
        {
            return STATE_ETHERNET_DISCONNECTED;
        }
        case CM::EConnectionEvent::EVENT_TCP_CONNECTION_FAILED:
        {
            conn_mgr.tcpDisconnect();
            conn_mgr.udpConnect();
            return STATE_SERVICE_DISCOVERY;
        }
        case CM::EConnectionEvent::EVENT_TCP_READ_DATA_BY_ID_RECEIVED:
        {
            if(subscribed_data[0] == 0ULL && subscribed_data[1] == 0ULL)
            {
                return STATE_CONNECTED_ECU;
            }
            else
            {
                sendNextRequest();
            }
            return STATE_WAITING_FOR_DATA_FROM_ECU;
        }
        case CM::EConnectionEvent::EVENT_NO_DATA_TO_REQUEST:
        {
            return STATE_CONNECTED_ECU;
        }
        default:
        {
            //no transition
            return current_connection_state;
        }
    }   
}

void DoIPDaemonTask::onEventDataSubscribe(const UMessageData& data)
{
    for(int i = 1; i <= data.event_subscriptions[0]; i++)
    {
        if(data.event_subscriptions[i] > 63)
        {
            subscribed_data[1] |= 1ULL << (data.event_subscriptions[i] - 64);
            if(currently_processed_request == REQUEST_INVALID)
            {
                currently_processed_request = static_cast<EDoIPRequest>(data.event_subscriptions[i]);
                first_in_cycle = currently_processed_request;
                last_start_cycle_timestamp = pdTICKS_TO_MS(xTaskGetTickCount());
            }
        }
        else
        {
            subscribed_data[0] |= 1ULL << data.event_subscriptions[i];
            if(currently_processed_request == REQUEST_INVALID)
            {
                currently_processed_request = static_cast<EDoIPRequest>(data.event_subscriptions[i]);
                first_in_cycle = currently_processed_request;
                last_start_cycle_timestamp = pdTICKS_TO_MS(xTaskGetTickCount());
            }
        }
    }

    CM::EConnectionEvent connEvent = CM::EConnectionEvent::EVENT_DATA_REQUESTED;
    xQueueSend(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(10000));
}

void DoIPDaemonTask::onEventDataUnsubscribe(const UMessageData& data)
{
    for (int i = 1; i <= data.event_subscriptions[0]; i++)
    {
        if (data.event_subscriptions[i] > 63)
        {
            subscribed_data[1] &= 0ULL << (data.event_subscriptions[i] - 64);
        }
        else
        {
            subscribed_data[0] &= 0ULL << data.event_subscriptions[i];
        }
    }

    if(subscribed_data[0] == 0ULL && subscribed_data[1] == 0ULL)
    {
        currently_processed_request = REQUEST_INVALID;
        first_in_cycle = REQUEST_INVALID;
        CM::EConnectionEvent connEvent = CM::EConnectionEvent::EVENT_NO_DATA_TO_REQUEST;
        xQueueSend(connectionEventsQueue, &(connEvent), static_cast<TickType_t>(10000));
    }
    else
    {
        for(int i = 1; i <= 128; i++)
        {
            if(i < 64)
            {
                if(subscribed_data[0] & (1ULL << i))
                {
                    first_in_cycle = static_cast<EDoIPRequest>(i);
                    break;
                }
            }
            else
            {
                if(subscribed_data[1] & (1ULL << (i - 64)))
                {
                    first_in_cycle = static_cast<EDoIPRequest>(i);
                    break;
                }
            }
        }
    }
}

void DoIPDaemonTask::sendNextRequest()
{
    // it is ugly, but it is fast, which is critical here
    for (int i = currently_processed_request + 1; i < 129; i++) //it is 129 on purpose, because after 127th param is processed we will reset index to 0
    {
        if(i > 127)
        {
            i = 0;
        }
        if(i < 64)
        {
            if (subscribed_data[0] & (1ULL << i))
            {
                auto dataType = static_cast<EDoIPRequest>(i);
                currently_processed_request = dataType;
                if(currently_processed_request == first_in_cycle && subscribed_for_cycle_time)
                {
                    auto current_timestamp = pdTICKS_TO_MS(xTaskGetTickCount());
                    auto ms_diff = current_timestamp - last_start_cycle_timestamp;
                    last_start_cycle_timestamp = current_timestamp;
                    UMessageData data;
                    data.last_request_cycle_ms = static_cast<uint16_t>(ms_diff);
                    event_bus.send(EVENT_LAST_REQUEST_CYCLE_TOOK_MS, data, EVENT_CLIENT_FRONTEND);
                }
                conn_mgr.sendRequestForData(dataType);
                break;
            }
        }
        else
        {
            if (subscribed_data[1] & (1ULL << (i - 64)))
            {
                auto dataType = static_cast<EDoIPRequest>(i - 64);
                currently_processed_request = dataType;
                conn_mgr.sendRequestForData(dataType);
                break;
            }
        }
    }
}

