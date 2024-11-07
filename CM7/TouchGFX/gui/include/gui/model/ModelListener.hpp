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

    virtual void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender) {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
