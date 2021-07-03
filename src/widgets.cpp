
#ifndef WIDGETS_CPP_QM_REBORN
#define WIDGETS_CPP_QM_REBORN

#include "widgets.h"
#include "linkedList.cpp"
#include "event_handler.cpp"

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

SceUInt32 getHashByID(char *id)
{
    Resource::Element sinfo;
    Resource::Element searchRequest;

    searchRequest.id.Set(id);

    sinfo.hash = searchRequest.GetHashById(&searchRequest);
    return sinfo.hash;
}

Widget *makeWidget(char *refId, char *idType, char *type, Widget *parent)
{
    paf::Resource::Element winfo;
    paf::Resource::Element sinfo;
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
__attribute__((used))
int unregisterWidget(char *refId)
{
    currentWidgets.remove_node(refId);
    return 0;
}

char *registerWidget(widgetData data)
{
    //This is just a wrapper, it'll add the widgets to a linked list, widgets are made on demand when impose menu loads via another thread
    currentWidgets.add_node(data);
    return data.refId;
}

int setText(char *text, Widget *widget)
{
    WString wstr = makeWString((const char *)text);
    return widget->SetLabel(&wstr);
}

int spawn(node *node)
{
    Widget *made;
    made = makeWidget(node->widget.refId, (char *)idTypes[node->widget.type], (char *)widgetTypes[node->widget.type], (node->widget.parentRefId == NULL ? main_plane : findWidgetByHash(getHashByID(node->widget.parentRefId))));
    NULL_ERROR_FAIL(made);
    SceFVector4 pos = makeSceVector4(node->widget.pos);
    SceFVector4 size = makeSceVector4(node->widget.size);
    Widget::Color col = makeSceColor(node->widget.col);
    made->SetPosition(&pos);
    made->SetSize(&size);

    QMEventHandler *eh = new QMEventHandler();
    switch (node->widget.type)
    {
        case button:
        {
            eh->pUserData = sce_paf_private_malloc(sizeof(widgetData));
            sce_paf_private_memcpy(eh->pUserData, &node->widget, sizeof(widgetData));
            made->SetFilterColor(&col);
            setText(node->widget.data.ButtonData.label, made);
            made->RegisterEventCallback(ON_PRESS_EVENT_ID, eh, 0);
            break;
        }

        case check_box:
        {
            eh->pUserData = sce_paf_private_malloc(sizeof(widgetData));
            sce_paf_private_memcpy(eh->pUserData, &node->widget, sizeof(widgetData));
            made->SetFilterColor(&col);
            made->RegisterEventCallback(ON_PRESS_EVENT_ID, eh, 0);
            break;
        }
        
        case text:
        {
            made->SetFilterColor(&col);
            sceClibPrintf("got size = %.1f, %s bold\n", node->widget.data.TextData.fontSize, node->widget.data.TextData.isbold ? "is" : "is not");
            TRY(made->SetOption(Widget::Option::Text_Bold, 0,0,node->widget.data.TextData.isbold));
            setText(node->widget.data.TextData.label, made);
            break;
        }
        
        case plane:
        {
            made->SetFilterColor(&col);
            break;
        }

        default:
        {
            break;
        }
    }

    return 0;
}

int displayWidgets()
{
    node *current = currentWidgets.head;
    while(current != NULL)
    {
        spawn(current);
        current = current->next;
    }
    
    return 0;
}

#endif