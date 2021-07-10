#include "widgets.h"

struct node
{
    widgetData widget;
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
    #ifdef DEBUG
        SCE_DBG_LOG_INFO("W:\n");
        node *tmp = head;
        while(tmp != NULL)
        {
            SCE_DBG_LOG_INFO("%s, ", tmp->widget.refId);
            tmp = tmp->next;
        }
        SCE_DBG_LOG_INFO("\n");
    #endif
    }

    void update_node(widgetData *widget, int flags)
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
                sce_paf_strncpy(tmp->widget.data.ButtonData.label, widget->data.ButtonData.label, 0x100);
                break;
            }
            case text:
            {
                sce_paf_strncpy(tmp->widget.data.TextData.label, widget->data.ButtonData.label, 0x100);
                break;
            }
            default:
                break;
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
    }

    void add_node(widgetData *widget)
    {
        node *tmp = new node;
        sce_paf_memcpy(&tmp->widget, widget, sizeof(widgetData));
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
