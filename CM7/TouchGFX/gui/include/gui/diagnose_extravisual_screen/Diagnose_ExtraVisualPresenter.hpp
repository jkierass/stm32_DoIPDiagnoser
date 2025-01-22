#ifndef DIAGNOSE_EXTRAVISUALPRESENTER_HPP
#define DIAGNOSE_EXTRAVISUALPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Diagnose_ExtraVisualView;

/**
 * @brief Presenter for Diagnose Extra Visual screen
 */
class Diagnose_ExtraVisualPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    /**
     * @brief Construct a new Diagnose_ExtraVisualPresenter object
     * 
     * @param v view
     */
    Diagnose_ExtraVisualPresenter(Diagnose_ExtraVisualView& v);

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
     * @brief Destroy the Diagnose_ExtraVisualPresenter object
     */
    virtual ~Diagnose_ExtraVisualPresenter() {}

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
     * @brief Construct a new Diagnose_ExtraVisualPresenter object
     */
    Diagnose_ExtraVisualPresenter();

    /**
     * @brief Load cache
     */
    void loadCache();

    Diagnose_ExtraVisualView& view;
};

#endif // DIAGNOSE_EXTRAVISUALPRESENTER_HPP
