#include "X11Impl.hpp"
#include "X11Window.hpp"
#include "X11Input.hpp"

#include <cmath>

void IX11Window::CreateWindow()
{
    // Open Display
    _wImpl->xDisplay = XOpenDisplay(nullptr);
    if (_wImpl->xDisplay == nullptr)
    {
        fputs("IWindow [IX11Window]: XOpenDisplay Failed...", stderr);
        return;
    }
    
    // Get ScreenID and Root Window
    _wImpl->iScreen = DefaultScreen(_wImpl->xDisplay);
    _wImpl->xRoot = RootWindow(_wImpl->xDisplay, _wImpl->iScreen);

    // Get Display SIze
    DisplayScreen* DpyScr = &_wImpl->dScreen;
    DpyScr->Width = XDisplayWidth(_wImpl->xDisplay, _wImpl->iScreen);
    DpyScr->Height = XDisplayHeight(_wImpl->xDisplay, _wImpl->iScreen);

    // Impl Attributes
    IWindow::Attributes* Attribs = &_wImpl->wAttribs;

    // Create Window
    _wImpl->xWindow = XCreateSimpleWindow(
        _wImpl->xDisplay, _wImpl->xRoot,        // Display, Parent Window
        Attribs->X,     Attribs->Y,             // X, Y
        Attribs->Width, Attribs->Height,        // W, H
        0, 0,                                   // Border Size, Border Colour
        0x0                                     // Background Colour
    );

    XStoreName(_wImpl->xDisplay, _wImpl->xWindow, Attribs->Title);
    XSetIconName(_wImpl->xDisplay, _wImpl->xWindow, Attribs->Title);

    // Atoms
    _wImpl->WMState = XInternAtom(_wImpl->xDisplay, "WM_STATE", False);
    _wImpl->WMNetState = XInternAtom(_wImpl->xDisplay, "_NET_WM_STATE", False);
	_wImpl->WMNetStateFullscreen = XInternAtom(_wImpl->xDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    _wImpl->WMDeleteWindow = XInternAtom(_wImpl->xDisplay, "WM_DELETE_WINDOW", False);
    _wImpl->WMNetMaximizedVertical = XInternAtom(_wImpl->xDisplay, "_NET_WM_STATE_MAXIMIZED_VERT", False);
    _wImpl->WMNetMaximizedHorizontal = XInternAtom(_wImpl->xDisplay, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
    _wImpl->WMNetStateHidden = XInternAtom(_wImpl->xDisplay, "_NET_WM_STATE_HIDDEN", False);
    _wImpl->WMNetActionMinimise = XInternAtom(_wImpl->xDisplay, "_NET_WM_ACTION_MINIMIZE", False);

    // Override Protocols
    std::vector<Atom> WMProtocols = 
    {
        _wImpl->WMDeleteWindow // Window Close Message
    };
    XSetWMProtocols(_wImpl->xDisplay, _wImpl->xWindow, WMProtocols.data(), WMProtocols.size());

    // Map Inputs
    // Possible Memory Leaks Associated
    X11Input::Init(&_wImpl->xDisplay, &_wImpl->xWindow);

    // Map
    XSelectInput(
        _wImpl->xDisplay, _wImpl->xWindow,
        ExposureMask                |
        KeyPressMask                |
        KeyReleaseMask              |
        StructureNotifyMask         |
        PointerMotionMask           |
        LeaveWindowMask             |
        EnterWindowMask             |
        ButtonPressMask             |
        ButtonReleaseMask           |
        ButtonMotionMask            |
        PropertyChangeMask
    );

    XMapWindow(_wImpl->xDisplay, _wImpl->xWindow);

    // Create Cursor
    _wImpl->xCursor = XCreateFontCursor(_wImpl->xDisplay, XC_arrow);

    // Flags

    // Maximize Window
    if (Attribs->aFlags & IWindow::Flags::WindowMaximized)
    {
        XEvent Event = {};

        Event.type                  = ClientMessage;
        Event.xclient.serial        = 0;
        Event.xclient.send_event    = True;
        Event.xclient.window        = _wImpl->xWindow;
        Event.xclient.message_type  = _wImpl->WMNetState;
        Event.xclient.format        = 32;
        Event.xclient.data.l[0]     = 1;
        Event.xclient.data.l[1]     = _wImpl->WMNetMaximizedHorizontal;
        Event.xclient.data.l[2]     = _wImpl->WMNetMaximizedVertical;

        XSendEvent(_wImpl->xDisplay, _wImpl->xRoot, False, SubstructureNotifyMask, &Event);
    }

    // Fullscreen Window
    if (Attribs->aFlags & IWindow::Flags::WindowFullScreen)
    {
        XEvent Event = {};

		Event.xclient.type			= ClientMessage;
		Event.xclient.serial		= 0;
		Event.xclient.send_event	= True;
		Event.xclient.window		= _wImpl->xWindow;
		Event.xclient.message_type  = _wImpl->WMNetState;
		Event.xclient.format        = 32;
		Event.xclient.data.l[0]     = 1;
		Event.xclient.data.l[1]     = _wImpl->WMNetStateFullscreen;
		Event.xclient.data.l[2]     = 0;

		XSendEvent(_wImpl->xDisplay, _wImpl->xRoot, False, SubstructureRedirectMask | SubstructureNotifyMask, &Event);
    }

    // Window Centered
    if (Attribs->aFlags & IWindow::Flags::PositionCentered)
    {
        // *Centers All Screens
        //  I think

        Attribs->X = (DpyScr->Width / 2)  - (Attribs->Width / 2); 
        Attribs->Y = (DpyScr->Height / 2) - (Attribs->Height / 2);

        // Set Window Position
        XMoveWindow(
            _wImpl->xDisplay, _wImpl->xWindow,
            Attribs->X, Attribs->Y
        );
    }

    // No Window Resizing
    if (Attribs->aFlags & IWindow::Flags::NoResizing)
    {
        XSizeHints* XHint;
        XHint = XAllocSizeHints();
        XHint->flags = PMinSize | PMaxSize;
        
        XWindowAttributes XWinAttribs;
        XGetWindowAttributes(_wImpl->xDisplay,_wImpl->xWindow, &XWinAttribs);

        XHint->min_width = XWinAttribs.width;
        XHint->max_width = XWinAttribs.width;
        XHint->min_height = XWinAttribs.height;
        XHint->max_height = XWinAttribs.height;

        XSetWMNormalHints(_wImpl->xDisplay, _wImpl->xWindow, XHint);
        XFree(XHint);
    }

    _wImpl->eStack.PushWindowCreatedEvent();
}

void IX11Window::DestroyWindow()
{
    XDestroyWindow(_wImpl->xDisplay, _wImpl->xWindow);
    XCloseDisplay(_wImpl->xDisplay);
}

IX11Window::IX11Window(const IWindow::Attributes& _Attributes)
{
    _wImpl = new __IWinImpl;
    _wImpl->wAttribs = _Attributes;
    CreateWindow();
}

IX11Window::~IX11Window()
{
    DestroyWindow();
    delete _wImpl;
}

unsigned IX11Window::GetPlatform() const
{
    // Linux Or Mach
    // TODO: Find Platform
    return IWindow::Platform::Linux | IWindow::Platform::Mach;
}

DisplayScreen* IX11Window::GetDisplayData() const
{
    return &_wImpl->dScreen;
}

void IX11Window::SetCursorMode(const CursorMode& _Cursor)
{
    switch (_Cursor)
    {
    case CursorMode::Free:
    {
        XUngrabPointer(_wImpl->xDisplay, CurrentTime);
        break;
    }
    case CursorMode::Confined:
    {
        // Returning:
        //  GrabNotViewable
        printf("grab pointer result: %d\n",
            XGrabPointer(
                _wImpl->xDisplay, _wImpl->xWindow,
                False, PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
                GrabModeAsync, GrabModeAsync, _wImpl->xWindow, _wImpl->xCursor, CurrentTime
            )
        );

        break;
    }
    case CursorMode::Locked:
        break;
    default: break;
    }
}

void IX11Window::SetCursorState(const CursorState& _Cursor)
{
    switch (_Cursor)
    {
    default: break;
    }
}
    
void* IX11Window::SetNativeData(void* _Data) { return _Data; }
void* IX11Window::GetNativeData() const      { return nullptr; }

WEvent IX11Window::Event() const
{
    return _wImpl->eStack.PopEvent();
}

bool IX11Window::IsEvent() const
{
    return _wImpl->eStack.IsEventStackNotEmpty();
}

void IX11Window::Update()
{
    // Lock Cursor
    _wImpl->eStack.ClearStack();

    while (XPending(_wImpl->xDisplay))
    {
        XNextEvent(_wImpl->xDisplay, &_wImpl->xEvent);

        XEvent* Event = &_wImpl->xEvent;
        int Type = _wImpl->xEvent.type;

        // TODO: Switch Statements
        // Later:
        // TODO:
        //  Controller Input

        // WindowMinimized
        // WindowMaximized

        switch (Type)
        {
        // Pointer In Out
        case EnterNotify:
            _wImpl->eStack.PushPointerInEvent();
            break;
        case LeaveNotify:
            _wImpl->eStack.PushPointerOutEvent();
            break;

        // Window Exposed
        case Expose:
            _wImpl->eStack.PushWindowExposedEvent();
            break;

        // Mouse Button Event
        case ButtonPress:
        case ButtonRelease:
        {
            // TODO: Cleanup and use eStack functions
            WEvent ButtonEvent;
            ButtonEvent.Type           = WEventType::ButtonEvent;

            switch (Event->xbutton.button)
            {
            case Button1:   // LEFT
                ButtonEvent.eButton.Code = Buttons::Button_1;
                break;
            case Button2:   // MIDDLE
                ButtonEvent.eButton.Code = Buttons::Button_3;
                break;
            case Button3:   // RIGHT
                ButtonEvent.eButton.Code = Buttons::Button_2;
                break;

            // Scroll
            case Button4: // Up
                ButtonEvent.Type = WEventType::ScrollEvent;
                ButtonEvent.eScroll.Delta = 120.0f;
                ButtonEvent.eScroll.Axis = ScrollAxis::ScrollVertical;
                ButtonEvent.eScroll.Direction = Scroll::ScrollUp;
                break;
            case Button5: // Down
                ButtonEvent.Type = WEventType::ScrollEvent;
                ButtonEvent.eScroll.Delta = -120.0f;
                ButtonEvent.eScroll.Axis = ScrollAxis::ScrollVertical;
                ButtonEvent.eScroll.Direction = Scroll::ScrollDown;
                break;
            case 6: // Left
                ButtonEvent.Type = WEventType::ScrollEvent;
                ButtonEvent.eScroll.Delta = -120.0f;
                ButtonEvent.eScroll.Axis = ScrollAxis::ScrollHorizontal;
                ButtonEvent.eScroll.Direction = Scroll::ScrollLeft;
                break;
            case 7: // Right
                ButtonEvent.Type = WEventType::ScrollEvent;
                ButtonEvent.eScroll.Delta = 120.0f;
                ButtonEvent.eScroll.Axis = ScrollAxis::ScrollHorizontal;
                ButtonEvent.eScroll.Direction = Scroll::ScrollRight;
                break;

            
            case 8:
                ButtonEvent.eButton.Code = Buttons::Button_4;
                break;
            case 9:
                ButtonEvent.eButton.Code = Buttons::Button_5;
                break;
            default:
                ButtonEvent.eButton.Code = Buttons::NoButton;
                break;
            }

            if (ButtonEvent.Type != WEventType::ScrollEvent)
            {
                ButtonEvent.eButton.Action =
                 Type == ButtonPress ? ButtonAction::Pressed : ButtonAction::Released;

                ButtonEvent.eButton.PointerX = Event->xmotion.x;
                ButtonEvent.eButton.PointerY = Event->xmotion.y;
            }
            else 
            {
                ButtonEvent.eScroll.AbsDelta = abs(ButtonEvent.eScroll.Delta);
                ButtonEvent.eScroll.PointerX = Event->xmotion.x;
                ButtonEvent.eScroll.PointerY = Event->xmotion.y;
            }

            _wImpl->eStack.PushEvent(ButtonEvent);
            break;
        }

        // Mouse Moved
        case MotionNotify:
            _wImpl->eStack.PushPointerMovedEvent(Event->xmotion.x, Event->xmotion.y);
            break;

        // Key Event
        case KeyPress:
        {
            // xKeyCode to UTF-8
            Status xStatus;
            KeySym xKeySym;
            char utf8[4];
            Xutf8LookupString(X11Input::IC, &Event->xkey, utf8, 4, &xKeySym, &xStatus);

            _wImpl->eStack.PushCharacterInputEvent(X11Input::UTF82C32(utf8));
        } // fall through
        case KeyRelease:
        {
            _wImpl->eStack.PushKeyInputEvent(
                X11Input::FromX11Key(Event->xkey.keycode),
                Type == KeyPress ? KeyAction::Pressed : KeyAction::Released
            );
            break;
        }

        // Structure Notify
        case ConfigureNotify:
        {
            _wImpl->eStack.PushWindowChangedEvent(
                Event->xconfigure.x, Event->xconfigure.y,
                Event->xconfigure.width, Event->xconfigure.height
            );
            break;
        }

        case PropertyNotify:
        {
            XPropertyEvent* Property = &Event->xproperty;

            if (Property->atom == _wImpl->WMNetState)
            {
                Atom            Type;
                int             Format;
                unsigned long   nAtoms,
                                BytesAfter;
                Atom*           Atoms = NULL;

                XGetWindowProperty(
                    _wImpl->xDisplay, _wImpl->xWindow,
                    _wImpl->WMNetState,
                    0, 1024, False, XA_ATOM,
                    &Type, &Format, &nAtoms, &BytesAfter,
                    (unsigned char**)&Atoms
                );

                if (Atoms[0] == _wImpl->WMNetStateHidden
                 || Atoms[0] == _wImpl->WMNetActionMinimise)
                {
                    _wImpl->eStack.PushWindowMinimizedEvent();
                    break;
                }

                if (Atoms[0] == _wImpl->WMNetMaximizedHorizontal
                 || Atoms[0] == _wImpl->WMNetMaximizedVertical)
                {
                    _wImpl->eStack.PushWindowMaximizedEvent();
                    break;
                }
                
                XFree(Atoms);
            }

            break;
        }

        // Client Messages
        case ClientMessage:
        {
            XClientMessageEvent* Message = &Event->xclient;
            
            if (static_cast<Atom>(Message->data.l[0]) == _wImpl->WMDeleteWindow)
            {
                _wImpl->eStack.PushWindowCloseEvent();
            }

            break;
        }

        default: break;
        }
    }

    if (_wImpl->uCallback)
        _wImpl->uCallback();
}