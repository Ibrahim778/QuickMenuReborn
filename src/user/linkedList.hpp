
#ifndef QM_REBORN_LL_HPP
#define QM_REBORN_LL_HPP
#include "utils.hpp"
struct node
{
    widgetData widget;
    node *next;
};

class WidgetList
{
public:
    node *head, *tail;
    WidgetList();
    void PrintAll();
    void UpdateNode(widgetData *widget, int flags);
    widgetData *GetNode(const char *refID);
    widgetData *AddNode(widgetData *widget);
    void RemoveNode(const char *tag);
};

struct texNode
{
    paf::String refID;
    paf::String texturePath;

    graphics::Texture *texture;

    texNode *next;
};

class TexList
{
public:
    texNode *head, *tail;
    TexList();
    texNode *GetNode(const char *refID);
    void AddNode(const char *refID, const char *texturePath);
    void RemoveNode(const char *refID);
};
#endif