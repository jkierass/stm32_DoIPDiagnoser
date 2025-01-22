#ifndef DIAGNOSEVIEW_HPP
#define DIAGNOSEVIEW_HPP

#include <gui_generated/diagnose_screen/DiagnoseViewBase.hpp>
#include <gui/diagnose_screen/DiagnosePresenter.hpp>

/**
 * @brief View for Diagnose screen
 */
class DiagnoseView : public DiagnoseViewBase
{
public:

    /**
     * @brief Construct a new DiagnoseView object
     */
    DiagnoseView();

    /**
     * @brief Destroy the DiagnoseView object
     */
    virtual ~DiagnoseView() {}

    /**
     * @brief Initialize the screen
     */
    virtual void setupScreen();

    /**
     * @brief Tear down the screen
     */

    virtual void tearDownScreen();

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
protected:
};

#endif // DIAGNOSEVIEW_HPP
