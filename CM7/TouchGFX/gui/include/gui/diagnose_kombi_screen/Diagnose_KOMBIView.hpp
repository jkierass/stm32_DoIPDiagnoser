#ifndef DIAGNOSE_KOMBIVIEW_HPP
#define DIAGNOSE_KOMBIVIEW_HPP

#include <gui_generated/diagnose_kombi_screen/Diagnose_KOMBIViewBase.hpp>
#include <gui/diagnose_kombi_screen/Diagnose_KOMBIPresenter.hpp>

/**
 * @brief View for Diagnose KOMBI screen
 */
class Diagnose_KOMBIView : public Diagnose_KOMBIViewBase
{
public:

    /**
     * @brief Construct a new Diagnose KOMBIView object
     */
    Diagnose_KOMBIView();

    /**
     * @brief Destroy the Diagnose KOMBIView object
     */
    virtual ~Diagnose_KOMBIView() {}

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
     * @brief Show popup message
     * 
     * @param message message to show
     * @param msgSize message size
     */
    void showPopup(uint8_t message[], size_t msgSize);

    /**
     * @brief Handle button UART clicked
     */
    void ButtonUartClicked() override;
protected:

    bool button_uart_click_state = false;
};

#endif // DIAGNOSE_KOMBIVIEW_HPP
