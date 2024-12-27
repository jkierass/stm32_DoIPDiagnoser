#include <gui/diagnose_motor1_screen/Diagnose_Motor1View.hpp>

Diagnose_Motor1View::Diagnose_Motor1View()
{
    // insert handled params
    currentParamData.emplace(EVENT_DATA_UPDATE_BATTERY_VOLTAGE, UMessageData());
    currentParamData.emplace(EVENT_DATA_UPDATE_MOTOR_TEMPERATURE, UMessageData());
    currentParamData.emplace(EVENT_DATA_UPDATE_AIR_MASS, UMessageData());
    currentParamData.emplace(EVENT_DATA_UPDATE_INTAKE_AIR_TEMPERATURE, UMessageData());
    currentParamData.emplace(EVENT_DATA_UPDATE_BOOST_PRESSURE_SET, UMessageData());
    currentParamData.emplace(EVENT_DATA_UPDATE_BOOST_PRESSURE_ACT, UMessageData());
    currentParamData.emplace(EVENT_DATA_UPDATE_RAIL_PRESSURE_SET, UMessageData());
    currentParamData.emplace(EVENT_DATA_UPDATE_RAIL_PRESSURE_ACT, UMessageData());
    currentParamData.emplace(EVENT_DATA_UPDATE_AIR_MASS_SET, UMessageData());

    // initialize
    currentParamData[EVENT_DATA_UPDATE_BATTERY_VOLTAGE].battery_voltage = 0.0;
    currentParamData[EVENT_DATA_UPDATE_MOTOR_TEMPERATURE].motor_temperature = 0.0;
    currentParamData[EVENT_DATA_UPDATE_AIR_MASS].air_mass = 0.0;
    currentParamData[EVENT_DATA_UPDATE_INTAKE_AIR_TEMPERATURE].intake_air_temperature = 0.0;
    currentParamData[EVENT_DATA_UPDATE_BOOST_PRESSURE_SET].boost_pressure_set = 0.0;
    currentParamData[EVENT_DATA_UPDATE_BOOST_PRESSURE_ACT].boost_pressure_act = 0.0;
    currentParamData[EVENT_DATA_UPDATE_RAIL_PRESSURE_SET].rail_pressure_set = 0.0;
    currentParamData[EVENT_DATA_UPDATE_RAIL_PRESSURE_ACT].rail_pressure_act = 0.0;
    currentParamData[EVENT_DATA_UPDATE_AIR_MASS_SET].air_mass_set = 0.0;

}

void Diagnose_Motor1View::setupScreen()
{
    Diagnose_Motor1ViewBase::setupScreen();
}

void Diagnose_Motor1View::tearDownScreen()
{
    Diagnose_Motor1ViewBase::tearDownScreen();
}

void Diagnose_Motor1View::DiagnoseMotorListUpdateItem(ParameterListElement& item, int16_t itemIndex)
{
    switch(itemIndex)
    {
    	case 0:
    		item.setParameterString(std::string("Battery voltage [V]"));
    		item.setValue(currentParamData[EVENT_DATA_UPDATE_BATTERY_VOLTAGE].battery_voltage);
    		break;
    	case 1:
    		item.setParameterString(std::string("Motor temperature [C]"));
    		item.setValue(currentParamData[EVENT_DATA_UPDATE_MOTOR_TEMPERATURE].motor_temperature);
    		break;
    	case 2:
    		item.setParameterString(std::string("Air mass [kg/h]"));
    		item.setValue(currentParamData[EVENT_DATA_UPDATE_AIR_MASS].air_mass);
    		break;
    	case 3:
    		item.setParameterString(std::string("Intake air temperature [C]"));
    		item.setValue(currentParamData[EVENT_DATA_UPDATE_INTAKE_AIR_TEMPERATURE].intake_air_temperature);
    		break;
    	case 4:
    		item.setParameterString(std::string("Boost pressure set [mbar]"));
    		item.setValue(currentParamData[EVENT_DATA_UPDATE_BOOST_PRESSURE_SET].boost_pressure_set);
    		break;
    	case 5:
    		item.setParameterString(std::string("Boost pressure act [mbar]"));
    		item.setValue(currentParamData[EVENT_DATA_UPDATE_BOOST_PRESSURE_ACT].boost_pressure_act);
    		break;
    	case 6:
    		item.setParameterString(std::string("Rail pressure set [bar]"));
    		item.setValue(currentParamData[EVENT_DATA_UPDATE_RAIL_PRESSURE_SET].rail_pressure_set);
    		break;
    	case 7:
    		item.setParameterString(std::string("Rail pressure act [bar]"));
    		item.setValue(currentParamData[EVENT_DATA_UPDATE_RAIL_PRESSURE_ACT].rail_pressure_act);
    		break;
    	case 8:
    		item.setParameterString(std::string("Air mass set [mg/pass]"));
    		item.setValue(currentParamData[EVENT_DATA_UPDATE_AIR_MASS_SET].air_mass_set);
    		break;
        default:
            break;
    }
}

