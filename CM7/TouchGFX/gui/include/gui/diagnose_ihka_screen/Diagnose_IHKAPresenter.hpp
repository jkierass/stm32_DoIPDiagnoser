#ifndef DIAGNOSE_IHKAPRESENTER_HPP
#define DIAGNOSE_IHKAPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Diagnose_IHKAView;

class Diagnose_IHKAPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Diagnose_IHKAPresenter(Diagnose_IHKAView& v);

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

    virtual ~Diagnose_IHKAPresenter() {}

    void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) override;

private:
    Diagnose_IHKAPresenter();

    void loadCache();

    Diagnose_IHKAView& view;
};

#endif // DIAGNOSE_IHKAPRESENTER_HPP
