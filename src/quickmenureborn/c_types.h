#ifndef C_TYPES_H_QM_REBORN
#define C_TYPES_H_QM_REBORN

#ifdef QM_REBORN_INTERNAL
#ifdef __SNC__
#include <kernel/types.h>
#else
#include <psp2/types.h>
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    button = 0,
    check_box,
    text,
    plane,
    slidebar,
} widget_type;

typedef enum check_box_state
{
    //ON
    CHECKBOX_ON,
    //OFF
    CHECKBOX_OFF,
    //Previous saved state
    CHECKBOX_PREV_STATE
} CheckBoxState;


typedef struct 
{
    //Text to be displayed
    char label[0x100];
    int isbold;
} textData;

typedef struct 
{
    void (*onPress)(void);
    //Text to be displayed
    char label[0x100];
} buttonData;

typedef struct 
{
    //Function called when toggled
    void (*OnToggle)(int state);
    //Current State
    CheckBoxState state;
} toggleData;

typedef struct
{
    void (*OnChange)();
} slidebarData;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} widgetColor;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vector4;

typedef struct
{
    char type[256];
    char styleInfo[256];
    int useHash;
    int hash;
} advancedData;

typedef struct
{
    char refId[256];

    char parentRefId[256];
    int hasParent;
    
    widget_type type;

    vector4 pos;
    widgetColor col;
    vector4 size;
    
    void (*OnLoad)(void);

    union extraData
    {
        toggleData CheckBoxData;
        buttonData ButtonData;
        textData TextData;
        slidebarData SlidebarData;
    } data;

    //BOOL: Set weather the advanced data is used
    int isAdvanced;

    //Don't mess with, unless you know what you're doing
    advancedData adata;
    
} widgetData;

typedef enum packet_type
{
    register_widget,
    unregister_widget,
    update_widget,
    open_quickmenu

} packetType;

widgetColor makeWidgetColor(float r, float g, float b, float a);
vector4 makeWidgetVector4(float x, float y, float z, float w);

#define UPDATE_COLOR 0x1
#define UPDATE_SIZE 0x2
#define UPDATE_POSITION 0x4
#define UPDATE_TEXT 0x8
// Corresponds to the event called on press (OnPress for buttons and OnToggle for Check Boxes)
#define UPDATE_EVENT 0x10
#define UPDATE_ALL 0xFF
// Corresponds to the OnLoad event for all widgets
#define UPDATE_LOAD 0x20
#define UPDATE_CHECKBOX_STATE 0x40

#ifdef __cplusplus
}
#endif

#endif