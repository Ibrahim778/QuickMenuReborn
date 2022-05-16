#include "main.h"
#include "linkedList.hpp"
#include "utils.hpp"
#include "widgets.hpp"
#include "event_handler.hpp"
#include "config_mgr.h"
#include <kernel/libkernel.h>
#include "common.hpp"
#include <taihen.h>

Widget *(*getImposeRoot)() = SCE_NULL;
Plugin *imposePlugin = SCE_NULL;
Plugin *QuickMenuRebornPlugin = SCE_NULL;
ScePVoid powerRoot = SCE_NULL;
Box *scrollBox;
Widget *exitButton;

WidgetList currentWidgets;
TexList currTextures;

tai_hook_ref_t getControlsHookRef;
SceUID getControlsHookID;

const char *widgetTemplateID[] = //THESE ARE IN ORDER TO MATCH WITH THE ENUM
{
    BUTTON_TEMPLATE_ID,
    CHECKBOX_TEMPLATE_ID,
    TEXT_TEMPLATE_ID,
    PLANE_TEMPLATE_ID,
    SLIDEBAR_TEMPLATE_ID,
    PROGRESSBAR_TOUCH_TEMPLATE_ID,
    BUSYINDICATOR_TEMPLATE_ID
};

static bool addedTask = false;

SceVoid CheckMenuCloseTask(ScePVoid dat)
{
    if(Utils::FindWidget(0xC0099932) == NULL) //Check if exit button has been deleted
    {
        OnQuickMenuClose();
        common::Utils::RemoveMainThreadTask(CheckMenuCloseTask, NULL);
        addedTask = false;
        return;
    }

}

SceVoid OnQuickMenuClose()
{
    print("Quick Menu has closed!\n");

    texNode *n = currTextures.head;
    while(n != NULL)
    {
        Utils::DeleteTexture(&n->texture);
        n->texture = SCE_NULL;
        n = n->next;
    }


    node *widget = currentWidgets.head;
    while(widget != NULL)
    {
        if(widget->widget.OnDelete != NULL)
            widget->widget.OnDelete(widget->widget.refId);
        widget = widget->next;
    }
}

SceVoid onPluginReady(Plugin *plugin)
{
    QuickMenuRebornPlugin = plugin;
}

