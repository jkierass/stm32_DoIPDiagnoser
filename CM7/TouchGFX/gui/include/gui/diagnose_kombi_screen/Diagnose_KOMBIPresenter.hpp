#ifndef DIAGNOSE_KOMBIPRESENTER_HPP
#define DIAGNOSE_KOMBIPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Diagnose_KOMBIView;

class Diagnose_KOMBIPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Diagnose_KOMBIPresenter(Diagnose_KOMBIView& v);

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

    virtual ~Diagnose_KOMBIPresenter() {}

    void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) override;

private:
    Diagnose_KOMBIPresenter();

    void loadCache();

    Diagnose_KOMBIView& view;
};

#endif // DIAGNOSE_KOMBIPRESENTER_HPP
