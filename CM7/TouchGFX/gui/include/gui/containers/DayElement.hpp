#ifndef DAYELEMENT_HPP
#define DAYELEMENT_HPP

#include <gui_generated/containers/DayElementBase.hpp>

class DayElement : public DayElementBase
{
public:
    DayElement();
    virtual ~DayElement() {}

    virtual void initialize();

    void setNumber(int no);

protected:
};

#endif // DAYELEMENT_HPP
