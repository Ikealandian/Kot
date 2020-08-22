#include "X11/X11Window.hpp"

typedef struct __X11InputMapping
{
    static std::map<const char*, Keys> X11InputMap;
    static std::vector<unsigned int> X11KeyTable;
    static std::vector<Keys> KeyTable;

    static XIC IC;
    static XIM IM;

    static int LastCX, LastCY;

    static void SaveCursorInfo(const std::pair<int, int>& _Cursor)
    {
        LastCX = _Cursor.first;
        LastCY = _Cursor.second;
    }   

    static std::pair<int, int> GetCursorDelta(const std::pair<int, int>& _Cursor)
    {
        return { LastCX - _Cursor.first, LastCY - _Cursor.second };
    }

    static char32_t UTF82C32(char _utf8[4])
    {
        char32_t char32;
        if ((_utf8[0] & 0x80) == 0x00)
                    char32 = _utf8[0];
        else if ((_utf8[0] & 0xE0) == 0xC0)
            char32 
                = (_utf8[0] & 0x1F) << 6
                | (_utf8[1] & 0x3F);
        else if ((_utf8[2] & 0xF0) == 0xE0)
            char32 
                = (_utf8[0] & 0x0F) << 12
                | (_utf8[1] & 0x3F) << 6
                | (_utf8[2] & 0x3F);
        else if ((_utf8[0] & 0xF8) == 0xF0)
            char32 
                = (_utf8[0] & 0x07) << 18
                | (_utf8[1] & 0x3F) << 12
                | (_utf8[2] & 0x3F) << 6
                | (_utf8[3] & 0x3F);
        return char32;
    }

    static void InitKeyTables(Display** _xdis)
    {
        XkbDescPtr pMapDesc = XkbGetMap(*_xdis, 0, XkbUseCoreKbd);
        XkbGetNames(*_xdis, XkbKeyNamesMask, pMapDesc);

        X11KeyTable.resize(pMapDesc->max_key_code + 1);
        KeyTable.resize(pMapDesc->max_key_code + 1);

        for (unsigned int X11Code = 0; X11Code <= pMapDesc->max_key_code; X11Code++) {
            
            bool Found = false;
            for (auto it : X11InputMap) {
                if (strncmp(it.first, pMapDesc->names->keys[X11Code].name, XkbKeyNameLength) == 0) {

                    KeyTable[X11Code] = it.second;
                    X11KeyTable[(std::size_t)it.second] = X11Code;
                    
                    Found = true;
                    break;
                }
            }
            
            if (!Found)
            {
                KeyTable[X11Code] = Keys::NoKey;
            }
        }

        XkbFreeNames(pMapDesc, XkbKeyNamesMask, True);
        XkbFreeKeyboard(pMapDesc, 0, True);
    }

    static void Init(Display** _xdis, Window* _xwin)
    {
        X11InputMap = {
            { "AE01", Keys::Num1 },
            { "AE02", Keys::Num2 },
            { "AE03", Keys::Num3 },
            { "AE04", Keys::Num4 },
            { "AE05", Keys::Num5 },
            { "AE06", Keys::Num6 },
            { "AE07", Keys::Num7 },
            { "AE08", Keys::Num8 },
            { "AE09", Keys::Num9 },
            { "AE10", Keys::Num0 },
            { "AD01", Keys::Q },
            { "AD02", Keys::W },
            { "AD03", Keys::E },
            { "AD04", Keys::R },
            { "AD05", Keys::T },
            { "AD06", Keys::Y },
            { "AD07", Keys::U },
            { "AD08", Keys::I },
            { "AD09", Keys::O },
            { "AD10", Keys::P },
            { "AC01", Keys::A },
            { "AC02", Keys::S },
            { "AC03", Keys::D },
            { "AC04", Keys::F },
            { "AC05", Keys::G },
            { "AC06", Keys::H },
            { "AC07", Keys::J },
            { "AC08", Keys::K },
            { "AC09", Keys::L },
            { "AB01", Keys::Z },
            { "AB02", Keys::X },
            { "AB03", Keys::C },
            { "AB04", Keys::V },
            { "AB05", Keys::B },
            { "AB06", Keys::N },
            { "AB07", Keys::M },
            // { "AC10", Keys::Semicolon },
            // { "AC11", Keys::Apostrophe },
            // { "AD11", Keys::LBracket },
            // { "AD12", Keys::RBracket },
            // { "AB08", Keys::Comma },
            // { "AB09", Keys::Period },
            // { "AB10", Keys::Slash },
            { "ESC",  Keys::Escape },
            { "FK01", Keys::F1 },
            { "FK02", Keys::F2 },
            { "FK03", Keys::F3 },
            { "FK04", Keys::F4 },
            { "FK05", Keys::F5 },
            { "FK06", Keys::F6 },
            { "FK07", Keys::F7 },
            { "FK08", Keys::F8 },
            { "FK09", Keys::F9 },
            { "FK10", Keys::F10 },
            { "FK11", Keys::F11 },
            { "FK12", Keys::F12 },
            { "PRSC", Keys::PrintScreen },
            { "SCLK", Keys::ScrollLock },
            { "PAUS", Keys::Pause},
            { "TLDE", Keys::Tilde },
            { "BKSP", Keys::Backspace },
            { "TAB",  Keys::Tab },
            // { "BKSL", Keys::Backslash },
            { "CAPS", Keys::Caps },
            { "RTRN", Keys::Return },
            { "LFSH", Keys::LShift },
            { "RTSH", Keys::RShift },
            { "LCTL", Keys::LControl },
            { "RCTL", Keys::RControl },
            { "LWIN", Keys::LMeta },
            { "RWIN", Keys::RMeta },
            { "LALT", Keys::LAlt },
            { "RLAT", Keys::RAlt },
            { "SPCE", Keys::Space },
            { "MENU", Keys::Menu },
            { "INS",  Keys::Insert },
            { "HOME", Keys::Home },
            { "PGUP", Keys::PageUp },
            { "DELE", Keys::Delete },
            { "END",  Keys::End },
            { "PGDN", Keys::PageDown },
            { "UP",   Keys::ArrowUp },
            { "LEFT", Keys::ArrowLeft },
            { "DOWN", Keys::ArrowDown },
            { "RGHT", Keys::ArrowRight },
            // { "NMLK", Keys::NumLock },
            // { "KPDV", Keys::Divide },
            // { "KPMU", Keys::Multiply },
            // { "KPSU", Keys::Subtract },
            // { "KPAD", Keys::Add },
            // { "KPEN", Keys::Enter },
            // { "KPDL", Keys::Decimal },
            // { "KP0",  Keys::Numpad0 },
            // { "KP1",  Keys::Numpad1 },
            // { "KP2",  Keys::Numpad2 },
            // { "KP3",  Keys::Numpad3 },
            // { "KP4",  Keys::Numpad4 },
            // { "KP5",  Keys::Numpad5 },
            // { "KP6",  Keys::Numpad6 },
            // { "KP7",  Keys::Numpad7 },
            // { "KP8",  Keys::Numpad8 },
            // { "KP9",  Keys::Numpad9 },
        };

        InitKeyTables(_xdis);

        XSetLocaleModifiers("");
        IM = XOpenIM(*_xdis, nullptr, nullptr, nullptr);

        if (!IM)
        {
            XSetLocaleModifiers("@im=none");
            IM = XOpenIM(*_xdis, nullptr, nullptr, nullptr);    
        }

        IC = XCreateIC
        (
            IM,
            XNInputStyle,
            XIMPreeditNothing | XIMStatusNothing,
            XNClientWindow,
            *_xwin,
            XNFocusWindow,
            *_xwin,
            nullptr
        );
    }

    static Keys FromX11Key(int Key)
    {
        if (Key > (int)Keys::Last)
            return Keys::NoKey;
        return KeyTable[Key];
    }
}X11Input;

