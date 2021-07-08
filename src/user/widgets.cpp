
#ifndef WIDGETS_CPP_QM_REBORN
#define WIDGETS_CPP_QM_REBORN

#include "widgets.h"
#include "linkedList.cpp"
#include <kernel/libkernel.h>
#include "event_handler.cpp"

static widget::Widget *(*getImposeRoot)();
static Plugin *imposePlugin;
static widget::Widget *powerRoot;
static Widget *main_plane;

const char *idTypes[] = 
{
    "_common_default_style_button",
    "_common_default_style_check_box",
    "_common_default_style_text",
    "_common_default_style_plane"
};

const char *widgetTypes[] = 
{
    "button",
    "check_box",
    "text",
    "plane"
};

linked_list currentWidgets;

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
    imposePlugin = Plugin::GetByName("power_manage_plugin");
    NULL_ERROR_FAIL(imposePlugin);
    //Power manage plugin -> power manage root
    powerRoot = imposePlugin->GetWidgetByNum(1);
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

Widget *makeWidget(const char *refId, const char *idType, const char *type, Widget *parent)
{
    //WidgetInfo
    paf::Resource::Element winfo;
    //SearchInfo
    paf::Resource::Element sinfo;
    //Search Request
    paf::Resource::Element searchRequest;

    searchRequest.id.Set(refId);
    TRY_RET(winfo.GetHashById(&searchRequest), winfo.hash, SceUInt32);
    
    searchRequest.id.Set(idType);
    TRY_RET(sinfo.GetHashById(&searchRequest), sinfo.hash, SceUInt32);
    

    Widget *newWidget;
    TRY_RET(imposePlugin->CreateWidgetWithStyle(parent, type, &winfo, &sinfo), newWidget, Widget *);
    FAIL_IF(newWidget == NULL || newWidget < 0);
    return newWidget;
}

int unregisterWidget(const char *refId)
{
    if(sce_paf_strcmp(refId, "") == 0) return 0;
    currentWidgets.remove_node(refId);
    currentWidgets.print();
    return 0;
}

int registerWidget(widgetData *data)
{
    SCE_DBG_LOG_INFO("Internal add call\n");
    //This is just a wrapper, it'll add the widgets to a linked list, widgets are made on demand when impose menu loads via another thread
    currentWidgets.add_node(data);
    currentWidgets.print();
    return 0;
}

int setText(const char *text, Widget *widget)
{
    WString wstr = makeWString((const char *)text);
    return widget->SetLabel(&wstr);
}

int updateWidget(widgetData *data, int flags)
{
    //Update the widget in the linked list for when it needs to be redrawn
    currentWidgets.update_node(data);

    SceAppMgrAppState state;
    sceAppMgrGetAppState(&state);
    //If quickMenu is being displayed, update the current widget
    if(state.isSystemUiOverlaid)
        editWidget(data, flags);
    
    return 0;
}

int updateValues(Widget *made, widgetData *widget, int flags)
{
    SceFVector4 pos = makeSceVector4(widget->pos);
    SceFVector4 size = makeSceVector4(widget->size);
    Widget::Color col = makeSceColor(widget->col);
    
    if(flags & UPDATE_POSITION) made->SetPosition(&pos);
    if(flags & UPDATE_SIZE) made->SetSize(&size);

    QMEventHandler *eh = new QMEventHandler();
    switch (widget->type)
    {
        case button:
        {
            if(flags & UPDATE_EVENT)
            {
                eh->pUserData = sce_paf_malloc(sizeof(widgetData));
                sce_paf_memcpy(eh->pUserData, &widget, sizeof(widgetData));
                made->RegisterEventCallback(ON_PRESS_EVENT_ID, eh, 0);
            }
            if(flags & UPDATE_COLOR) made->SetFilterColor(&col);
            if(flags & UPDATE_TEXT) setText(widget->data.ButtonData.label, made);
            break;
        }

        case check_box:
        {
            if(flags & UPDATE_EVENT)
            {
                eh->pUserData = sce_paf_malloc(sizeof(widgetData));
                sce_paf_memcpy(eh->pUserData, &widget, sizeof(widgetData));
                made->RegisterEventCallback(ON_PRESS_EVENT_ID, eh, 0);
            }
            if(flags & UPDATE_COLOR) made->SetFilterColor(&col);
            break;
        }
        
        case text:
        {
            delete eh;
            if(flags & UPDATE_COLOR) made->SetFilterColor(&col);
            if(flags & UPDATE_TEXT)
            {
                //made->SetOption(Widget::Option::Text_Bold, 0,0,widget.data.TextData.isbold);
                setText(widget->data.TextData.label, made);
            }
            break;
        }
        
        case plane:
        {
            delete eh;
            if(flags & UPDATE_COLOR) made->SetFilterColor(&col);
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

int spawn(node *node, int flags)
{
    Widget *made;
    made = 
    makeWidget(node->widget->refId, 
    (char *)idTypes[node->widget->type], 
    (char *)widgetTypes[node->widget->type], 
    ((sce_paf_strlen(node->widget->parentRefId) == 0) ? main_plane : findWidgetByHash(getHashByID(node->widget->parentRefId))));
    sceClibPrintf("Adding widget with settings:\n refId: %s\nparentRefID: %s\n", node->widget->refId, node->widget->parentRefId);
    NULL_ERROR_FAIL(made);

    updateValues(made, node->widget, flags);
    return 0;
}

int displayWidgets()
{
    node *current = currentWidgets.head;
    while(current != NULL)
    {
        SCE_DBG_LOG_INFO("Spawning widget:  %s\n", current->widget->refId);
        spawn(current, UPDATE_ALL);
        current = current->next;
    }
    
    return 0;
}

#endif