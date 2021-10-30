#ifndef QM_REBORN_EH_HPP
#define QM_REBORN_EH_HPP


#include "utils.hpp"
#include "widgets.hpp"
#include "linkedList.hpp"

class QMREventHandler : public Widget::EventCallback
{
public:
    QMREventHandler();

    static void onGet(SceInt32 , Widget *self, SceInt32, ScePVoid puserData);
};

#endif