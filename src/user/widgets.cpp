
#ifndef WIDGETS_CPP_QM_REBORN
#define WIDGETS_CPP_QM_REBORN

#include "widgets.h"
#include "linkedList.cpp"
#include <kernel/libkernel.h>
#include <registrymgr.h>
#include "event_handler.cpp"

static widget::Widget *(*getImposeRoot)();
static Plugin *imposePlugin;
static widget::Widget *powerRoot;
static Widget *main_plane;
//Actual Impose_plugin
static Plugin *impose_plugin;
static Widget *impose_pluginFirstWidget;

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
    #ifdef DEBUG
    SCE_DBG_LOG_INFO("Called unregister widget, refId: %s\n", refId);
    #endif
    //If refID not given, bail out
    if(sce_paf_strcmp(refId, "") == 0) return 0;
    
    //Remove widget from list
    currentWidgets.remove_node(refId);
    currentWidgets.print();

    //Make widget impossible to press to avoid triggering dead callbacks
    SceAppMgrAppState state;
    sceAppMgrGetAppState(&state);
    if(state.isSystemUiOverlaid)
    {
        widgetData dat;
        sce_paf_memset(dat.refId, 0, 0x100);
        sce_paf_strncpy(dat.refId, refId, 0x100);

        dat.size.x = 0;
        dat.size.y = 0;

        updateWidget(&dat, UPDATE_SIZE);
    }
    return 0;
}

int registerWidget(widgetData *data)
{
    #ifdef DEBUG
    SCE_DBG_LOG_INFO("Internal add call\n");
    #endif
    //This is just a wrapper, it'll add the widgets to a linked list, widgets are made on demand when impose menu loads via another thread
    currentWidgets.add_node(data);
    #ifdef DEBUG
    currentWidgets.print();
    #endif

    SceAppMgrAppState state;
    sceAppMgrGetAppState(&state);
    if(state.isSystemUiOverlaid)
        spawn(data, UPDATE_ALL);

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
    currentWidgets.update_node(data, flags);

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
    if(flags & UPDATE_COLOR) made->SetFilterColor(&col);

    QMEventHandler *eh = new QMEventHandler();
    switch (widget->type)
    {
        case button:
        {
            if(flags & UPDATE_EVENT)
            {
                sceClibPrintf("Updating event...\n");
                eh->pUserData = sce_paf_malloc(sizeof(widgetData));
                sce_paf_memcpy(eh->pUserData, widget, sizeof(widgetData));
                made->RegisterEventCallback(ON_PRESS_EVENT_ID, eh, 0);
            }
            if(flags & UPDATE_TEXT) 
            {
                setText(widget->data.ButtonData.label, made);
            }

            break;
        }

        case check_box:
        {
            if(flags & UPDATE_EVENT)
            {
                sceClibPrintf("Updating event....\n");
                eh->pUserData = sce_paf_malloc(sizeof(widgetData));
                sce_paf_memcpy(eh->pUserData, &widget, sizeof(widgetData));
                int ret = made->RegisterEventCallback(ON_PRESS_EVENT_ID, eh, 0);
                sceClibPrintf("RET = 0x%X\n", ret);
            }
            break;
        }
        
        case text:
        {
            delete eh;
            if(flags & UPDATE_TEXT)
            {
                made->SetOption((paf::widget::Widget::Option)7, 0,0,widget->data.TextData.isbold ? SCE_TRUE : SCE_FALSE);
                setText(widget->data.TextData.label, made);
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
            int state = 0;
            sceRegMgrGetKeyInt(REG_CONFIG_DIR, dat->refId, &state);
            ((CheckBox *)widget)->SetChecked(0, state, 0);
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
    made = 
    makeWidget(widget->refId, 
    widget->isAdvanced ? widget->adata.idType : idTypes[widget->type], 
    widget->isAdvanced ? widget->adata.type : widgetTypes[widget->type], 
    (widget->hasParent == 0) ? main_plane : findWidgetByHash(getHashByID(widget->parentRefId)));


#ifdef DEBUG
    sceClibPrintf("Adding widget with settings:\nrefId: %s\nparentRefID: %s\nhasParent: %s\nisAdvanced: %s", widget->refId, widget->parentRefId, widget->hasParent ? "True" : "False", widget->isAdvanced ? "True" : "False");
#endif

    NULL_ERROR_FAIL(made);
    updateValues(made, widget, widget->isAdvanced ? UPDATE_COLOR | UPDATE_POSITION | UPDATE_SIZE : flags);
    setupValues(made, widget);
    if(widget->OnLoad != NULL) widget->OnLoad();
    return 0;
}

int openQuickMenu()
{
    SCE_DBG_LOG_INFO("REACHED INTERNAL FUNCTION");
    if(impose_plugin == NULL)
    {
        impose_plugin = Plugin::GetByName("impose_plugin");
        NULL_ERROR_FAIL(impose_plugin);
    }
    if(impose_pluginFirstWidget == NULL)
    {
        impose_pluginFirstWidget = impose_plugin->GetWidgetByNum(1);
        NULL_ERROR_FAIL(impose_pluginFirstWidget);
    }
    int (*openQM)();
    openQM = (int(*)()) *(int *)((int)impose_pluginFirstWidget + 0xc);
    openQM();
    return 0;
}

int displayWidgets()
{
    node *current = currentWidgets.head;
    while(current != NULL)
    {
        spawn(&current->widget, UPDATE_ALL);
        current = current->next;
    }
    
    return 0;
}
#endif