#include "linkedList.hpp"
#include "main.h"
#include "types.h"
#include "widgets.h"
#ifndef QM_REBORN_LL_CPP
#define QM_REBORN_LL_CPP

    linked_list::linked_list()
    {
        head = NULL;
        tail = NULL;
    }

    void linked_list::printall()
    {
    #ifdef DEBUG
        node *current = head;
        while (current != NULL)
        {
            print("%s, ", current->widget.refId);
            current = current->next;
        }
        
    #endif
    }

    void linked_list::update_checkbox_status(const char *refID, CheckBoxState state)
    {
        node *tmp = head;
        while(tmp != NULL)
        {
            if(sce_paf_strcmp(tmp->widget.refId, refID) == 0)
                break;
            tmp = tmp->next;
        }

        if(tmp == NULL)
            return;

        tmp->widget.data.CheckBoxData.state = state;
    }

    void linked_list::update_node(widgetData *widget, int flags)
    {
        node *tmp = head;
        while (tmp != NULL)
        {
            if(sce_paf_strcmp(tmp->widget.refId, widget->refId) == 0)
                break;
            tmp = tmp->next;
        }
        if(tmp == NULL)
        {
            print("WARNING TMP == NULL CANNOT UPDATE\n");
            return;
        }

        if(flags & UPDATE_COLOR)
            tmp->widget.col = widget->col;
        if(flags & UPDATE_SIZE)
            tmp->widget.size = widget->size;
        if(flags & UPDATE_POSITION)
            tmp->widget.pos = widget->pos;
        if(flags & UPDATE_TEXT)
        {
            switch (widget->type)
            {
            case button:
            {
                sce_paf_memcpy(tmp->widget.data.ButtonData.label, widget->data.ButtonData.label, sizeof(tmp->widget.data.ButtonData.label));
                print("After copying text %s\n", tmp->widget.data.ButtonData.label);
                break;
            }
            case text:
            {
                sce_paf_strncpy(tmp->widget.data.TextData.label, widget->data.ButtonData.label, sizeof(tmp->widget.data.ButtonData.label));
                break;
            }
            default:
                break;
            }
        }

        if(flags & UPDATE_CHECKBOX_STATE)
        {
            if(tmp->widget.type == check_box)
            {
                tmp->widget.data.CheckBoxData.state = widget->data.CheckBoxData.state;
            }
        }

        if(flags & UPDATE_EVENT)
        {
            switch (widget->type)
            {
            case button:
            {
                tmp->widget.data.ButtonData.onPress = widget->data.ButtonData.onPress;
                break;
            }
            
            case check_box:
            {
                tmp->widget.data.CheckBoxData.OnToggle = widget->data.CheckBoxData.OnToggle;
                break;
            }

            default:
                break;
            }
        }

        if(flags & UPDATE_LOAD)
            tmp->widget.OnLoad = widget->OnLoad;
    }

    void linked_list::add_node(widgetData *widget)
    {
        node *tmp = new node;
        sce_paf_memcpy(&tmp->widget, widget, sizeof(widgetData));
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
    
    //Credit to CreepNT for this
    void linked_list::remove_node(const char *tag) {
        //Check head isn't NULL
        if (head == NULL)
            return;
        
        //First, handle the case where we free the head
        if (sce_paf_strcmp(head->widget.refId, tag) == 0) {
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
            if (sce_paf_strcmp((*pCurrentNodeNext)->widget.refId, tag) == 0) //pCurrentNodeNext points to the pointer that points to the node we need to delete
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

#endif