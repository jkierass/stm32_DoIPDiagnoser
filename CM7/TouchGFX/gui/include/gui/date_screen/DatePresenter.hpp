#ifndef DATEPRESENTER_HPP
#define DATEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DateView;

class DatePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DatePresenter(DateView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~DatePresenter() {}

    void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) override;
    bool sendEvent(EEventType event, UMessageData msg, EEventClient eventReceiver);

private:
    DatePresenter();

    void loadCache();

    DateView& view;
};

#endif // DATEPRESENTER_HPP
