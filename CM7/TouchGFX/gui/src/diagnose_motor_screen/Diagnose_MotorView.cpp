#include <gui/diagnose_motor_screen/Diagnose_MotorView.hpp>

Diagnose_MotorView::Diagnose_MotorView()
{

}

void Diagnose_MotorView::DiagnoseMotorListUpdateItem(ParameterListElement& item, int16_t itemIndex)
{
    switch(itemIndex)
    {
    	case 0:
    		item.setParameterString(std::string("Battery voltage [V]"));
    		item.setValue(static_cast<float>(15.123));
    		break;
    	case 1:
    		item.setParameterString(std::string("Motor temperature [C]"));
    		item.setValue(static_cast<float>(50.1));
    		break;
    	case 2:
    		item.setParameterString(std::string("Air mass [kg/h]"));
    		item.setValue(static_cast<float>(350.23));
    		break;
    	case 3:
    		item.setParameterString(std::string("Intake air temperature [C]"));
    		item.setValue(static_cast<float>(60.8));
    		break;
    	case 4:
    		item.setParameterString(std::string("Boost pressure set [mbar]"));
    		item.setValue(static_cast<float>(1921.12));
    		break;
    	case 5:
    		item.setParameterString(std::string("Boost pressure act [mbar]"));
    		item.setValue(static_cast<float>(1823.71));
    		break;
    	case 6:
    		item.setParameterString(std::string("Rail pressure set [bar]"));
    		item.setValue(static_cast<float>(1024.64));
    		break;
    	case 7:
    		item.setParameterString(std::string("Rail pressure act [bar]"));
    		item.setValue(static_cast<float>(1024.99));
    		break;
    	case 8:
    		item.setParameterString(std::string("Air mass set [mg/pass]"));
    		item.setValue(static_cast<float>(523.13));
    		break;
    	case 9:
    		item.setParameterString(std::string("Air mass act [mg/pass]"));
    		item.setValue(static_cast<float>(590.45));
    		break;
    	case 10:
    		item.setParameterString(std::string("Ambient pressure [mbar]"));
    		item.setValue(static_cast<float>(930.12));
    		break;
    	case 11:
    		item.setParameterString(std::string("Fuel temperature [C]"));
    		item.setValue(static_cast<float>(40.99));
    		break;
//    	case 12:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 13:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 14:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 15:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 16:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 17:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 18:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 19:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 20:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 21:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 22:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 23:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 24:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 25:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
//    	case 26:
//    		item.setParameterString();
//    		item.setValue();
//    		break;
    	default:
    		break;
    }
}

void Diagnose_MotorView::setupScreen()
{
    Diagnose_MotorViewBase::setupScreen();
}

void Diagnose_MotorView::tearDownScreen()
{
    Diagnose_MotorViewBase::tearDownScreen();
}
