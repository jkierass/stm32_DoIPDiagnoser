#ifndef DIAGNOSE_IHKAPRESENTER_HPP
#define DIAGNOSE_IHKAPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Diagnose_IHKAView;


/**
 * @brief Presenter for Diagnose IHKA screen
 */
class Diagnose_IHKAPresenter : public touchgfx::Presenter, public ModelListener
{
public:

    /**
     * @brief Construct a new Diagnose_IHKAPresenter object
     * 
     * @param v view
     */
    Diagnose_IHKAPresenter(Diagnose_IHKAView& v);

    /**
     * @brief The activate function is called automatically when this screen is "switched in"
     *        (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * @brief The deactivate function is called automatically when this screen is "switched out"
     *        (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    /**
     * @brief Destroy the Diagnose_IHKAPresenter object
     */
    virtual ~Diagnose_IHKAPresenter() {}

    /**
     * @brief Handle the event
     * 
     * @param event event type
     * @param msg message data
     * @param eventSender event sender
     * @param eventReceiver event receiver
     */
    void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) override;

    /**
     * @brief Send event
     * 
     * @param event event type
     * @param message message data
     * @param eventReceiver event receiver
     * @return true if success
     */
    bool sendEvent(EEventType event, UMessageData message, EEventClient eventReceiver);

private:

    /**
     * @brief Construct a new Diagnose_IHKAPresenter object
     */
    Diagnose_IHKAPresenter();

    /**
     * @brief Load cache
     */
    void loadCache();

    Diagnose_IHKAView& view;
};

#endif // DIAGNOSE_IHKAPRESENTER_HPP
