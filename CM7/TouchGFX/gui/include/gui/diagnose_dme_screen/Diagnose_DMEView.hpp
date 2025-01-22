#ifndef DIAGNOSE_DMEVIEW_HPP
#define DIAGNOSE_DMEVIEW_HPP

#include <gui_generated/diagnose_dme_screen/Diagnose_DMEViewBase.hpp>
#include <gui/diagnose_dme_screen/Diagnose_DMEPresenter.hpp>

/**
 * @brief View for Diagnose DME screen
 */
class Diagnose_DMEView : public Diagnose_DMEViewBase
{
public:

    /**
     * @brief Construct a new Diagnose DMEView object
     */
    Diagnose_DMEView();
    virtual ~Diagnose_DMEView() {}

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
     * @brief Set request cycle time value on the screen
     * 
     * @param cycleMs cycle duration is ms
     */
    void setRequestCycleTime(uint16_t cycleMs);

    /**
     * @brief Show the popup on screen
     * 
     * @param message message to show
     * @param msgSize size of the message
     */
    void showPopup(uint8_t message[], size_t msgSize);

    /**
     * @brief Handle clicking UART button
     */
    void ButtonUartClicked() override;
protected:

    bool button_uart_click_state = false;
};

#endif // DIAGNOSE_DMEVIEW_HPP
