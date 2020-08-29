#ifndef _KOT_WINDOW_EVENTS_
#define _KOT_WINDOW_EVENTS_

#include <InputCodes.hpp>

enum class WEventType
{
    NoEvent,        // Unknown Event / No Event

    WindowClosed,   // -
    WindowExposed,  // -

    WindowChanged,  // WEventChanged: Resized, Moved
        WindowResized = WindowChanged,
        WindowMoved = WindowChanged,

    WindowMinimized,      // -
    WindowMaximized,      // -

    PointerMoved,   // WEventPointerMoved
    PointerIn,      // -
    PointerOut,     // -

    KeyEvent,       // WEventKey
    ButtonEvent,    // WEventButton
    ScrollEvent,    // WEventScroll

    CharEvent,      // Character Input Event (char32_t)
};

struct WEventChanged
{
    int X, Y;
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

struct WEventScroll
{
    ScrollAxis Axis;
    float Delta;
    
    Scroll Direction;
    float AbsDelta;
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
        WEventScroll        eScroll;
        WEventButton        eButton;
        WEventPointerMoved  ePMoved;
        WEventChanged       eWChanged;
    };    
};


#endif