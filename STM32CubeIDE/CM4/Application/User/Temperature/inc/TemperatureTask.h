#ifndef __TEMPERATURETASKTASK_H__
#define __TEMPERATURETASKTASK_H__

#include <vector>

#include "EventBus.h"

class TemperatureTask
{
public:
	TemperatureTask();

	void process();

	TemperatureTask(TemperatureTask const&) = delete;
	TemperatureTask& operator=(TemperatureTask const&) = delete;
	TemperatureTask(TemperatureTask&&) = delete;
	TemperatureTask& operator=(TemperatureTask&&) = delete;

private:
	short int getMedian();
	uint32_t doMeasurement();

	std::vector<float> measurements;

	EventBus event_bus;
};

#endif /* __TEMPERATURETASKTASK_H__ */
