#include <windowsx.h>
#include "WinWindow.hpp"

#define ptt_t std::pair<int, int>
#define wDestroyWindow WINUSERAPI BOOL WINAPI DestroyWindow(_In_ HWND hWnd);

typedef struct __WinInput
{
    static std::map<LONG, Keys> WinInputMap;
    static int LastCX, LastCY;
    static bool InWindow;

    static void Init()
    {
        WinInputMap = {
            { 'Q', Keys::Q },
            { 'W', Keys::W },
            { 'E', Keys::E },
            { 'R', Keys::R },
            { 'T', Keys::T },
            { 'Y', Keys::Y },
            { 'U', Keys::U },
            { 'I', Keys::I },
            { 'O', Keys::O },
            { 'P', Keys::P },
            { 'A', Keys::A },
            { 'S', Keys::S },
            { 'D', Keys::D },
            { 'F', Keys::F },
            { 'G', Keys::G },
            { 'H', Keys::H },
            { 'J', Keys::J },
            { 'K', Keys::K },
            { 'L', Keys::L },
            { 'Z', Keys::Z },
            { 'X', Keys::X },
            { 'C', Keys::C },
            { 'V', Keys::V },
            { 'B', Keys::B },
            { 'N', Keys::N },
            { 'M', Keys::M },
            { VK_SPACE, Keys::Space },
            { VK_ESCAPE, Keys::Escape },
            { VK_F1, Keys::F1 },
            { VK_F2, Keys::F2 },
            { VK_F3, Keys::F3 },
            { VK_F4, Keys::F4 },
            { VK_F5, Keys::F5 },
            { VK_F6, Keys::F6 },
            { VK_F7, Keys::F7 },
            { VK_F8, Keys::F8 },
            { VK_F9, Keys::F9 },
            { VK_F10, Keys::F10 },
            { VK_F11, Keys::F11 },
            { VK_F12, Keys::F12 },
            { VK_NUMPAD1, Keys::Num1 },
            { VK_NUMPAD2, Keys::Num2 },
            { VK_NUMPAD3, Keys::Num3 },
            { VK_NUMPAD4, Keys::Num4 },
            { VK_NUMPAD5, Keys::Num5 },
            { VK_NUMPAD6, Keys::Num6 },
            { VK_NUMPAD7, Keys::Num7 },
            { VK_NUMPAD8, Keys::Num8 },
            { VK_NUMPAD9, Keys::Num9 },
            { VK_NUMPAD0, Keys::Num0 },
            { VK_SNAPSHOT, Keys::PrintScreen },
            { VK_SCROLL, Keys::ScrollLock },
            { VK_PAUSE, Keys::Pause},
            { VK_OEM_3, Keys::Tilde },
            { VK_BACK, Keys::Backspace },
            { VK_TAB, Keys::Tab },
            { VK_CAPITAL, Keys::Caps },
            { VK_RETURN, Keys::Return },
            { VK_LSHIFT, Keys::LShift },
            { VK_RSHIFT, Keys::RShift },
            { VK_LCONTROL, Keys::LControl },
            { VK_RCONTROL, Keys::RControl },
            { 0L, Keys::LMeta },
            { 0L, Keys::RMeta },
            { VK_MENU, Keys::LAlt },
            { VK_RMENU, Keys::RAlt },
            { 0L, Keys::Menu },
            { VK_INSERT, Keys::Insert },
            { VK_HOME, Keys::Home },
            { VK_DELETE, Keys::Delete },
            { VK_END, Keys::End },
            { 0L, Keys::PageUp },
            { 0L, Keys::PageDown },
            { VK_UP, Keys::ArrowUp },
            { VK_LEFT, Keys::ArrowLeft },
            { VK_DOWN, Keys::ArrowDown },
            { VK_RIGHT, Keys::ArrowRight },
        };

    }

    static void SaveCursorInfo(const ptt_t& _Cursor)
    {
        LastCX = _Cursor.first;
        LastCY = _Cursor.second;
    }

    static ptt_t GetCursorDelta(const ptt_t& _Cursor)
    {
        return { LastCX - _Cursor.first, LastCY - _Cursor.second };
    }

    static ButtonAction ActionFromMessage(int Message)
    {
        switch (Message)
        {
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
            return ButtonAction::Pressed;
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
            return ButtonAction::Released;
        default: break;
        }
        return ButtonAction::NoAction;
    }

    static Buttons FromWinButton(int Button)
    {
        switch (Button)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            return Buttons::Button_1;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            return Buttons::Button_2;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            return Buttons::Button_3;
        default: break;
        }
        return Buttons::NoButton;
    }

    static ScrollAxis GetScrollAxis(int Message)
    {
        switch (Message)
        {
        case WM_MOUSEHWHEEL:
            return ScrollAxis::ScrollHorizontal;
        case WM_MOUSEWHEEL:
            return ScrollAxis::ScrollVertical;
        default: break;
        }
        return ScrollAxis::NoAxis;
    }

    static Scroll GetScrollDirection(int Delta, ScrollAxis Axis)
    {
        switch (Axis)
        {
        case ScrollAxis::ScrollVertical:
            return (Delta < 0 ? Scroll::ScrollDown : Scroll::ScrollUp);
        case ScrollAxis::ScrollHorizontal:
            return (Delta < 0 ? Scroll::ScrollLeft : Scroll::ScrollRight);
        default: break;
        }
        return Scroll::NoScroll;
    }

    static Keys FromVirtualKey(int Key)
    {
        if (WinInputMap.find(Key) == WinInputMap.end())
            return Keys::NoKey;
        return WinInputMap[Key];
    }

}WinInput;

