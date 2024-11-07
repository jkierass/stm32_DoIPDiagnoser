#ifndef MODEL_HPP
#define MODEL_HPP

#include "EventBusManager.h"

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    bool sendEvent(EEventType event, UMessageData message, EEventClient eventReceiver);

protected:
    ModelListener* modelListener;

    EventBusManager event_bus;
};

#endif // MODEL_HPP
