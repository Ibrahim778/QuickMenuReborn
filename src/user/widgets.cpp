#include "main.h"
#include "linkedList.hpp"
#include "utils.hpp"
#include "widgets.hpp"
#include "event_handler.hpp"
#include "config_mgr.h"
#include <kernel/libkernel.h>
#include "common.hpp"

Widget *(*getImposeRoot)() = SCE_NULL;
Plugin *imposePlugin = SCE_NULL;
Plugin *QuickMenuRebornPlugin = SCE_NULL;
Widget *powerRoot = SCE_NULL;
Box *scrollBox;

LinkedList currentWidgets;

const char *widgetTemplateID[] = 
{
    BUTTON_TEMPLATE_ID,
    CHECKBOX_TEMPLATE_ID,
    TEXT_TEMPLATE_ID,
    PLANE_TEMPLATE_ID,
    SLIDEBAR_TEMPLATE_ID,
    PROGRESSBAR_TOUCH_TEMPLATE_ID,
};

SceVoid onPluginReady(Plugin *plugin)
{
    if(plugin == SCE_NULL) print("Error Loading Plugin!\n");
    else print("Loaded Plugin Successfully!\n");
    QuickMenuRebornPlugin = plugin;
}

int initWidgets()
{
    if(!QuickMenuRebornPlugin)
    {
        Framework::PluginInitParam piParam;

        piParam.pluginName.Set("quick_menu_reborn_plugin");
        piParam.resourcePath.Set("ur0:QuickMenuReborn/qmr_plugin.rco");
        piParam.scopeName.Set("__main__");

        piParam.pluginStartCB = onPluginReady;

        paf::Framework::LoadPlugin(&piParam);
    }

    //Credit to GrapheneCT
    //Get power manage plugin object
    Plugin *powerManagePlugin = Plugin::Find("power_manage_plugin");
    if(powerManagePlugin < 0 || powerManagePlugin == NULL) return -1;

    imposePlugin = Plugin::Find("impose_plugin");
    if(imposePlugin < 0 || imposePlugin == NULL) return -1;

    //Power manage plugin -> power manage root
    powerRoot = powerManagePlugin->GetInterface(1);
    if(powerRoot < 0 || powerRoot == NULL) return -1;

    //Power manage root -> impose root (some virtual function)
    getImposeRoot = (widget::Widget *(*)()) *(int *)((int)powerRoot + 0x54);
    
    scrollBox = (Box *)Utils::FindWidget(SCROLL_VIEW_BOX_ID);
    if(scrollBox == SCE_NULL) return -1;
    return 0;
}

int summon(widgetData *data)
{
    print("Making: %s\n", data->refId);
    Widget *w;
    if(data->isAdvanced)
    {
        if(data->advancedData.useHash)
            w = QMR::MakeAdvancedWidgetWithHash(data->refId, data->advancedData.StyleInfo.hash, data->advancedData.type, data->hasParent ? Utils::FindWidget(Utils::GetHashById(data->parentRefId)) : scrollBox);
        else w = QMR::MakeAdvancedWidgetWithID(data->refId, data->advancedData.StyleInfo.styleID, data->advancedData.type, data->hasParent ? Utils::FindWidget(Utils::GetHashById(data->parentRefId)) : scrollBox);
    }   
    else w = QMR::MakeWidgetFromTemplate(widgetTemplateID[data->type], data->hasParent ? Utils::FindWidget(Utils::GetHashById(data->parentRefId)) : scrollBox);
    if(w == NULL)
    {
        print("Error making widget with id: %s\n", data->refId);
        return;
    }
    else print("Made widget %s (0x%X) successfully!\n", data->refId, Utils::GetHashById(data->refId));

    data->widget = w;

    w->hash = Utils::GetHashById(data->refId); //VERY IMPORTANT
    data->hash = w->hash;

    Utils::SetWidgetPosition(w, &data->pos); print("Assigned position\n");
    Utils::SetWidgetColor(w, &data->col); print("Assigned Color\n");
    Utils::SetWidgetSize(w, &data->size); print("Assigned Size\n");
    Utils::SetWidgetLabel(data->label, w); print("Assigned Label\n");

    //Create an int array and a variable to keep track of size. Loop through callbacks in struct. If event id is not present in int array add it and assign callback with id.
    {
        int idNum = 0; // Array Num
        int *ids = NULL; //Array pointer
        for (int  i = 0; i < data->CallbackNum; i++)
        {
            bool isRegistered = false;
            for(int x = 0; x < idNum && !isRegistered; x++)
                isRegistered = ids[x] == data->Callbacks[i].id; //Is callback already in our idList?

            if(!isRegistered)
            {

                QMREventHandler *eventHandler = new QMREventHandler();
                eventHandler->pUserData = sce_paf_malloc(sizeof(widgetData *));
                *(widgetData **)eventHandler->pUserData = data;
                w->RegisterEventCallback(data->Callbacks[i].id, eventHandler, 0); 

                ids = (int *)sce_paf_realloc(ids, sizeof(int) * (i + 1));
                ids[i] = data->Callbacks->id;
                idNum ++;
            }
        }
        sce_paf_free(ids);
    }

    if(data->OnRecall != NULL)
        data->OnRecall(data->refId);
    
    if(data->OnLoad != NULL)
        data->OnLoad(data->refId); 

    return;
}

