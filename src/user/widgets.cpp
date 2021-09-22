
#ifndef WIDGETS_CPP_QM_REBORN
#define WIDGETS_CPP_QM_REBORN

#include "main.h"
#include "linkedList.hpp"
#include "types.h"
#include "widgets.h"
#include "event_handler.hpp"
#include "config_mgr.h"
#include <kernel/libkernel.h>
#include <registrymgr.h>

static widget::Widget *(*getImposeRoot)();
//Power Manage Plugin
static Plugin *imposePlugin;
//Impose Plugin
static Plugin *actualImposePlugin;
static widget::Widget *powerRoot;
static Widget *main_plane;

linked_list currentWidgets;

const char *widgetStyles[] = 
{
    "_common_default_style_button",
    "_common_default_style_check_box",
    "_common_default_style_text",
    "_common_default_style_plane",
    "30396CEA" // Hex
};

const char *widgetTypes[] = 
{
    "button",
    "check_box",
    "text",
    "plane",
    "slidebar"
};

Widget *findWidgetByHash(SceUInt32 hash)
{
    Resource::Element find;
    find.hash = hash;

    Widget *found = getImposeRoot()->GetChildByHash(&find, 0);
    return found;
}

int initWidgets()
{
    //Credit to GrapheneCT
    //Get power manage plugin object
    imposePlugin = Plugin::Find("power_manage_plugin");
    NULL_ERROR_FAIL(imposePlugin);

    actualImposePlugin = Plugin::Find("impose_plugin");
    NULL_ERROR_FAIL(actualImposePlugin);

    //Power manage plugin -> power manage root
    powerRoot = imposePlugin->GetInterface(1);
    NULL_ERROR_FAIL(powerRoot);

    //Power manage root -> impose root (some virtual function)
    getImposeRoot = (widget::Widget *(*)()) *(int *)((int)powerRoot + 0x54);
    NULL_ERROR_FAIL(getImposeRoot);
    main_plane = findWidgetByHash(SCROLL_VIEW_BOX_ID);
    NULL_ERROR_FAIL(main_plane);
    return 0;
}

SceUInt32 getHashByID(const char *id)
{    
    Resource::Element sinfo;
    Resource::Element searchRequest;

    searchRequest.id.Set(id);

    sinfo.hash = searchRequest.GetHashById(&searchRequest);
    return sinfo.hash;
}

Widget *makeWidget(const char *refId, const char *styleInfo, const char *type, Widget *parent)
{
    //WidgetInfo
    paf::Resource::Element winfo;
    //SearchInfo
    paf::Resource::Element sinfo;
    //Search Request
    paf::Resource::Element searchRequest;

    searchRequest.id.Set(refId);
    TRY_RET(winfo.GetHashById(&searchRequest), winfo.hash, SceUInt32);
    
    searchRequest.id.Set(styleInfo);
    TRY_RET(sinfo.GetHashById(&searchRequest), sinfo.hash, SceUInt32);
    

    Widget *newWidget;
    TRY_RET(imposePlugin->CreateWidgetWithStyle(parent, type, &winfo, &sinfo), newWidget, Widget *);
    FAIL_IF(newWidget == NULL || newWidget < 0);
    return newWidget;
}

Widget *makeWidget(const char *refId, int styleHash, const char *type, Widget *parent)
{
    //WidgetInfo
    paf::Resource::Element winfo;
    //StyleInfo
    paf::Resource::Element sinfo;
    //Search Request
    paf::Resource::Element searchRequest;

    searchRequest.id.Set(refId);
    TRY_RET(winfo.GetHashById(&searchRequest), winfo.hash, SceUInt32);
    
    sinfo.hash = styleHash;    
    
    Widget *newWidget;
    TRY_RET(actualImposePlugin->CreateWidgetWithStyle(parent, type, &winfo, &sinfo), newWidget, Widget *);
    if(newWidget == NULL) print("Error can't make widget with refID %s, type %s, styleid = 0x%X\n", refId, type, styleHash);
    FAIL_IF(newWidget == NULL || newWidget < 0);
    return newWidget;
}

