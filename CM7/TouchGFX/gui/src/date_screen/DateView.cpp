#include <gui/date_screen/DateView.hpp>

DateView::DateView()
{

}

void DateView::scrollWheel_MonthUpdateItem(DayElement& item, int16_t itemIndex)
{
	item.setNumber(1 + itemIndex);
}

void DateView::scrollWheel_DayUpdateItem(DayElement& item, int16_t itemIndex)
{
	item.setNumber(1 + itemIndex);
}

void DateView::scrollWheel_MinuteUpdateItem(DayElement& item, int16_t itemIndex)
{
	item.setNumber(itemIndex);
}

void DateView::scrollWheel_HourUpdateItem(DayElement& item, int16_t itemIndex)
{
	item.setNumber(itemIndex);
}

void DateView::scrollWheel_YearUpdateItem(DayElement& item, int16_t itemIndex)
{
	item.setNumber(1950 + itemIndex);
}

void DateView::setupScreen()
{
    DateViewBase::setupScreen();
}

void DateView::tearDownScreen()
{
    DateViewBase::tearDownScreen();
}