void Diagnose_Motor1View::UpdateParameterValue(EEventType eventType, const UMessageData& data)
{
	switch(eventType)
	{
	    case EVENT_DATA_UPDATE_BATTERY_VOLTAGE:
        {
            currentParamData[EVENT_DATA_UPDATE_BATTERY_VOLTAGE].battery_voltage = data.battery_voltage;
            break;
        }
	    case EVENT_DATA_UPDATE_MOTOR_TEMPERATURE:
        {
            currentParamData[EVENT_DATA_UPDATE_MOTOR_TEMPERATURE].motor_temperature = data.motor_temperature;
            break;
        }
	    case EVENT_DATA_UPDATE_AIR_MASS:
        {
            currentParamData[EVENT_DATA_UPDATE_AIR_MASS].air_mass = data.air_mass;
            break;
        }
	    case EVENT_DATA_UPDATE_INTAKE_AIR_TEMPERATURE:
        {
            currentParamData[EVENT_DATA_UPDATE_INTAKE_AIR_TEMPERATURE].intake_air_temperature = data.intake_air_temperature;
            break;
        }
	    case EVENT_DATA_UPDATE_BOOST_PRESSURE_SET:
        {
            currentParamData[EVENT_DATA_UPDATE_BOOST_PRESSURE_SET].boost_pressure_set = data.boost_pressure_set;
            break;
        }
	    case EVENT_DATA_UPDATE_BOOST_PRESSURE_ACT:
        {
            currentParamData[EVENT_DATA_UPDATE_BOOST_PRESSURE_ACT].boost_pressure_act = data.boost_pressure_act;
            break;
        }
	    case EVENT_DATA_UPDATE_RAIL_PRESSURE_SET:
        {
            currentParamData[EVENT_DATA_UPDATE_RAIL_PRESSURE_SET].rail_pressure_set = data.rail_pressure_set;
            break;
        }
	    case EVENT_DATA_UPDATE_RAIL_PRESSURE_ACT:
        {
            currentParamData[EVENT_DATA_UPDATE_RAIL_PRESSURE_ACT].rail_pressure_act = data.rail_pressure_act;
            break;
        }
	    case EVENT_DATA_UPDATE_AIR_MASS_SET:
        {
            currentParamData[EVENT_DATA_UPDATE_AIR_MASS_SET].air_mass_set = data.air_mass_set;
            break;
        }
        default:
            break;
	}
}

void Diagnose_Motor1View::setTemperature(short int temperature)
{
    touchgfx::Unicode::snprintf(Text_TemperatureBuffer, TEXT_TEMPERATURE_SIZE, "%d", static_cast<int>(temperature));
    Text_Temperature.invalidate();
}

void Diagnose_Motor1View::setDate(uint8_t day, uint8_t month, unsigned int year)
{
    touchgfx::Unicode::snprintf(Text_DateBuffer, TEXT_DATE_SIZE, "%02d.%02d.%04d", static_cast<int>(day), static_cast<int>(month), static_cast<int>(year));
    Text_Date.invalidate();
}

void Diagnose_Motor1View::setTime(uint8_t hour, uint8_t minute)
{
    Clock.setTime24Hour(hour, minute, 0);
    Clock.invalidate();
}