int     WinInput::LastCX,
        WinInput::LastCY;

std::map<LONG, Keys>
         WinInput::WinInputMap;

bool    WinInput::InWindow = false;

LRESULT CALLBACK IWinWindow::SetupWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CREATE)
    {
        const CREATESTRUCTW* wmpCreateStruct = reinterpret_cast<const CREATESTRUCTW*>(lParam);
        IWinWindow* iwpWindow = static_cast<IWinWindow*>(wmpCreateStruct->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&IWinWindow::HandleMessages));
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(iwpWindow));
        return iwpWindow->HandleMessage(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK IWinWindow::HandleMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    IWinWindow* iwpWindow = reinterpret_cast<IWinWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return iwpWindow->HandleMessage(hWnd, msg, wParam, lParam);
}

LRESULT IWinWindow::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // NoEvent
    // 
    // ~ WindowClosed
    // ~ WindowExposed
    // 
    // ~ WindowChanged
    // ~ WindowResized
    // ~ WindowMoved
    // 
    // ~ PointerMoved
    // ~ PointerIn
    // ~ PointerOut
    // 
    // ~ KeyEvent
    // ~ ButtonEvent
    // 
    // ~ CharEvent
    //
    // ~ ScrollEvent
    // 
    // WindowMinimized
    // WindowMaximized
    //

    // TODO: XButton's

    switch (msg)
    {

    // Window Close Event
    case WM_DESTROY:
    case WM_CLOSE:
    {                
        WEvent CloseEvent;
        CloseEvent.Type = WEventType::WindowClosed;

        _wImpl->EventStack.push(CloseEvent);
        break;
    }

    case WM_CHAR:
    {
        WEvent CharEvent;
        CharEvent.Type = WEventType::CharEvent;
        CharEvent.eChar = wParam;

        _wImpl->EventStack.push(CharEvent);
        break;
    }

    case WM_MOUSEHWHEEL:
    case WM_MOUSEWHEEL:
    {
        WEvent ScrollEvent;
        ScrollEvent.Type = WEventType::ScrollEvent;

        POINT Point;
        Point.x = GET_X_LPARAM(lParam);
        Point.y = GET_Y_LPARAM(lParam);
        ScreenToClient(_wImpl->Window, &Point);
        
        const int Delta = GET_WHEEL_DELTA_WPARAM(wParam);

        ScrollEvent.eScroll.PointerX = Point.x;
        ScrollEvent.eScroll.PointerY = Point.y;

        ScrollEvent.eScroll.Axis = WinInput::GetScrollAxis(msg);
        ScrollEvent.eScroll.Direction = WinInput::GetScrollDirection(Delta, ScrollEvent.eScroll.Axis);

        ScrollEvent.eScroll.Delta = (float)Delta;
        ScrollEvent.eScroll.AbsDelta = abs((float)Delta);

        _wImpl->EventStack.push(ScrollEvent);
        break;
    }

    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    case WM_LBUTTONUP:
    {
        const POINTS Point = MAKEPOINTS(lParam);

        WEvent ButtonEvent;
        ButtonEvent.Type = WEventType::ButtonEvent;

        ButtonEvent.eButton.Action = WinInput::ActionFromMessage(msg);
        ButtonEvent.eButton.Code = WinInput::FromWinButton(msg);

        ButtonEvent.eButton.PointerX = Point.x;
        ButtonEvent.eButton.PointerY = Point.y;

        _wImpl->EventStack.push(ButtonEvent);
        break;
    }

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        // Key Event
        WEvent KeyEvent;
        KeyEvent.Type = WEventType::KeyEvent;
        KeyEvent.eKey.Code = WinInput::FromVirtualKey(wParam);
        KeyEvent.eKey.Action = msg == WM_KEYDOWN ? KeyAction::Pressed : KeyAction::Released;

        _wImpl->EventStack.push(KeyEvent);
        break;
    }

    // Window Exposed Event
    case WM_PAINT:
    {
        RECT rTemp;
        if (GetUpdateRect(_wImpl->Window, &rTemp, FALSE))
        {
            ValidateRect(_wImpl->Window, NULL);
            
            WEvent ExposeEvent;
            ExposeEvent.Type = WEventType::WindowExposed;

            _wImpl->EventStack.push(ExposeEvent);
        }

        break;
    }

    // Window Changed Event
    case WM_SIZE:
    case WM_MOVE:
    {
        WEvent ChangedEvent;
        ChangedEvent.Type = WEventType::WindowChanged;

        RECT rTemp;
        GetWindowRect(_wImpl->Window, &rTemp);

        ChangedEvent.eWChanged.W = rTemp.right;
        ChangedEvent.eWChanged.H = rTemp.bottom;

        ChangedEvent.eWChanged.X = rTemp.left;
        ChangedEvent.eWChanged.Y = rTemp.top;

        _wImpl->EventStack.push(ChangedEvent);
        break;
    }

    // Pointer Moved
    // Pointed In
    // Pointer Out
    // TODO: Use Client Region
    case WM_MOUSEMOVE:
    {
        const POINTS Point = MAKEPOINTS(lParam);

        if (Point.x >= 0 && Point.x < static_cast<SHORT>(_wImpl->_Attributes.Width)
            && Point.y >= 0 && Point.y < static_cast<SHORT>(_wImpl->_Attributes.Height))
        {
            WEvent PMovedEvent;
            PMovedEvent.Type = WEventType::PointerMoved;

            ptt_t CursorPosition =
            {
                Point.x, Point.y
            };
            ptt_t CursorDelta = WinInput::GetCursorDelta(CursorPosition);

            PMovedEvent.ePMoved.PointerX = CursorPosition.first;
            PMovedEvent.ePMoved.PointerY = CursorPosition.second;
            PMovedEvent.ePMoved.DeltaX = CursorDelta.first;
            PMovedEvent.ePMoved.DeltaY = CursorDelta.second;

            WinInput::SaveCursorInfo(CursorPosition);

            _wImpl->EventStack.push(PMovedEvent);

            if (!WinInput::InWindow)
            {
                SetCapture(hWnd);
                WinInput::InWindow = true;

                WEvent PointerNotify;
                PointerNotify.Type = WEventType::PointerIn;

                _wImpl->EventStack.push(PointerNotify);
            }
            break;
        }
        else
        {
            WinInput::InWindow = false;
            WEvent PointerNotify;
            PointerNotify.Type = WEventType::PointerOut;

            _wImpl->EventStack.push(PointerNotify);
            ReleaseCapture();
        }
        break;
    }
    
    default: break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void IWinWindow::CreateWindow()
{
    WinInput::Init();

	_wImpl->Instance = GetModuleHandle(nullptr);

    WNDCLASSEX WndInfo;

    WndInfo.cbSize = sizeof WndInfo;

	WndInfo.lpfnWndProc = SetupWinProc;
	WndInfo.hInstance = _wImpl->Instance;
	WndInfo.lpszClassName = "KotWinWindow";
	
    WndInfo.style = CS_OWNDC;

	WndInfo.cbClsExtra = 0;
	WndInfo.cbWndExtra = 0;

	WndInfo.hIcon = nullptr;
	WndInfo.hCursor = nullptr;
	WndInfo.hbrBackground = nullptr;
	WndInfo.lpszMenuName = nullptr;
    WndInfo.hIconSm = nullptr;

	RegisterClassEx(&WndInfo);

    IWindow::Attributes* Attribs = &_wImpl->_Attributes;

    DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    if (!(Attribs->aFlags & IWindow::Flags::NoResizing))
    {
        dwStyle |= WS_MAXIMIZEBOX | WS_SIZEBOX;
    }

    _wImpl->Window = CreateWindowExA(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        "KotWinWindow", _wImpl->_Attributes.Title,
        dwStyle,
        Attribs->X, Attribs->Y,
        Attribs->Width, Attribs->Height,
        nullptr, nullptr, _wImpl->Instance, this
    );

    ShowWindow(_wImpl->Window, SW_SHOW);

    if (Attribs->aFlags & IWindow::Flags::PositionCentered)
    {        
        HWND wParent = GetDesktopWindow();

        RECT wrParentSize;
        RECT wrWindowSize;
        GetWindowRect(wParent, &wrParentSize);
        GetWindowRect(_wImpl->Window, &wrWindowSize);

        unsigned int nWidth = wrWindowSize.right - wrWindowSize.left;
        unsigned int nHeight = wrWindowSize.bottom - wrWindowSize.top;

        unsigned int nX = ((wrParentSize.right - wrParentSize.left) - nWidth) / 2 + wrParentSize.left;
        unsigned int nY = ((wrParentSize.bottom - wrParentSize.top) - nHeight) / 2 + wrParentSize.top;

        Attribs->X = nX;
        Attribs->Y = nY;

        MoveWindow(_wImpl->Window, Attribs->X, Attribs->Y, nWidth, nHeight, FALSE);
    }
}

