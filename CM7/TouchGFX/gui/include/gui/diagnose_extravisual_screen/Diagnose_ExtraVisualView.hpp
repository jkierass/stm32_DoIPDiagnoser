#ifndef DIAGNOSE_EXTRAVISUALVIEW_HPP
#define DIAGNOSE_EXTRAVISUALVIEW_HPP

#include <gui_generated/diagnose_extravisual_screen/Diagnose_ExtraVisualViewBase.hpp>
#include <gui/diagnose_extravisual_screen/Diagnose_ExtraVisualPresenter.hpp>

class Diagnose_ExtraVisualView : public Diagnose_ExtraVisualViewBase
{
public:
    Diagnose_ExtraVisualView();
    virtual ~Diagnose_ExtraVisualView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void UpdateParameterValue(EEventType eventType, const UMessageData& data);

    void setTemperature(short int temperature);
    void setDate(uint8_t day, uint8_t month, unsigned int year);
    void setTime(uint8_t hour, uint8_t minute);

    void showPopup(uint8_t message[], size_t msgSize);

    void ButtonUartClicked() override;
    void graphOil_Clicked(AbstractDataGraph::GraphClickEvent value) override;
    void graphCoolant_Clicked(AbstractDataGraph::GraphClickEvent value) override;
    void graphOil_Dragged(AbstractDataGraph::GraphDragEvent value) override;
    void graphCoolant_Dragged(AbstractDataGraph::GraphDragEvent value) override;

protected:
    void updateInfoWidgetPosision_Oil();
    void updateInfoWidgetPosision_Coolant();

    int lastIndexOil = -1;
    int lastIndexCoolant = -1;
};

#endif // DIAGNOSE_EXTRAVISUALVIEW_HPP
