#ifndef DIAGNOSE_DMEPRESENTER_HPP
#define DIAGNOSE_DMEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Diagnose_DMEView;

class Diagnose_DMEPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Diagnose_DMEPresenter(Diagnose_DMEView& v);

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

    virtual ~Diagnose_DMEPresenter() {}

    void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) override;

private:
    Diagnose_DMEPresenter();

    void loadCache();

    Diagnose_DMEView& view;
};

#endif // DIAGNOSE_DMEPRESENTER_HPP
