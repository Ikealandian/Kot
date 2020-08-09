#ifndef _KOT_WINDOW_EVENTS_
#define _KOT_WINDOW_EVENTS_

#include <InputCodes.hpp>

enum class WEventType
{
    NoEvent,        // Unknown Event / No Event

    WindowClosed,   // - No Event Structs
    WindowExposed,  // - 
    WindowResized,  // WEventResized
    WindowMoved,    // WEventMoved

    PointerMoved,   // -
    PointerIn,      // -
    PointerOut,     // -

    KeyEvent,       // WEventKey
    ButtonEvent,    // WEventButton

    CharEvent,      // Character Input Event
};

struct WEventMoved
{
    int X, Y;
};

struct WEventResized
{
    int W, H;
};

struct WEventPointerMoved
{
    int PointerX, PointerY;
    int DeltaX, DeltaY;
};

struct WEventKey
{
    Keys Code;
    KeyAction Action;
};

struct WEventButton
{
    Buttons Code;
    ButtonAction Action;
    int PointerX, PointerY;
};

// Is Inefficient?
struct WEvent
{
    WEventType Type;

    union
    {
        char32_t            eChar;
        WEventKey           eKey;
        WEventButton        eButton;
        WEventPointerMoved  ePMoved;
        WEventMoved         eWMoved;
        WEventResized       eWResized;
    };    
};


#endif