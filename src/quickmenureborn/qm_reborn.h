#ifndef QM_REBORN_H
#define QM_REBORN_H

#include "c_types.h"

#define EXPORT extern 

#ifndef QM_REBORN_INTERNAL
#undef EXPORT
#define EXPORT
#endif

EXPORT
int addWidget(widgetData *data);

EXPORT
int updateWidget(widgetData *data, int flags);

EXPORT
int removeWidget(const char *refID);

EXPORT
widgetColor makeWidgetColor(float r, float g, float b, float a);
EXPORT
vector4 makeWidgetVector4(float x, float y, float z, float w);

#define COLOR_WHITE makeWidgetColor(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_RED makeWidgetColor(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GREEN makeWidgetColor(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_BLUE makeWidgetColor(0.0f, 0.0f, 1.0f, 1.0f)
#define COLOR_YELLOW makeWidgetColor(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_LIGHT_BLUE makeWidgetColor(0.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_PINK makeWidgetColor(1.0f, 0, 1.0f, 1.0f)
#define COLOR_YELLOW makeWidgetColor(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_GRAY makeWidgetColor(0.5f, 0.5f, 0.5f, 0.5f)

#define makeCommonWidgetColor(common) makeWidgetColor(common, common, common, common)
#define makeCommonWidgetVector4(common) makeWidgetVector4(common, common, common, common)

#define makeWidgetVector4Int(x,y,z,w) makeWidgetVector4(x##.0f, y##.0f, z##.0f, w##.0f)
#define makeWidgetColorInt(r,g,b,a) makeWidgetColor(r##.0f, g##.0f, b##.0f, a##.0f)

#define makeCommonWidgetVector4Int(common) makeWidgetVector4(common##.0f, common##.0f, common##.0f, common##.0f)
#define makeCommonWidgetColorInt(common) makeWidgetColor(common##.0f, common##.0f, common##.0f, common##.0f)


#define QuickMenuRebornButton(refID, parentRefID, Size, Position, Color, _Text, OnPress) \
{\
    widgetData data;\
    data.col = Color;\
    data.parentRefId = parentRefID;\
    data.pos = Position;\
    data.size = Size;\
    data.type = button;\
    data.data.ButtonData.onPress = OnPress;\
    data.refId = refID;\
    data.data.ButtonData.label = _Text;\
    addWidget(&data);\
}

#define QuickMenuRebornText(refID, parentRefID, Size, Position, Color, _Text) \
{\
    widgetData data;\
    data.col = Color;\
    data.parentRefId = parentRefID;\
    data.pos = Position;\
    data.size = Size;\
    data.type = text;\
    data.refId = refID;\
    data.data.ButtonData.label = _Text;\
    addWidget(&data);\
}

#define QuickMenuRebornCheckBox(refID, parentRefID, Size, Position, Color, onToggle) \
{\
    widgetData data;\
    data.col = Color;\
    data.parentRefId = parentRefID;\
    data.pos = Position;\
    data.size = Size;\
    data.type = check_box;\
    data.data.CheckBoxData.OnToggle = onToggle;\
    data.refId = refID;\
    addWidget(&data);\
}

#define QuickMenuRebornPlane(refID, parentRefID, Size, Position, Color)\
{\
    widgetData data;\
    data.refId = refID;\
    data.parentRefId = parentRefID;\
    data.pos = Position;\
    data.size = Size;\
    data.col = Color;\
    data.type = plane;\
    addWidget(&data);\
}


#define QuickMenuRebornUpdateButton(refID, Size, Position, Color, _Text, OnPress, flags) \
{\
    widgetData data;\
    data.col = Color;\
    data.pos = Position;\
    data.size = Size;\
    data.type = button;\
    data.data.ButtonData.onPress = OnPress;\
    data.refId = refID;\
    data.data.ButtonData.label = _Text;\
    updateWidget(data, flags);\
}

#define QuickMenuRebornUpdateCheckBox(refID, Size, Position, Color, onToggle, flags) \
{\
    widgetData data;\
    data.col = Color;\
    data.pos = Position;\
    data.size = Size;\
    data.type = check_box;\
    data.data.CheckBoxData.OnToggle = onToggle;\
    data.refId = refID;\
    updateWidget(data, flags);\
}

#define QuickMenuRebornUpdateText(refID, Size, Position, Color, _Text, flags) \
{\
    widgetData data;\
    data.col = Color;\
    data.pos = Position;\
    data.size = Size;\
    data.type = text;\
    data.data.TextData.label = _Text;\
    data.refId = refID;\
    updateWidget(data, flags);\
}

#define QuickMenuRebornUpdatePlane(refID, Size, Position, Color, flags) \
{\
    widgetData data;\
    data.col = Color;\
    data.pos = Position;\
    data.size = Size;\
    data.type = plane;\
    data.refId = refID;\
    updateWidget(data, flags);\
}

#define QuickMenuRebornSeparator(refID)\
{\
    widgetData data;\
    data.refId = "qm_reborn_" refID "_line";\
    data.parentRefId = NULL;\
    data.pos = makeWidgetVector4(0,0,0,0);\
    data.size = makeWidgetVector4(825.0f,2.0f,0.0f,0.0f);\
    data.col = makeWidgetColor(.75f,.75f,.75f,.75f);\
    data.type = plane;\
    addWidget(&data);\
}

#endif