#include <windowsx.h>
#include "WinWindow.hpp"

#define wDestroyWindow WINUSERAPI BOOL WINAPI DestroyWindow(_In_ HWND hWnd);

typedef struct __WinInput
{
    static std::map<LONG, Keys> WinInputMap;
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

    static ButtonAction ActionFromMessage(int Message)
    {
        switch (Message)
        {
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_XBUTTONDOWN:
            return ButtonAction::Pressed;
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
        case WM_XBUTTONUP:
            return ButtonAction::Released;
        default: break;
        }
        return ButtonAction::NoAction;
    }

    static Buttons FromWinButton(int Button, LPARAM wParam)
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
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        {
            DWORD vkButton = GET_XBUTTON_WPARAM(wParam);
            switch (vkButton)
            {
            case XBUTTON1:
                return Buttons::Button_4;
            case XBUTTON2:
                return Buttons::Button_5;
            default: break;
            }
            break;
        }
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

    static Keys FromVirtualKey(int Key)
    {
        if (WinInputMap.find(Key) == WinInputMap.end())
            return Keys::NoKey;
        return WinInputMap[Key];
    }
}WinInput;

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
    // TODO: XButton's

    switch (msg)
    {

    // Window Close Event
    case WM_DESTROY:
    case WM_CLOSE:
    {                
        _wImpl->eStack.PushWindowCloseEvent();
        break;
    }

    // Character Input
    case WM_CHAR:
    {
        _wImpl->eStack.PushCharacterInputEvent(wParam);
        break;
    }

    case WM_MOUSEHWHEEL:
    case WM_MOUSEWHEEL:
    {
        POINT Point;
        Point.x = GET_X_LPARAM(lParam);
        Point.y = GET_Y_LPARAM(lParam);
        ScreenToClient(_wImpl->Window, &Point);
        
        float Delta = GET_WHEEL_DELTA_WPARAM(wParam);

        _wImpl->eStack.PushMouseScrollEvent(Delta, WinInput::GetScrollAxis(msg), Point.x, Point.y);
        break;
    }

    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDOWN:
    case WM_XBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    case WM_LBUTTONUP:
    case WM_XBUTTONUP:
    {
        const POINTS Point = MAKEPOINTS(lParam);

        _wImpl->eStack.PushButtonInputEvent(
            WinInput::FromWinButton(msg, wParam),
            WinInput::ActionFromMessage(msg),
            Point.x, Point.y
        );
        break;
    }

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        _wImpl->eStack.PushKeyInputEvent(
            WinInput::FromVirtualKey(wParam),
            WM_KEYDOWN ? KeyAction::Pressed : KeyAction::Released
        );
        break;
    }

    // Window Exposed Event
    case WM_PAINT:
    {
        RECT rTemp;
        if (GetUpdateRect(_wImpl->Window, &rTemp, FALSE))
        {
            ValidateRect(_wImpl->Window, NULL);
            _wImpl->eStack.PushWindowExposedEvent();
        }

        break;
    }

    // Window Changed Event
    case WM_SIZE:
    {
        if (wParam == SIZE_MINIMIZED)
        {
            _wImpl->eStack.PushWindowMinimizedEvent();
            break;
        }

        if (wParam == SIZE_MAXIMIZED)
        {
            _wImpl->eStack.PushWindowMaximizedEvent();
        }
        [[fallthrough]];
    }
    case WM_MOVE:
    {
        RECT rTemp;
        GetWindowRect(_wImpl->Window, &rTemp);

        long X = rTemp.left;
        long Y = rTemp.top;
 
        long Width = rTemp.right - rTemp.left;
        long Height = rTemp.bottom - rTemp.top;

        _wImpl->eStack.PushWindowChangedEvent(X, Y, Width, Height);
        break;
    }

    // Pointer Moved
    // Pointed In Out
    // TODO: Use Client Region
    case WM_MOUSEMOVE:
    {
        const POINTS Point = MAKEPOINTS(lParam);

        if (Point.x >= 0 && Point.x < static_cast<SHORT>(_wImpl->wAttribs.Width)
            && Point.y >= 0 && Point.y < static_cast<SHORT>(_wImpl->wAttribs.Height))
        {
            _wImpl->eStack.PushPointerMovedEvent(Point.x, Point.y);

            if (!WinInput::InWindow)
            {
                SetCapture(hWnd);
                
                WinInput::InWindow = true;
                _wImpl->eStack.PushPointerInEvent();
            }
            break;
        }
        else
        {
            ReleaseCapture();
            
            WinInput::InWindow = false;
            _wImpl->eStack.PushPointerOutEvent();
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

    IWindow::Attributes* Attribs = &_wImpl->wAttribs;

    DWORD dwStyle = WS_VISIBLE;

    if (Attribs->aFlags & IWindow::Flags::WindowFullScreen)
    {
        HMONITOR hmon = MonitorFromPoint({ (LONG)Attribs->X, (LONG)Attribs->Y }, MONITOR_DEFAULTTOPRIMARY);
        MONITORINFO mi = { sizeof(mi) };

        if (GetMonitorInfo(hmon, &mi))
        {
            Attribs->X = mi.rcMonitor.left;
            Attribs->Y = mi.rcMonitor.top;
            Attribs->Width = mi.rcMonitor.right - mi.rcMonitor.left;
            Attribs->Height = mi.rcMonitor.bottom - mi.rcMonitor.top;
        }

        dwStyle |= WS_POPUP;
    }
    else
    {
        dwStyle |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    }

    if (!(Attribs->aFlags & IWindow::Flags::NoResizing))
    {
        dwStyle |= WS_SIZEBOX;
    }

    _wImpl->Window = CreateWindowExA(
        WS_EX_TOPMOST,
        "KotWinWindow", Attribs->Title,
        dwStyle,
        Attribs->X, Attribs->Y,
        Attribs->Width, Attribs->Height,
        nullptr, nullptr, _wImpl->Instance, this
    );

    if (Attribs->aFlags & IWindow::Flags::BorderlessWindow)
    {
        // SetWindowLong(_wImpl->Window,
        //  GWL_STYLE, dwStyle ^ WS_CAPTION);
    }

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

    if (Attribs->aFlags & IWindow::Flags::WindowMaximized)
    {
        SendMessage(_wImpl->Window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    }

    _wImpl->eStack.PushWindowCreatedEvent();
}

void IWinWindow::DestroyWindow()
{
    UnregisterClass("KotWinWindow", _wImpl->Instance);
    wDestroyWindow(_wImpl->Window);
}

IWinWindow::IWinWindow(const IWindow::Attributes& _Attributes)
{
    _wImpl = new __IWinImpl;
    _wImpl->wAttribs = _Attributes;
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
        break;
    }
    case CursorMode::Locked:
        break;
    default: break;
    }
}

void IWinWindow::SetCursorState(const CursorState& _Cursor)
{
    switch (_Cursor)
    {
    case CursorState::Shown:
    {
        while (ShowCursor(TRUE) < 0);
        break;
    }
    case CursorState::Hidden:
    {
        while (ShowCursor(FALSE) >= 0);
        break;
    }
    default: break;
    }
}

unsigned IWinWindow::GetPlatform() const
{
    return IWindow::Platform::Windows;
}

DisplayScreen* IWinWindow::GetDisplayData() const
{
    return &_wImpl->dScreen;
}

void* IWinWindow::SetNativeData(void* _Data) { return _Data; }
void* IWinWindow::GetNativeData() const { return nullptr; }

WEvent IWinWindow::Event() const
{
    return _wImpl->eStack.PopEvent();
}

bool IWinWindow::IsEvent() const
{
    return _wImpl->eStack.IsEventStackNotEmpty();
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