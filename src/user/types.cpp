#include "main.h"
#include "types.h"

WString makeWString(const char *string)
{
    String *str = new String((char *)string);
    WString wstr;
    str->ToWString(&wstr);
    delete str;
    return wstr;
}

Widget::Color makeSceColor(float r, float g, float b, float a)
{
    Widget::Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

Widget::Color makeSceColor(widgetColor col)
{
    Widget::Color color;
    color.r = col.r;
    color.g = col.g;
    color.b = col.b;
    color.a = col.a;
    return color;
}

SceFVector4 makeSceVector4(float x, float y, float z, float w)
{
    SceFVector4 toReturn;
    toReturn.x = x;
    toReturn.y = y;
    toReturn.z = z;
    toReturn.w = w;

    return toReturn;
}



SceFVector4 makeSceVector4(vector4 *vect)
{
    SceFVector4 toret;
    toret.x = vect->x;
    toret.y = vect->y;
    toret.z = vect->z;
    toret.w = vect->w;

    return toret;
}

SceFVector4 makeSceVector4(vector4 vect)
{
    SceFVector4 toret;
    toret.x = vect.x;
    toret.y = vect.y;
    toret.z = vect.z;
    toret.w = vect.w;

    return toret;
}
