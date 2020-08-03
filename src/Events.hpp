#ifndef _KOT_WINDOW_EVENTS_
#define _KOT_WINDOW_EVENTS_

#include <InputCodes.hpp>

enum class IEventType
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

struct IEventPointerMoved
{
    int PointerX, PointerY;
    int DeltaX, DeltaY;
};

struct IEventKey
{
    Keys Code;
    KeyAction Action;
};

struct IEventButton
{
    Buttons Code;
    ButtonAction Action;
    int PointerX, PointerY;
};

struct IEvent
{
    IEventType Type;

    union
    {
        char32_t            eChar;
        IEventKey           eKey;
        IEventButton        eButton;
        IEventPointerMoved  ePMoved;
    };    
};


#endif