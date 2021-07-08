#include "widgets.h"

struct node
{
    widgetData *widget;
    node *next;
};

class linked_list
{
public:
    node *head;
    linked_list()
    {
        head = NULL;
    }

    void print()
    {
        SCE_DBG_LOG_INFO("W:\n");
        node *tmp = head;
        while(tmp != NULL)
        {
            SCE_DBG_LOG_INFO("%s, ", tmp->widget->refId);
            tmp = tmp->next;
        }
        SCE_DBG_LOG_INFO("\n");
    }

    void update_node(widgetData *widget)
    {
        node *tmp = head;
        while (tmp != NULL)
        {
            if(sce_paf_strcmp(tmp->widget->refId, widget->refId) == 0)
                break;
            tmp = tmp->next;
        }
        if(tmp == NULL)
        {
            return;
        }

        tmp->widget = widget;
    }

    void add_node(widgetData *widget)
    {
        node *tmp = new node;
        tmp->widget = widget;
        tmp->next = NULL;

        if(head == NULL)
        {
            head = tmp;
        }
    }
    
    //Credit to CreepNT for this
    void remove_node(const char *tag) {
        //Check head isn't NULL
        if (head == NULL)
            return;
        
        //First, handle the case where we free the head
        if (sce_paf_strcmp(head->widget->refId, tag) == 0) {
            node* nodeToDelete = head;
            head = head->next;
            sce_paf_free(nodeToDelete);
            return;
        }

        //Bail out if the head is the only node
        if (head->next == NULL)
            return;

        //Else, try to locate node we're asked to remove
        node** pCurrentNodeNext = &head;   //This points to the current node's `next` field (or to pHead)
        while (1) {
            if (sce_paf_strcmp((*pCurrentNodeNext)->widget->refId, tag) == 0) //pCurrentNodeNext points to the pointer that points to the node we need to delete
                break;

            //If the next node's next is NULL, we reached the end of the list. Bail out.
            if ((*pCurrentNodeNext)->next == NULL)
                return;

            pCurrentNodeNext = &(*pCurrentNodeNext)->next;
        };
        node* nodeToDelete = *pCurrentNodeNext;
        *pCurrentNodeNext = (*pCurrentNodeNext)->next;
        sce_paf_free(nodeToDelete);
    }

    /*
    void remove_node(const char *refId)
    {  
        node *prev = NULL;
        node *current = head;
        while (current != NULL)
        {

            if(sce_paf_strcmp(refId, current->widget->refId) == 0)
            {
                break;
            }
            else
            {
                prev = current;
                current = current->next;
            }
        }
        
        if(current == NULL)
            return;

        if(current == head) 
        {
            head = NULL; 
        }
        else prev->next = current->next;

        delete current;
    }
    */
};
