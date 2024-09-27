#ifndef DIAGNOSEVIEW_HPP
#define DIAGNOSEVIEW_HPP

#include <gui_generated/diagnose_screen/DiagnoseViewBase.hpp>
#include <gui/diagnose_screen/DiagnosePresenter.hpp>

class DiagnoseView : public DiagnoseViewBase
{
public:
    DiagnoseView();
    virtual ~DiagnoseView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // DIAGNOSEVIEW_HPP
