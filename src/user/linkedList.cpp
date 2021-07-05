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

    void update_node(widgetData widget)
    {
        node *tmp = head;
        while (tmp != NULL)
        {
            if(sce_paf_strcmp(tmp->widget.refId, widget.refId) == 0)
                break;
            tmp = tmp->next;
        }
        if(tmp == NULL)
        {
            return;
        }

        tmp->widget = widget;
    }

    void add_node(widgetData widget)
    {
        node *tmp = new node;
        tmp->widget = widget;
        tmp->next = NULL;

        if(head == NULL)
        {
            head = tmp;
        }
    }

    void remove_node(const char *refId)
    {
        node *prev = NULL;
        node *current = head;
        while (current != NULL)
        {

            if(sce_paf_strcmp(refId, current->widget.refId) == 0)
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
};
