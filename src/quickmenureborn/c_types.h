#ifndef C_TYPES_H_QM_REBORN
#define C_TYPES_H_QM_REBORN

#ifdef QM_REBORN_INTERNAL
#define EXPORT_PIPE "quickmenureborn_exports_pipe"
#endif

#ifdef __cplusplus

extern "C" {

#endif

typedef enum
{
    button = 0,
    check_box,
    text,
    plane
} widget_type;

typedef struct 
{
    //Text to be displayed
    const char *label;
    int isbold;
} textData;

typedef struct 
{
    void (*onPress)(void);
    //Text to be displayed
    const char *label;
} buttonData;

typedef struct 
{
    //Function called when toggled
    void (*OnToggle)(int state);
} toggleData;

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
    const char *refId;
    const char *parentRefId;
    widget_type type;

    vector4 pos;
    widgetColor col;
    vector4 size;
    union
    {
        toggleData CheckBoxData;
        buttonData ButtonData;
        textData TextData;
    } data;
    
} widgetData;

typedef enum packet_type
{
    register_widget,
    unregister_widget,
    update_widget

} packetType;

typedef struct 
{
    //Function to pass to
    packetType type;
    //Data to be passed to the functions
    widgetData data;
    //Only check if type == update_widget
    int updateFlags;
    //Only used for unreigistering widget, otherwise not set
    char refId[256];
} exportPacket;

widgetColor makeWidgetColor(float r, float g, float b, float a);
vector4 makeWidgetVector4(float x, float y, float z, float w);

#define UPDATE_COLOR 0x1
#define UPDATE_SIZE 0x2
#define UPDATE_POSITION 0x4
#define UPDATE_TEXT 0x8
#define UPDATE_EVENT 0x10
#define UPDATE_ALL 0xFF

#ifdef __cplusplus
}
#endif

#endif