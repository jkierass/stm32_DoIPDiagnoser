#ifndef MAINSCREENPRESENTER_HPP
#define MAINSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MainScreenView;

/**
 * @brief Presenter for Main screen
 */
class MainScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:

    /**
     * @brief Construct a new MainScreenPresenter object
     * 
     * @param v view
     */
    MainScreenPresenter(MainScreenView& v);

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
     * @brief Destroy the MainScreenPresenter object
     */
    virtual ~MainScreenPresenter() {}

    /**
     * @brief Handle the event
     * 
     * @param event event type
     * @param msg message data
     * @param eventSender event sender
     * @param eventReceiver event receiver
     */
    void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) override;

private:

    /**
     * @brief Construct a new MainScreenPresenter object
     */
    MainScreenPresenter();

    /**
     * @brief Load cache
     */
    void loadCache();

    MainScreenView& view;
};

#endif // MAINSCREENPRESENTER_HPP
