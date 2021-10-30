#include "main.h"
#include "utils.hpp"
#include "common.hpp"

Widget::Color Utils::MakeSceColor(float r, float g, float b, float a)
{
    Widget::Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

Widget::Color Utils::MakeSceColor(widgetColor col)
{
    Widget::Color color;
    color.r = col.r;
    color.g = col.g;
    color.b = col.b;
    color.a = col.a;
    return color;
}

Widget::Color Utils::MakeSceColor(widgetColor *col)
{
    Widget::Color color;
    color.r = col->r;
    color.g = col->g;
    color.b = col->b;
    color.a = col->a;
    return color;
}

SceFVector4 Utils::MakeSceVector4(float x, float y, float z, float w)
{
    SceFVector4 toReturn;
    toReturn.x = x;
    toReturn.y = y;
    toReturn.z = z;
    toReturn.w = w;

    return toReturn;
}

SceVoid Utils::SetWidgetColor(Widget *w, widgetColor *col)
{
    Widget::Color scol = Utils::MakeSceColor(col);
    w->SetFilterColor(&scol);
}

SceVoid Utils::SetWidgetPosition(Widget *w, vector4 *pos)
{
    SceFVector4 spos = Utils::MakeSceVector4(pos);
    w->SetPosition(&spos);
}

SceVoid Utils::SetWidgetSize(Widget *w, vector4 *size)
{
    SceFVector4 ssize = Utils::MakeSceVector4(size);
    w->SetSize(&ssize);
}

SceVoid Utils::SetWidgetColor(Widget *w, float r, float g, float b, float a)
{
    Widget::Color col;
    col.r = r;
    col.g = g;
    col.b = b;
    col.a = a;
    w->SetFilterColor(&col);
}

SceVoid Utils::SetWidgetPosition(Widget *wi, float x, float y, float z, float w)
{
    SceFVector4 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    wi->SetPosition(&v);
}

SceVoid Utils::SetWidgetSize(Widget *wi, float x, float y, float z, float w)
{
    SceFVector4 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    wi->SetSize(&v);
}

Widget *Utils::GetChildByHash(Widget *Parent, SceUInt32 hash)
{
    if(Parent == NULL) return NULL;
    Resource::Element e;
    e.hash = hash;
    return Parent->GetChildByHash(&e, 0);
}

SceUInt32 Utils::GetHashById(const char *id)
{   
    Resource::Element sinfo;
    Resource::Element searchRequest;

    searchRequest.id.Set(id);

    sinfo.hash = searchRequest.GetHashById(&searchRequest);
    return sinfo.hash;
}

Widget *Utils::FindWidget(SceUInt32 hash)
{
    return Utils::GetChildByHash(getImposeRoot(), hash);
}

SceInt32 Utils::SetWidgetLabel(const char *text, Widget *widget)
{
    WString wstr;
    String str;
    str.Set(text);
    str.ToWString(&wstr);

    return widget->SetLabel(&wstr);
}

SceFVector4 Utils::MakeSceVector4(vector4 *vect)
{
    SceFVector4 toret;
    toret.x = vect->x;
    toret.y = vect->y;
    toret.z = vect->z;
    toret.w = vect->w;

    return toret;
}

SceFVector4 Utils::MakeSceVector4(vector4 vect)
{
    SceFVector4 toret;
    toret.x = vect.x;
    toret.y = vect.y;
    toret.z = vect.z;
    toret.w = vect.w;

    return toret;
}

bool widgetsDisplayed()
{
    SceAppMgrAppState s;
    sceAppMgrGetAppState(&s);
    return s.isSystemUiOverlaid && displayed;
}

bool checkFileExist(const char *path)
{
	SceIoStat stat;
	return sceIoGetstat(path, &stat) >= 0;
}