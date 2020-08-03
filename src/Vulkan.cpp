#include <Vulkan.hpp>
#include "X11/X11Vulkan.hpp"

__IVkInterface* __IVkInterface::Create(IWindow** _Window)
{
    return new IX11Vulkan(_Window);
}