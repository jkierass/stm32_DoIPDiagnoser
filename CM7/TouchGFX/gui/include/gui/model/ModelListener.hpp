#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>
#include "MessageDataTypes.h"

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    /**
     * @brief Handle the event
     * 
     * @param event event type
     * @param msg message data
     * @param eventSender event sender
     * @param eventReceiver event receiver
     */
    virtual void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
