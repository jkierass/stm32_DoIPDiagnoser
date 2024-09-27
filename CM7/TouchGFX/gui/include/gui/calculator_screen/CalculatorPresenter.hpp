#ifndef CALCULATORPRESENTER_HPP
#define CALCULATORPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class CalculatorView;

class CalculatorPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    CalculatorPresenter(CalculatorView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~CalculatorPresenter() {}

private:
    CalculatorPresenter();

    CalculatorView& view;
};

#endif // CALCULATORPRESENTER_HPP
