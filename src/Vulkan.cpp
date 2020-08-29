#include "Platform.hpp"
#include "Vulkan.hpp"

#ifdef _KOT_PLATFORM_LINUX
#   include "X11/X11Vulkan.hpp"
using PlatformVulkan = IX11Vulkan;
#endif

__IVkInterface* __IVkInterface::Create(IWindow** _Window)
{
    return new PlatformVulkan(_Window);
}