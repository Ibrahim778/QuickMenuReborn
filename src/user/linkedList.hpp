
#ifndef QM_REBORN_LL_HPP
#define QM_REBORN_LL_HPP
#include "utils.hpp"
struct node
{
    widgetData widget;
    node *next;
};

class LinkedList
{
public:
    node *head, *tail;
    LinkedList();
    void PrintAll();
    void UpdateNode(widgetData *widget, int flags);
    widgetData *GetNode(const char *refID);
    widgetData *AddNode(widgetData *widget);
    void RemoveNode(const char *tag);
};
#endif