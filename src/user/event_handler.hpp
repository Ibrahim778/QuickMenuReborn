#ifndef QM_REBORN_EH_HPP
#define QM_REBORN_EH_HPP


#include "types.h"
#include "widgets.h"
#include "linkedList.hpp"
#include "types.h"

class QMEventHandler : public Widget::EventCallback
{
public:
    QMEventHandler();

    static void onGet(SceInt32 , Widget *self, SceInt32, ScePVoid puserData);
};

#endif