#include "widgets.h"

struct node
{
    widgetData widget;
    node *next;
};

class linked_list
{
private:
    node *tail;

public:
    node *head;
    linked_list()
    {
        head = NULL;
        tail = NULL;
    }

    void add_node(widgetData widget)
    {
        node *tmp = new node;
        tmp->widget = widget;
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

    void remove_node(char *refId)
    {
        node *prev;
        node *current = head;
        while (current != NULL)
        {

            if(!sceClibStrcmp(refId, current->widget.refId))
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
        else
        {
            if(current == head) head = NULL; 
            else prev->next = current->next;
            delete current;
        }
    }
};
