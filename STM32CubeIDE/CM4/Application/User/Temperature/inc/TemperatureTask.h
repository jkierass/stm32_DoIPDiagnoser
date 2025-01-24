/**
  * @file           : TemperatureTask.h
  * @brief          : Header for handling temperature measurement task.
  * @author         : Jakub Kierasinski
  */

#ifndef __TEMPERATURETASKTASK_H__
#define __TEMPERATURETASKTASK_H__

#include <vector>

#include "EventBus.h"


/**
 * @brief Class for handling temperature measurement task
 */
class TemperatureTask
{
public:

    /**
     * @brief Construct a new TemperatureTask object
     */
	TemperatureTask();

    /**
     * @brief Process the temperature task
     */
	void process();

	TemperatureTask(TemperatureTask const&) = delete;
	TemperatureTask& operator=(TemperatureTask const&) = delete;
	TemperatureTask(TemperatureTask&&) = delete;
	TemperatureTask& operator=(TemperatureTask&&) = delete;

private:

    /**
     * @brief Get the median value from the measurements
     * 
     * @return short int median value
     */
    short int getMedian();

    /**
     * @brief Do the temperature measurement
     * 
     * @return uint32_t measurement value
     */
    uint32_t doMeasurement();

	std::vector<float> measurements;

	EventBus event_bus;
};

#endif /* __TEMPERATURETASKTASK_H__ */
