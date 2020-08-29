#include "Platform.hpp"
#include "Window.hpp"

#ifdef _KOT_PLATFORM_LINUX
#   include "X11/X11Window.hpp"
using PlatformWindow = IX11Window;
#endif

#ifdef _KOT_PLATFORM_WINDOWS
#   include "Windows/WinWindow.hpp"
using PlatformWindow = IWinWindow;
#endif

__IWinInterface* __IWinInterface::Create(const __IWinInterface::Attributes& _Attributes)
{
    return new PlatformWindow(_Attributes);
}