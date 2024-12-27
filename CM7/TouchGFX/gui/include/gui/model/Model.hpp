#ifndef MODEL_HPP
#define MODEL_HPP

#include "EventBus.h"

class ModelListener;

class Model
{
public:

    struct SAuxDataCache
    {
        uint8_t day = 1;
        uint8_t month = 1;
        unsigned int year = 0;
        uint8_t hour = 1;
        uint8_t minute = 0;

        short int room_temperature = 23;
    } auxDataCache;

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
