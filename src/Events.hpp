#ifndef _WINDOW_EVENTS_
#define _WINDOW_EVENTS_

#include <InputCodes.hpp>

enum class IEventType
{
    NoEvent,        // Unknown Event / No Event

    WindowClosed,   // - No Event Struct
    WindowResized,  // -  ~ No X11 Support
    WindowMoved,    // -  ~ No X11 Support
    WindowExposed,  // - 

    PointerMoved,   //
    PointerIn,      //
    PointerOut,     //

    KeyEvent,       // IEventKey
    ButtonEvent,    // IEventButton

    CharEvent,      // Character Input Event
};

struct IEventPMoved
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
        char32_t        eChar;
        IEventKey       eKey;
        IEventButton    eButton;
        IEventPMoved    ePMoved;
    };    
};


#endif