int displayWidgets()
{
    node *n = currentWidgets.head;
    while(n != NULL)
    {
        summon(&n->widget);
        n = n->next;
    }
}

SceInt32 QMR::UnregisterWidget(const char *id)
{
    currentWidgets.RemoveNode(id);
}

widgetData *QMR::RegisterWidget(widgetData *dat)
{
    return currentWidgets.AddNode(dat);
}

SceInt32 QMR::RegisterEventHandler(SceInt32 id, widgetData *dat, ECallback Function, void *userDat)
{
    if(dat == NULL || Function == NULL) 
    {
        print("Error Invalid Arg\n");
        return -2;
    }
    dat->Callbacks = (CallbackData *)sce_paf_realloc(dat->Callbacks, (dat->CallbackNum + 1) * sizeof(CallbackData)); 
    if(dat->Callbacks == NULL) 
    {
        print("Error allocating mem\n");
        return -1;
    } 

    dat->Callbacks[dat->CallbackNum].function = Function;
    dat->Callbacks[dat->CallbackNum].id = id;
    dat->Callbacks[dat->CallbackNum].userDat = userDat;
    
    dat->CallbackNum++;

    print("%s: Assigned callback for function: %p id: 0x%X\n", dat->refId, Function, id);
    return 0;
}

Widget *QMR::MakeAdvancedWidgetWithID(const char *refId, const char *styleInfo, const char *type, Widget *parent)
{
    //WidgetInfo
    paf::Resource::Element winfo;
    //SearchInfo
    paf::Resource::Element sinfo;
    //Search Request
    paf::Resource::Element searchRequest;

    searchRequest.id.Set(refId);
    winfo.hash = winfo.GetHashById(&searchRequest);
    
    searchRequest.id.Set(styleInfo);
    sinfo.hash = sinfo.GetHashById(&searchRequest);    

    Widget *newWidget = imposePlugin->CreateWidgetWithStyle(parent, type, &winfo, &sinfo);
    
    return newWidget;
}

Widget *QMR::MakeAdvancedWidgetWithHash(const char *refId, int styleHash, const char *type, Widget *parent)
{
    //WidgetInfo
    paf::Resource::Element winfo;
    //StyleInfo
    paf::Resource::Element sinfo;
    //Search Request
    paf::Resource::Element searchRequest;

    searchRequest.id.Set(refId);
    winfo.hash = winfo.GetHashById(&searchRequest);
    
    sinfo.hash = styleHash;    
    
    Widget *newWidget = imposePlugin->CreateWidgetWithStyle(parent, type, &winfo, &sinfo);
    return newWidget;
}

Widget *QMR::MakeWidgetFromTemplate(const char *id, Widget *targetRoot)
{
    Plugin::TemplateInitParam tinit;
    Resource::Element e;
    e.id.Set(id);
    e.hash = e.GetHashById(&e);

    QuickMenuRebornPlugin->AddWidgetFromTemplate(targetRoot, &e, &tinit);

    return targetRoot->GetChildByNum(targetRoot->childNum - 1);
}