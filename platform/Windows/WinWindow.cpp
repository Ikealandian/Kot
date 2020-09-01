#include "WinWindow.hpp"
#include "WindowsInput.hpp"

#include <windowsx.h>

#define wDestroyWindow WINUSERAPI BOOL WINAPI DestroyWindow(_In_ HWND hWnd);

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
    _wImpl->eStack.ClearStack();

    MSG mTempMsg;
    while (PeekMessage(&mTempMsg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&mTempMsg);
        DispatchMessage(&mTempMsg);
    }

    if (_wImpl->uCallback)
        _wImpl->uCallback();
}