#ifndef MODEL_HPP
#define MODEL_HPP

#include "EventBus.h"

class ModelListener;

/**
 * @brief Model class
 */
class Model
{
public:

    /**
     * @brief struct for caching auxiliary data (date, time, room temperature)
     */
    struct SAuxDataCache
    {
        uint8_t day = 1;
        uint8_t month = 1;
        unsigned int year = 0;
        uint8_t hour = 1;
        uint8_t minute = 0;

        short int room_temperature = 23;
    } auxDataCache;

    /**
     * @brief Construct a new Model object
     */
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    /**
     * @brief Send forward the event
     * 
     * @param event event type
     * @param msg message data
     * @param eventSender event sender
     * @param eventReceiver event receiver
     */
    bool sendEvent(EEventType event, UMessageData message, EEventClient eventReceiver);


protected:
    ModelListener* modelListener;

    EventBus event_bus;
};

#endif // MODEL_HPP
