#ifndef _KOT_WINDOW_EVENTS_
#define _KOT_WINDOW_EVENTS_

#include <InputCodes.hpp>

enum class WEventType
{
    NoEvent,        // Unknown Event / No Event

    WindowClosed,   // - No Event Structs
    WindowResized,  // -  ~ No X11 Support
    WindowMoved,    // -  ~ No X11 Support
    WindowExposed,  // - 

    PointerMoved,   // -
    PointerIn,      // -
    PointerOut,     // -

    KeyEvent,       // IEventKey
    ButtonEvent,    // IEventButton

    CharEvent,      // Character Input Event
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

struct WEvent
{
    WEventType Type;

    union
    {
        char32_t            eChar;
        WEventKey           eKey;
        WEventButton        eButton;
        WEventPointerMoved  ePMoved;
    };    
};


#endif