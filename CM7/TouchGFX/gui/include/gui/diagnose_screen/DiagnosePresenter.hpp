#ifndef DIAGNOSEPRESENTER_HPP
#define DIAGNOSEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DiagnoseView;

/**
 * @brief Presenter for Diagnose screen
 */
class DiagnosePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    /**
     * @brief Construct a new DiagnosePresenter object
     * 
     * @param v view
     */
    DiagnosePresenter(DiagnoseView& v);

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
     * @brief Destroy the DiagnosePresenter object
     */
    virtual ~DiagnosePresenter() {}

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
     * @brief Construct a new DiagnosePresenter object
     */
    DiagnosePresenter();

    /**
     * @brief Load cache
     */
    void loadCache();

    DiagnoseView& view;
};

#endif // DIAGNOSEPRESENTER_HPP
