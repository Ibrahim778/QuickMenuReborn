
#include "types.h"
#ifndef QM_REBORN_LL_HPP
#define QM_REBORN_LL_HPP
struct node
{
    widgetData widget;
    node *next;
};

class linked_list
{
public:
    node *head, *tail;
    linked_list();
    void printall();
    void update_node(widgetData *widget, int flags);
    void update_checkbox_status(const char *refID, CheckBoxState state);
    widgetData *get_node(const char *refID);
    void add_node(widgetData *widget);
    void remove_node(const char *tag);
};
#endif