// Static X11Input Variables
XIC                             X11Input::IC;
XIM                             X11Input::IM;
std::map<const char*, Keys>     X11Input::X11InputMap;
std::vector<unsigned int>       X11Input::X11KeyTable;
std::vector<Keys>               X11Input::KeyTable;
int                             X11Input::LastCX,
                                X11Input::LastCY;
// Results:
//  0x1 xOpenDisplayError;

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
    DisplayScreen* DpyScr = &_wImpl->_Display;
    DpyScr->Width = XDisplayWidth(_wImpl->xDisplay, _wImpl->iScreen);
    DpyScr->Height = XDisplayHeight(_wImpl->xDisplay, _wImpl->iScreen);

    // Impl Attributes
    IWindow::Attributes* Attribs = &_wImpl->_Attributes;

    // Create Window
    _wImpl->xWindow = XCreateSimpleWindow(
        _wImpl->xDisplay, _wImpl->xRoot,        // Display, Parent Window
        Attribs->X,     Attribs->Y,             // X, Y
        Attribs->Width, Attribs->Height,        // W, H
        0, 0,                                   // Border Size, Border Colour
        0x0                                     // Background Colour
    );

    // Atoms
    _wImpl->WMDeleteWindow = XInternAtom(_wImpl->xDisplay, "WM_DELETE_WINDOW", False);

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
        ButtonMotionMask
    );
    XMapWindow(_wImpl->xDisplay, _wImpl->xWindow);

    // Flags

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
}

void IX11Window::DestroyWindow()
{
    XDestroyWindow(_wImpl->xDisplay, _wImpl->xWindow);
    XCloseDisplay(_wImpl->xDisplay);
}