int unregisterWidget(const char *refId)
{
    #ifdef DEBUG
    SCE_DBG_LOG_INFO("Called unregister widget, refId: %s\n", refId);
    #endif
    //If refID not given, bail out
    if(sce_paf_strcmp(refId, "") == 0) return 0;

    //Remove widget from list
    currentWidgets.remove_node(refId);
    currentWidgets.printall();

    return 0;
}

int registerWidget(widgetData *data)
{
    #ifdef DEBUG
    print("Internal add call\n");
    #endif
    //This is just a wrapper, it'll add the widgets to a linked list, widgets are made on demand when impose menu loads via another thread
    currentWidgets.add_node(data);
    #ifdef DEBUG
    currentWidgets.printall();
    #endif

    SceAppMgrAppState state;
    sceAppMgrGetAppState(&state);
    if(state.isSystemUiOverlaid)
    {
        spawn(data, UPDATE_ALL);
        if(data->OnLoad != NULL) data->OnLoad();
    }

    return 0;
}

int setText(const char *text, Widget *widget)
{
    WString wstr;
    String str;
    str.Set(text);
    str.ToWString(&wstr);
    print("Got Actual Text: %s got text in String %s got Text in WString %ls\n", text, str.data, wstr.data);
    return widget->SetLabel(&wstr);
}

int update_Widget(widgetData *data, int flags)
{
    print("UPDATING NODE\n");
    
    //Update the widget in the linked list for when it needs to be redrawn
    currentWidgets.update_node(data, flags);

    print("DONE!!!\n");

    SceAppMgrAppState state;
    sceAppMgrGetAppState(&state);
    //If quickMenu is being displayed, update the current widget
    if(state.isSystemUiOverlaid)
    {
        print("EDITING WIDGET\n");
        widgetData *dat;
        if((dat = currentWidgets.get_node(data->refId)) != NULL)
            editWidget(dat, flags);
        print("DONE\n");
    }
    
    return 0;
}

void dummyprint(const char *fmt, ...)
{
    //Just to let the snc compiler acually freaking compile!
    (void)fmt;
}

int updateValues(Widget *made, widgetData *widget, int flags)
{
    SceFVector4 pos = makeSceVector4(widget->pos);
    SceFVector4 size = makeSceVector4(widget->size);
    Widget::Color col = makeSceColor(widget->col);
    
    if(flags & UPDATE_POSITION) made->SetPosition(&pos);
    if(flags & UPDATE_SIZE) made->SetSize(&size);
    if(flags & UPDATE_COLOR) made->SetFilterColor(&col);

    QMEventHandler *eh = new QMEventHandler();
    switch (widget->type)
    {
        case button:
        {
            if(flags && UPDATE_EVENT)
            {
                eh->pUserData = sce_paf_malloc(sizeof(widgetData));
                sce_paf_memcpy(eh->pUserData, widget, sizeof(widgetData));
                made->RegisterEventCallback(ON_PRESS_EVENT_ID, eh, 0);
            }
            if(flags && UPDATE_TEXT) 
            {
                setText(widget->data.ButtonData.label, made);
            }

            break;
        }

        case check_box:
        {
            if(flags && UPDATE_EVENT)
            {
                eh->pUserData = sce_paf_malloc(sizeof(widgetData));
                sce_paf_memcpy(eh->pUserData, widget, sizeof(widgetData));
                made->RegisterEventCallback(ON_PRESS_EVENT_ID, eh, 0);
            }
            break;
        }

        case slidebar:
        {
            delete eh;
            break;
        }
        
        case text:
        {
            delete eh;
            if(flags && UPDATE_TEXT)
            {
                setText(widget->data.TextData.label, made);
                made->SetOption(Widget::Option::Text_Bold, 0,0,widget->data.TextData.isbold ? SCE_TRUE : SCE_FALSE);
            }
            break;
        }

        default:
        {
            delete eh;
            break;
        }
    }
    return 0;
}

