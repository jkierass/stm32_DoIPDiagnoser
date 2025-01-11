#ifndef DIAGNOSE_EXTRAVISUALPRESENTER_HPP
#define DIAGNOSE_EXTRAVISUALPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Diagnose_ExtraVisualView;

class Diagnose_ExtraVisualPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Diagnose_ExtraVisualPresenter(Diagnose_ExtraVisualView& v);

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

    virtual ~Diagnose_ExtraVisualPresenter() {}

     void OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver) override;

private:
    Diagnose_ExtraVisualPresenter();

    void loadCache();

    Diagnose_ExtraVisualView& view;
};

#endif // DIAGNOSE_EXTRAVISUALPRESENTER_HPP
