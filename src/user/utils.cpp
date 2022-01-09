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
    w->SetColor(&scol);
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
    w->SetColor(&col);
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

    searchRequest.id = (id);

    sinfo.hash = searchRequest.GetHashById(&searchRequest);
    return sinfo.hash;
}

Widget *Utils::FindWidget(SceUInt32 hash)
{
    return Utils::GetChildByHash(getImposeRoot(), hash);
}

SceInt32 Utils::SetWidgetLabel(const char *label, Widget *w)
{

    if(label == NULL || label[0] == '\0' || sce_paf_strlen(label) == 0) return;

    WString wstr;
    WString::CharToNewWString(label, &wstr);

    w->SetLabel(&wstr);
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

SceVoid Utils::DeleteTexture(graphics::Texture *tex, bool deletePointer)
{
    if(tex != NULL)
    {
        if(tex->texSurface != NULL)
        {
            graphics::Surface *s = tex->texSurface;
            tex->texSurface = SCE_NULL;
            delete s;
        }
        if(deletePointer)
            delete tex;
    }
}

SceInt32 Utils::CreateTextureFromFile(graphics::Texture *tex, const char *file)
{
    if(tex == NULL) return SCE_FALSE;

    ObjectWithCleanup openResult;
    SceInt32 err;
    LocalFile::Open(&openResult, file, SCE_O_RDONLY, 0666, &err);

    if(err < 0)
        return err;

    graphics::Texture::CreateFromFile(tex, imposePlugin->memoryPool, &openResult);

    openResult.cleanup->cb(openResult.object);
    delete openResult.cleanup;

    return tex->texSurface != NULL ? 0 : -1;
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
    return paf::io::Misc::Exists(path);
}

void dummyprint(const char *format, ...){}