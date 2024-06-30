#pragma once

#include <Arduino.h>

typedef union valuetype
{
    char *string;
    bool Bool;
    uint8_t u8;
    int8_t s8;
    uint16_t u16;
    int16_t s16;
    uint32_t u32;
    int32_t s32;
    uint64_t u64;
    int64_t s64;
    float f32;
    const char *stringconstant;
} value_t;

typedef enum
{
    PREVIOS_LINE,
    NEXT_LINE,
    UPDATE_LINE
} LINE_TARGET_TYPE;

typedef enum
{
    PREVIOS_SCREEN,
    NEXT_SCREEN,
    UPDATE_SCREEN
} MENU_TARGET_TYPE;

typedef enum
{
    NO_VALUE,
    NORMAL,
    SWITCH,
    BARGRAPH,
    NAVIGATION
} line_t;

typedef enum
{
    NO_TYPE,
    BOOL_t,
    U8_t,
    U16_t,
    U32_t
} line_value_t; 

struct MenuFlags
{
    bool menuActive_f : 1;
    bool menuUpdate_f : 1;
    bool lineSelected : 1;
    bool bar_chars_inited : 1;
    bool num_chars_inited : 1;
    bool backlight_enabled : 1;
};

/**
 *  @brief Struct display line parameters 
 */
struct MenuLine
{
    String line_text;  
    value_t *line_value; 
    line_value_t val_type;
    line_t line_type;
};

typedef struct MenuLine MenuLine_t;

// #ifdef __AVR__
typedef void (*pointf_t)();
typedef void (*obj_pointf_t)(void *);
// #else
// typedef std::function<void(void)> pointf_t;
// typedef std::function<void(void)> obj_pointf_t;
// #endif