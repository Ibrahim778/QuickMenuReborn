#include "linkedList.hpp"
#include "main.h"
#include "utils.hpp"
#include "widgets.hpp"

widgetData *LinkedList::GetNode(const char *refID)
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

LinkedList::LinkedList()
{
    head = NULL;
    tail = NULL;
}

void LinkedList::PrintAll()
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

void LinkedList::UpdateNode(widgetData *widget, int flags)
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
}

widgetData *LinkedList::AddNode(widgetData *widget)
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
void LinkedList::RemoveNode(const char *tag)
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