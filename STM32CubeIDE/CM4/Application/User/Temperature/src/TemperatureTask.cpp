#include "TemperatureTask.h"
#include "cm_ipc.h"

#include "stm32h7xx_hal.h"

#include <cmath>
#include <algorithm>
#include <limits>

extern I2C_HandleTypeDef hi2c4;

extern QueueHandle_t queueToTemperature;

constexpr size_t MEASUREMENTS_COUNT = 6;

constexpr uint16_t MCP9808_I2C_ADDRESS 	= 0x18 << 1;  // MCP9808 address (shift for HAL I2C)
constexpr uint8_t  MCP9808_TEMP_REG    	= 0x05;       // Temperature register


void StartTask_CTemp(void *argument)
{
	TemperatureTask TemperatureTaskInstance;

    for (;;)
    {
    	TemperatureTaskInstance.process();
    	osDelay(2000);
    }
}

TemperatureTask::TemperatureTask() : event_bus(EventBus(queueToTemperature, EVENT_CLIENT_THERMOMETER))
{
	measurements.reserve(MEASUREMENTS_COUNT);
}

void TemperatureTask::process()
{
	uint32_t err = doMeasurement();
	if( err != HAL_I2C_ERROR_NONE)
	{
		return;
	}
	short int measurement = getMedian();
	if(measurement == std::numeric_limits<short int>::min())
	{
		return;
	}
	EEventClient receiver = EVENT_CLIENT_FRONTEND;
	EEventType event = EVENT_UPDATE_ROOM_TEMPERATURE;
	UMessageData data;
	data.room_temperature = measurement;
	event_bus.send(event, data, receiver);
}

short int TemperatureTask::getMedian()
{
	short int median = std::numeric_limits<short>::min();
	size_t currMesSize = measurements.size();

	if(currMesSize > 0)
	{
		std::vector<float> sortedCopy = measurements;
		std::sort(sortedCopy.begin(), sortedCopy.end());

		if(currMesSize % 2 == 0)
		{
			median = static_cast<short int>(std::round((sortedCopy[currMesSize / 2 - 1] + sortedCopy[currMesSize / 2]) / 2));
		}
		else
		{
			median = static_cast<short int>(std::round(sortedCopy[currMesSize / 2]));
		}
	}
	return median;
}

uint32_t TemperatureTask::doMeasurement()
{
	uint8_t data[2];

    if (HAL_I2C_Mem_Read(&hi2c4, MCP9808_I2C_ADDRESS, MCP9808_TEMP_REG, I2C_MEMADD_SIZE_8BIT, data, 2, 1000u) != HAL_OK)
    {
    	uint32_t err = HAL_I2C_GetError(&hi2c4);
        // Error handling - restart I2C4 interface
    	HAL_I2C_DeInit(&hi2c4);
    	HAL_I2C_Init(&hi2c4);
    	//do not block other tasks, try again later.
    	return err;
    }

	uint16_t dataLong;
	float tmpVal = 0;
	float tmpValDec;

	dataLong = ((data[0] << 8u) | data[1]);
	/* Extract the integer part from the fixed point value */
	tmpVal = ((dataLong & 0x0FFF) >> 4);

	/* Extract decimal part */
	tmpValDec = 0.0625;
	for (int i=0; i < 4; i++)
	{
		tmpVal += ((dataLong >> i) & 0x0001) * tmpValDec;
		tmpValDec *= 2u;
	}

	short int measurement = static_cast<short int>(tmpVal);

	size_t currMesSize = measurements.size();

	if(currMesSize < MEASUREMENTS_COUNT)
	{
		measurements.push_back(measurement);
	}
	else
	{
		std::rotate(measurements.begin(), measurements.begin() + 1, measurements.end());
		measurements[currMesSize - 1] = measurement;
	}
	return HAL_I2C_ERROR_NONE;
}
