#ifndef _KOT_WINDOW_EVENTS_
#define _KOT_WINDOW_EVENTS_

#include <InputCodes.hpp>

enum class WEventType
{
    NoEvent,        // Unknown Event / No Event

    WindowCreated,  // -

    WindowClosed,   // -
    WindowExposed,  // -

    WindowChanged,  // WEventChanged: Resized, Moved
        WindowResized = WindowChanged,
        WindowMoved = WindowChanged,

    WindowMinimized,// -
    WindowMaximized,// -

    PointerMoved,   // WEventPointerMoved
    PointerIn,      // -
    PointerOut,     // -

    KeyEvent,       // WEventKey
    ButtonEvent,    // WEventButton
    ScrollEvent,    // WEventScroll

    CharEvent,      // Character Input Event (char32_t)
};

struct WEventWindowChanged
{
    long X, Y;
    long Width, Height;
};

struct WEventPointerMoved
{
    long PointerX, PointerY;
    long DeltaX, DeltaY;
};

struct WEventKey
{
    Keys Code;
    KeyAction Action;
};

struct WEventScroll
{
    long PointerX, PointerY;

    ScrollAxis Axis;
    float Delta;
    
    Scroll Direction;
    float AbsDelta;
};

struct WEventButton
{
    Buttons Code;
    ButtonAction Action;
    long PointerX, PointerY;
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
        WEventScroll        eScroll;
        WEventPointerMoved  ePMoved;
        WEventWindowChanged eWChanged;
    };    
};


#endif