int editWidget(widgetData *data, int flags)
{
    
    Widget *toEdit = findWidgetByHash(getHashByID(data->refId));
    updateValues(toEdit, data, flags);
    return 0;
}

int setupValues(Widget *widget, widgetData *dat)
{
    switch (dat->type)
    {
    case check_box:
    {
        print("Got state %d\n", dat->data.CheckBoxData.state);
        int toSet = 0;
        switch (dat->data.CheckBoxData.state)
        {
        case CHECKBOX_ON:
            toSet = 1;
            break;
        case CHECKBOX_PREV_STATE:
            toSet = readCheckBoxState(dat->refId);
            if(toSet == CONFIG_MGR_ERROR_NOT_EXIST) toSet = 0;
            break;
        case CHECKBOX_OFF:
        default:
            toSet = 0;
            break;
        }
        ((CheckBox *)widget)->SetChecked(0, toSet, 0);
        break;
    }
    case text:
    {
        widget->SetOption(Widget::Option::Text_Bold, 0, 0, SCE_FALSE);
        break;
    }
    case slidebar:
    {
        SceFVector4 size = makeSceVector4(65, 65, 0, 0);


        Widget *ball = widget->GetChildByNum(0);
        if(ball != NULL) ball->SetSize(&size);

        sceClibPrintf("%s\n", ((SlideBar *)widget)->TypeSlideBar());
        break;
    }
    
    default:
        break;
    }
    return 0;
}

int spawn(widgetData *widget, int flags)
{
    Widget *made;
    print("Got widget %s parent %s\n", widget->refId, widget->parentRefId);
    if(widget->isAdvanced)
    {
        if(widget->adata.useHash)
            made = makeWidget(widget->refId, widget->adata.hash, widget->adata.type, (widget->hasParent == 0) ? main_plane : findWidgetByHash(getHashByID(widget->parentRefId)));
        else
            made = makeWidget(widget->refId, widget->adata.styleInfo, widget->adata.type, (widget->hasParent == 0) ? main_plane : findWidgetByHash(getHashByID(widget->parentRefId)));
    }
    else
    {
        int style = 0;
        if(style = (int)sceClibStrtoll(widgetStyles[widget->type], NULL, 16), style != 0)
            made = makeWidget(widget->refId, style, widgetTypes[widget->type], (widget->hasParent == 0) ? main_plane : findWidgetByHash(getHashByID(widget->parentRefId)));
        else
            made = makeWidget(widget->refId, widgetStyles[widget->type], widgetTypes[widget->type], (widget->hasParent == 0) ? main_plane : findWidgetByHash(getHashByID(widget->parentRefId)));
    }


#ifdef DEBUG
    print("Adding widget with settings:\nrefId: %s\nparentRefID: %s\nhasParent: %s\nisAdvanced: %s", widget->refId, widget->parentRefId, widget->hasParent ? "True" : "False", widget->isAdvanced ? "True" : "False");
#endif

    NULL_ERROR_FAIL(made);
    updateValues(made, widget, widget->isAdvanced ? UPDATE_COLOR | UPDATE_POSITION | UPDATE_SIZE : flags);
    print("DONE\n");
    print("OnLoad %s NULL\n", widget->OnLoad == NULL ? "==" : "!=");
    print("Setting up values, %s\n", widget->refId);
    setupValues(made, widget);
    print("DONE ALL\n");
    return 0;
}

int displayWidgets()
{
    node *current = currentWidgets.head;
    while(current != NULL)
    {
        spawn(&current->widget, UPDATE_ALL);
        print("ONLOAD IS %s\n", current->widget.OnLoad == NULL ? "NULL" : "NOT NULL");
        if(current->widget.OnLoad != NULL) current->widget.OnLoad();
        current = current->next;
    }
    return 0;
}
#endif