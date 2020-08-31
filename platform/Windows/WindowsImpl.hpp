#ifndef _KOT_WINDOWS_IMPLEMENTATION_
#define _KOT_WINDOWS_IMPLEMENTATION_

#include <ImplOutline.hpp>
#include <Windows.h>

struct __IWinImpl : public ImplOutline
{
    HWND Window = nullptr;
    HINSTANCE Instance = nullptr;
};

#endif