IX11Window::IX11Window(const IWindow::Attributes& _Attributes)
{
    _wImpl = new __IWinImpl;
    _wImpl->_Attributes = _Attributes;
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
    return &_wImpl->_Display;
}
    
void* IX11Window::SetNativeData(void* _Data) { return _Data; }
void* IX11Window::GetNativeData() const      { return nullptr; }

WEvent IX11Window::Event() const
{
    WEvent Event = _wImpl->EventStack.top();
    _wImpl->EventStack.pop();
    return Event;
}

bool IX11Window::IsEvent() const
{
    // If the event stack is not empty
    return (!_wImpl->EventStack.empty());
}

void IX11Window::Update()
{
    while (XPending(_wImpl->xDisplay))
    {
        XNextEvent(_wImpl->xDisplay, &_wImpl->xEvent);

        XEvent* Event = &_wImpl->xEvent;
        int Type = _wImpl->xEvent.type;

        // TODO: Switch Statements
        // Later:
        // TODO:
        //  Controller Input
        switch (Type)
        {
        // Pointer In Out
        case EnterNotify:
        case LeaveNotify:
        {
            WEvent PointerNotify;
            PointerNotify.Type = Type == EnterNotify ? WEventType::PointerIn : WEventType::PointerOut;

            _wImpl->EventStack.push(PointerNotify);
            break;
        }

        // Window Exposed
        case Expose:
        {
            WEvent ExposeEvent;
            ExposeEvent.Type = WEventType::WindowExposed;

            _wImpl->EventStack.push(ExposeEvent);
            break;
        }

        // Mouse Button Event
        case ButtonPress:
        case ButtonRelease:
        {
            WEvent ButtonEvent;
            ButtonEvent.Type           = WEventType::ButtonEvent;
            ButtonEvent.eButton.Action = Type == ButtonPress ? ButtonAction::Pressed : ButtonAction::Released;

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
            // TODO: 4 and 5 Dont Work
            case Button4:
                ButtonEvent.eButton.Code = Buttons::Button_4;
                break;
            case Button5:
                ButtonEvent.eButton.Code = Buttons::Button_5;
                break;
            default:
                ButtonEvent.eButton.Code = Buttons::NoButton;
                break;
            }

            ButtonEvent.eButton.PointerX = Event->xmotion.x;
            ButtonEvent.eButton.PointerY = Event->xmotion.y;

            _wImpl->EventStack.push(ButtonEvent);
            break;
        }

        // Mouse Moved
        case MotionNotify:
        {
            WEvent PMovedEvent;
            PMovedEvent.Type = WEventType::PointerMoved;
            
            std::pair<int, int> CursorPosition = 
            {
                Event->xmotion.x, Event->xmotion.y
            };
            std::pair<int, int> CursorDelta = X11Input::GetCursorDelta(CursorPosition);

            PMovedEvent.ePMoved.PointerX = CursorPosition.first;
            PMovedEvent.ePMoved.PointerY = CursorPosition.second;
            PMovedEvent.ePMoved.DeltaX = CursorDelta.first;
            PMovedEvent.ePMoved.DeltaY = CursorDelta.second;

            X11Input::SaveCursorInfo(CursorPosition);

            _wImpl->EventStack.push(PMovedEvent);
            break;
        }

        // Key Event
        case KeyPress:
        {
            // xKeyCode to UTF-8
            Status xStatus;
            KeySym xKeySym;
            char utf8[4];
            Xutf8LookupString(X11Input::IC, &Event->xkey, utf8, 4, &xKeySym, &xStatus);

            // Char Event
            WEvent CharEvent;
            CharEvent.Type          = WEventType::CharEvent;
            CharEvent.eChar         = X11Input::UTF82C32(utf8);

            _wImpl->EventStack.push(CharEvent);
        } // fall through
        case KeyRelease:
        {
            // Key Event
            WEvent KeyEvent;
            KeyEvent.Type           = WEventType::KeyEvent;
            KeyEvent.eKey.Code      = X11Input::FromX11Key(Event->xkey.keycode);
            KeyEvent.eKey.Action    = Type == KeyPress ? KeyAction::Pressed : KeyAction::Released;

            _wImpl->EventStack.push(KeyEvent);
            break;
        }

        // Client Messages
        case ClientMessage:
        {
            XClientMessageEvent* Message = &Event->xclient;
            if (static_cast<Atom>(Message->data.l[0]) == _wImpl->WMDeleteWindow)
            {
                WEvent CloseEvent;
                CloseEvent.Type = WEventType::WindowClosed;

                _wImpl->EventStack.push(CloseEvent);
            }
            break;
        }

        default: break;
        }
    }

    // Window Resized

    // if (true)
    // {
    //     WEvent ResizedEvent;
    // }

    // Window Moved

    // if (true)
    // {
    //     WEvent MovedEvent;
    // }
}