int initWidgets()
{
    if(!QuickMenuRebornPlugin)
    {
        Framework::PluginInitParam piParam;

        piParam.pluginName = "quick_menu_reborn_plugin";
        piParam.resourcePath = "ur0:QuickMenuReborn/qmr_plugin.rco";
        piParam.scopeName = "__main__";

        piParam.pluginStartCB = onPluginReady;

        paf::Framework::LoadPlugin(&piParam);
        
        if(QuickMenuRebornPlugin == NULL) return -1;
    }

    //Credit to GrapheneCt
    //Get power manage plugin object
    Plugin *powerManagePlugin = Plugin::Find("power_manage_plugin");
    if(powerManagePlugin < 0 || powerManagePlugin == NULL) return -1;

    imposePlugin = Plugin::Find("impose_plugin");
    if(imposePlugin < 0 || imposePlugin == NULL) return -1;

    //Power manage plugin -> power manage root
    powerRoot = powerManagePlugin->GetInterface(1);
    if(powerRoot < 0 || powerRoot == NULL) return -1;

    //Power manage root -> impose root (some virtual function)
    getImposeRoot = (Widget *(*)()) *(int *)((int)powerRoot + 0x54);
    
    scrollBox = (Box *)Utils::FindWidget(SCROLL_VIEW_BOX_ID);
    if(scrollBox == SCE_NULL) return -1;

    exitButton = Utils::FindWidget(0xC0099932);
    if(exitButton == SCE_NULL) return -1;

    //We need to add a task to catch the time one of the widgets are deleted, means the menu was closed.
    if(!addedTask)
    {
        common::Utils::AddMainThreadTask(CheckMenuCloseTask, NULL);
        addedTask = true;
    }

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

    //Assigning user handlers here
    QMR::AssignHandlersToWidget(w, data); print("Assigned Handlers\n");

    //Assigning textures here
    if(sce_paf_strlen(data->textureId) > 0)
    {
        QMR::AssignTextureToWidget(w, data->textureId); 
        print("Assigned Texture\n");
    }
    if(sce_paf_strlen(data->textureBaseId) > 0)
    {
        QMR::AssignTextureBaseToWidget(w, data->textureBaseId);
        print("Assigned Texture Base\n");   
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

SceVoid QMR::AssignHandlersToWidget(Widget *w, widgetData *data)
{
    //Create an int array and a variable to keep track of size. Loop through callbacks in struct. If event id is not present in int array add it and assign callback with id.
    
    int idNum = 0; // Array Num
    int *ids = NULL; //Array pointer
    for (int i = 0; i < data->CallbackNum; i++)
    {
        bool isRegistered = false;
        for(int x = 0; x < idNum && !isRegistered; x++)
            isRegistered = ids[x] == data->Callbacks[i].id; //Is callback already in our idList?

        if(!isRegistered)
        {

            QMREventHandler *eventHandler = new QMREventHandler();
            eventHandler->pUserData = data;
            w->RegisterEventCallback(data->Callbacks[i].id, eventHandler, 0); 

            ids = (int *)sce_paf_realloc(ids, sizeof(int) * (i + 1));
            ids[i] = data->Callbacks->id;
            idNum ++;
        }
    }
    sce_paf_free(ids);
}

SceVoid QMR::AssignTextureToWidget(Widget *w, const char *refID)
{
    texNode *n = currTextures.GetNode(refID);
    if(n != NULL)
    {
        if(n->texture == NULL)
        {
            bool ret = Utils::CreateTextureFromFile(&n->texture, n->texturePath.data);
            if(!ret)
            {
                print("[Error] Cannot Create Texture! Utils::CreateTextureFromFile() -> 0x%X\n", ret);
                delete n->texture;
                n->texture = SCE_NULL;
                return;
            }
        }

        w->SetTexture(&n->texture, 0, 0);
    }
    else print("[Error] Texture: %s is not registered!\n", refID);
}

SceVoid QMR::AssignTextureBaseToWidget(Widget *w, const char *refID)
{
    texNode *n = currTextures.GetNode(refID);
    if(n != NULL)
    {
        if(n->texture == NULL)
        {
            bool ret = Utils::CreateTextureFromFile(&n->texture, n->texturePath.data);
            if(!ret)
            {
                print("[Error] Cannot Create Texture! Utils::CreateTextureFromFile() -> 0x%X\n", ret);
                delete n->texture;
                n->texture = SCE_NULL;
                return;
            }
        }

        w->SetTextureBase(&n->texture);
    }
    else print("[Error] Texture: %s is not registered!\n", refID);
}

SceInt32 QMR::UnregisterWidget(const char *id)
{
    currentWidgets.RemoveNode(id);
}

widgetData *QMR::RegisterWidget(widgetData *dat)
{
    return currentWidgets.AddNode(dat);
}

SceInt32 QMR::RegisterTexture(const char *id, const char *path)
{
    currTextures.AddNode(id, path);
    return 0;
}

SceInt32 QMR::UnregisterTexture(const char *id)
{
    texNode *n = currTextures.GetNode(id);
    if(n == NULL) return 0;

    if(widgetsDisplayed() && n->texture != NULL)
        return QMR_ERROR_RESOURCE_BUSY;
    
    QMR::UnregisterTexture(id);
    return 0;
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

    searchRequest.id = (refId);
    winfo.hash = winfo.GetHashById(&searchRequest);
    
    searchRequest.id = (styleInfo);
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

    searchRequest.id = (refId);
    winfo.hash = winfo.GetHashById(&searchRequest);
    
    sinfo.hash = styleHash;    
    
    Widget *newWidget = imposePlugin->CreateWidgetWithStyle(parent, type, &winfo, &sinfo);
    return newWidget;
}

Widget *QMR::MakeWidgetFromTemplate(const char *id, Widget *targetRoot)
{
    Plugin::TemplateInitParam tinit;
    Resource::Element e;
    e.id = (id);
    e.hash = e.GetHashById(&e);

    QuickMenuRebornPlugin->TemplateOpen(targetRoot, &e, &tinit);

    return targetRoot->GetChildByNum(targetRoot->childNum - 1);
}