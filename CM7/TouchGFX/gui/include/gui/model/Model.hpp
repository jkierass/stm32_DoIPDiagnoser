#ifndef MODEL_HPP
#define MODEL_HPP

#include "EventBus.h"

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

    EventBus event_bus;
};

#endif // MODEL_HPP
