#include "WinWindow.hpp"

#define ptt_t std::pair<int, int>
#define wDestroyWindow WINUSERAPI BOOL WINAPI DestroyWindow(_In_ HWND hWnd);

typedef struct __WinInput
{
    static int LastCX, LastCY;
    static bool InWindow;

    static void SaveCursorInfo(const ptt_t& _Cursor)
    {
        LastCX = _Cursor.first;
        LastCY = _Cursor.second;
    }

    static ptt_t GetCursorDelta(const ptt_t& _Cursor)
    {
        return { LastCX - _Cursor.first, LastCY - _Cursor.second };
    }

}WinInput;

int WinInput::LastCX,
    WinInput::LastCY;

bool WinInput::InWindow;

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
    // KeyEvent
    // ButtonEvent
    // 
    // CharEvent

    switch (msg)
    {

    // Window Close Event
    case WM_CLOSE:
    {                
        WEvent CloseEvent;
        CloseEvent.Type = WEventType::WindowClosed;

        _wImpl->EventStack.push(CloseEvent);
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

    case WM_MOUSELEAVE:
    {
        WinInput::InWindow = false;
        WEvent PointerNotify;
        PointerNotify.Type = WEventType::PointerOut;

        _wImpl->EventStack.push(PointerNotify);
        break;
    }

    case WM_MOUSEMOVE:
    {
        const POINTS pt = MAKEPOINTS(lParam);

        if (pt.x >= 0 && pt.x < _wImpl->_Attributes.Width
            && pt.y >= 0 && pt.y < _wImpl->_Attributes.Height)
        {
            WEvent PMovedEvent;
            PMovedEvent.Type = WEventType::PointerMoved;

            ptt_t CursorPosition =
            {
                pt.x, pt.y
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

    if (Attribs->aFlags & IWindow::Flags::PositionCentered)
    {        
        HWND wParent = GetParent(_wImpl->Window);

        RECT wrParentSize;
        GetWindowRect(wParent, &wrParentSize);

        wrParentSize.bottom /= 2;
        wrParentSize.right /= 2;

        Attribs->X = wrParentSize.right - (Attribs->Width / 2);
        Attribs->Y = wrParentSize.bottom - (Attribs->Width / 2);

        MoveWindow(_wImpl->Window, Attribs->X, Attribs->Y, Attribs->Width, Attribs->Height, FALSE);
    }

    ShowWindow(_wImpl->Window, SW_SHOW);
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