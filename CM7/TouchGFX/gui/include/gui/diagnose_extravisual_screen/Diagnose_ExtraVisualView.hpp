#ifndef DIAGNOSE_EXTRAVISUALVIEW_HPP
#define DIAGNOSE_EXTRAVISUALVIEW_HPP

#include <gui_generated/diagnose_extravisual_screen/Diagnose_ExtraVisualViewBase.hpp>
#include <gui/diagnose_extravisual_screen/Diagnose_ExtraVisualPresenter.hpp>

/**
 * @brief View for Diagnose Extra Visual screen
 */
class Diagnose_ExtraVisualView : public Diagnose_ExtraVisualViewBase
{
public:
    /**
     * @brief Construct a new Diagnose Extra Visual View object
     */
    Diagnose_ExtraVisualView();

    /**
     * @brief Destroy the Diagnose Extra Visual View object
     */
    virtual ~Diagnose_ExtraVisualView() {}

    /**
     * @brief Initialize the screen
     */
    virtual void setupScreen();

    /**
     * @brief Tear down the screen
     */
    virtual void tearDownScreen();

    /**
     * @brief Update the diagnostic parameter value on screen
     * 
     * @param eventType event type
     * @param data message data
     */
    void UpdateParameterValue(EEventType eventType, const UMessageData& data);

    /**
     * @brief Set the temperature on screen
     * 
     * @param temperature temperature value
     */
    void setTemperature(short int temperature);

    /**
     * @brief Set the date on screen
     * 
     * @param day day
     * @param month month
     * @param year year
     */
    void setDate(uint8_t day, uint8_t month, unsigned int year);

    /**
     * @brief Set the time on screen
     * 
     * @param hour hour
     * @param minute minute
     */
    void setTime(uint8_t hour, uint8_t minute);

    /**
     * @brief Show popup with message
     * 
     * @param message message to show
     * @param msgSize message size
     */
    void showPopup(uint8_t message[], size_t msgSize);

    /**
     * @brief Handle button UART clicked
     */
    void ButtonUartClicked() override;

    /**
     * @brief Handle graph oil clicked
     * 
     * @param value graph click event
     */
    void graphOil_Clicked(AbstractDataGraph::GraphClickEvent value) override;

    /**
     * @brief Handle graph coolant clicked
     * 
     * @param value graph click event
     */
    void graphCoolant_Clicked(AbstractDataGraph::GraphClickEvent value) override;

    /**
     * @brief Handle graph oil dragged
     * 
     * @param value graph drag event
     */
    void graphOil_Dragged(AbstractDataGraph::GraphDragEvent value) override;

    /**
     * @brief Handle graph coolant dragged
     * 
     * @param value graph drag event
     */
    void graphCoolant_Dragged(AbstractDataGraph::GraphDragEvent value) override;

    /**
     * @brief Set request cycle time value on the screen
     * 
     * @param cycleMs cycle duration is ms
     */
    void setRequestCycleTime(uint16_t cycleMs);
protected:

    /**
     * @brief Update the position of value snippet on the oil graph
     */
    void updateInfoWidgetPosision_Oil();

    /**
     * @brief Update the position of value snippet on the coolant graph
     */
    void updateInfoWidgetPosision_Coolant();

    int lastIndexOil = -1;
    int lastIndexCoolant = -1;

    bool button_uart_click_state = false;
};

#endif // DIAGNOSE_EXTRAVISUALVIEW_HPP
