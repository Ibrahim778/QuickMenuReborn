#ifndef C_TYPES_H_QM_REBORN
#define C_TYPES_H_QM_REBORN
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
    char *label;
    float fontSize;
    int isbold;
} textData;

typedef struct 
{
    void (*onPress)(void);
    //Text to be displayed
    char *label;
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
    char *refId;
    char *parentRefId;
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

widgetColor makeWidgetColor(float r, float g, float b, float a);
vector4 makeWidgetVector4(float x, float y, float z, float w);

#ifdef __cplusplus
}
#endif

#endif