void IWinWindow::DestroyWindow()
{
    UnregisterClass("KotWinWindow", _wImpl->Instance);
    wDestroyWindow(_wImpl->Window);
}

IWinWindow::IWinWindow(const IWindow::Attributes& _Attributes)
{
    _wImpl = new __IWinImpl;
    _wImpl->_Attributes = _Attributes;
    CreateWindow();
}

IWinWindow::~IWinWindow()
{
    DestroyWindow();
    delete _wImpl;
}

void IWinWindow::SetCursorMode(const CursorMode& _Cursor)
{
    switch (_Cursor)
    {
    case CursorMode::Free:
    {
        ClipCursor(nullptr);
        break;
    }
    case CursorMode::Confined:
    {
        RECT rect;
        GetClientRect(_wImpl->Window , &rect);
        MapWindowPoints(_wImpl->Window, nullptr, reinterpret_cast<POINT*>(&rect), 2);
        ClipCursor(&rect);
        printf("%d", GetLastError());
        break;
    }
    case CursorMode::Locked:
        break;
    default: break;
    }
}

unsigned IWinWindow::GetPlatform() const
{
    return IWindow::Platform::Windows;
}

DisplayScreen* IWinWindow::GetDisplayData() const
{
    return &_wImpl->_Display;
}

void* IWinWindow::SetNativeData(void* _Data) { return _Data; }
void* IWinWindow::GetNativeData() const { return nullptr; }

WEvent IWinWindow::Event() const
{
    WEvent Event = _wImpl->EventStack.top();
    _wImpl->EventStack.pop();
    return Event;
}

bool IWinWindow::IsEvent() const
{
    // If the event stack is not empty
    return (!_wImpl->EventStack.empty());
}

void IWinWindow::Update()
{
    MSG mTempMsg;
    while (PeekMessage(&mTempMsg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&mTempMsg);
        DispatchMessage(&mTempMsg);
    }
}