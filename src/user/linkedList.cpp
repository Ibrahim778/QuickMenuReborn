#include "linkedList.hpp"
#include "main.h"
#include "utils.hpp"
#include "widgets.hpp"

widgetData *WidgetList::GetNode(const char *refID)
{
    node *n = head;
    while(n != NULL)
    {
        if(sce_paf_strncmp(n->widget.refId, refID, sizeof(n->widget.refId)) == 0)
            return &n->widget;
        n = n->next;
    }

    return NULL;
}

WidgetList::WidgetList()
{
    head = NULL;
    tail = NULL;
}

void WidgetList::PrintAll()
{
#ifdef _DEBUG
    node *current = head;
    while (current != NULL)
    {
        print("%s, ", current->widget.refId);
        current = current->next;
    }
#endif
}

void WidgetList::UpdateNode(widgetData *widget, int flags)
{
    node *tmp = head;
    bool found = false;
    while (tmp != NULL)
    {
        if (sce_paf_strcmp(tmp->widget.refId, widget->refId) == 0)
        {
            found = true;
            break;
        }
        tmp = tmp->next;
    }

    if(!found)
    {
        print("Error widget not present in list. Abort!\n");
    }
    else
    {
        sce_paf_memcpy(&tmp->widget, widget, sizeof(widgetData));
    }
}

widgetData *WidgetList::AddNode(widgetData *widget)
{
    node *tmp = new node;
    sce_paf_memcpy(&tmp->widget, widget, sizeof(widgetData));
    tmp->next = NULL;

    if (head == NULL)
    {
        head = tmp;
        tail = tmp;
    }
    else
    {
        tail->next = tmp;
        tail = tail->next;
    }

    return &tmp->widget;
}

//Credit to CreepNT for this
void WidgetList::RemoveNode(const char *tag)
{
    //Check head isn't NULL
    if (head == NULL)
        return;

    //First, handle the case where we free the head
    if (sce_paf_strcmp(head->widget.refId, tag) == 0)
    {
        node *nodeToDelete = head;
        head = head->next;
        sce_paf_free(nodeToDelete);
        return;
    }

    //Bail out if the head is the only node
    if (head->next == NULL)
        return;

    //Else, try to locate node we're asked to remove
    node **pCurrentNodeNext = &head; //This points to the current node's `next` field (or to pHead)
    while (1)
    {
        if (sce_paf_strcmp((*pCurrentNodeNext)->widget.refId, tag) == 0) //pCurrentNodeNext points to the pointer that points to the node we need to delete
            break;

        //If the next node's next is NULL, we reached the end of the list. Bail out.
        if ((*pCurrentNodeNext)->next == NULL)
            return;

        pCurrentNodeNext = &(*pCurrentNodeNext)->next;
    };
    node *nodeToDelete = *pCurrentNodeNext;
    *pCurrentNodeNext = (*pCurrentNodeNext)->next;
    sce_paf_free(nodeToDelete->widget.Callbacks);
    sce_paf_free(nodeToDelete);
}

TexList::TexList()
{
    head = NULL;
    tail = NULL;
}

texNode *TexList::GetNode(const char *refID)
{
    texNode *n = head;
    long len = sce_paf_strlen(refID);
    while(n != NULL)
    {
        if(sce_paf_strncmp(n->refID.data, refID, len) == 0)
            return n;
        
        n = n->next;
    }
    
    return NULL;
}

void TexList::AddNode(const char *refID, const char *tex)
{
    texNode *tmp = new texNode;
    tmp->refID = refID;
    tmp->texturePath = tex;

    tmp->texture = NULL;

    tmp->next = NULL;
    if(head == NULL)
    {
        head = tmp;
        tail = tmp;
    }
    else
    {
        tail->next = tmp;
        tail = tail->next;
    }
}

void TexList::RemoveNode(const char *refID)
{
    //Check head isn't NULL
    if (head == NULL)
        return;

    //First, handle the case where we free the head
    if (sce_paf_strcmp(head->refID.data, refID) == 0)
    {
        texNode *nodeToDelete = head;
        head = head->next;
        sce_paf_free(nodeToDelete);
        return;
    }

    //Bail out if the head is the only node
    if (head->next == NULL)
        return;

    //Else, try to locate node we're asked to remove
    texNode **pCurrentNodeNext = &head; //This points to the current node's `next` field (or to pHead)
    while (1)
    {
        if (sce_paf_strcmp((*pCurrentNodeNext)->refID.data, refID) == 0) //pCurrentNodeNext points to the pointer that points to the node we need to delete
            break;

        //If the next node's next is NULL, we reached the end of the list. Bail out.
        if ((*pCurrentNodeNext)->next == NULL)
            return;

        pCurrentNodeNext = &(*pCurrentNodeNext)->next;
    };
    texNode *nodeToDelete = *pCurrentNodeNext;
    *pCurrentNodeNext = (*pCurrentNodeNext)->next;

    if(nodeToDelete->texture != NULL)
    {
        if(nodeToDelete->texture->texSurface != NULL)
        {
            graphics::Surface *s = nodeToDelete->texture->texSurface;
            nodeToDelete->texture->texSurface = NULL;
            delete s;
        }

        delete nodeToDelete->texture;
    }

    sce_paf_free(nodeToDelete);   
}