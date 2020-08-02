#include "Window.hpp"
#include "X11/X11Window.hpp"

__IWinInterface* __IWinInterface::Create(const __IWinInterface::Attributes& _Attributes)
{
    return new IX11Window(_Attributes);
}