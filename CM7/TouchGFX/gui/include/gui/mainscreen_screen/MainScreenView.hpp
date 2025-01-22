#ifndef MAINSCREENVIEW_HPP
#define MAINSCREENVIEW_HPP

#include <gui_generated/mainscreen_screen/MainScreenViewBase.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>

/**
 * @brief View for Main screen
 */
class MainScreenView : public MainScreenViewBase
{
public:

    /**
     * @brief Construct a new MainScreenView object
     */
    MainScreenView();

    /**
     * @brief Destroy the MainScreenView object
     */
    virtual ~MainScreenView() {}

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
protected:
};

#endif // MAINSCREENVIEW_HPP
