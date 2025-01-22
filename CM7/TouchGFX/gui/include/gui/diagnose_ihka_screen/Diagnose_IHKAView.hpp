#ifndef DIAGNOSE_IHKAVIEW_HPP
#define DIAGNOSE_IHKAVIEW_HPP

#include <gui_generated/diagnose_ihka_screen/Diagnose_IHKAViewBase.hpp>
#include <gui/diagnose_ihka_screen/Diagnose_IHKAPresenter.hpp>

/**
 * @brief View for Diagnose IHKA screen
 */
class Diagnose_IHKAView : public Diagnose_IHKAViewBase
{
public:
    /**
     * @brief Construct a new Diagnose IHKAView object
     */
    Diagnose_IHKAView();

    /**
     * @brief Destroy the Diagnose IHKAView object
     */
    virtual ~Diagnose_IHKAView() {}

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
     * @brief Show popup message
     * 
     * @param message message to show
     * @param msgSize message size
     */
    void showPopup(uint8_t message[], size_t msgSize);

    /**
     * @brief Set request cycle time value on the screen
     * 
     * @param cycleMs cycle duration is ms
     */
    void setRequestCycleTime(uint16_t cycleMs);

    /**
     * @brief Handle button UART clicked
     */
    void ButtonUartClicked() override;

protected:

    bool button_uart_click_state = false;
};

#endif // DIAGNOSE_IHKAVIEW_HPP
