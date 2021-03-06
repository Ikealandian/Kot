#include "X11Vulkan.hpp"
#include "X11Window.hpp"
#include "X11Impl.hpp"

#include <stdio.h>
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>

IX11Vulkan::IX11Vulkan(IWindow** _Window)
{
    _vkImpl = new __IVkImpl;
    _vkImpl->_pIWindow = *_Window;
}

IX11Vulkan::~IX11Vulkan()
{
    _vkImpl->_pIWindow = nullptr;
    delete _vkImpl;
}

std::vector<const char*> IX11Vulkan::GetRequiredExtentions() const
{
    return 
    {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME
    };
}

VkSurfaceKHR IX11Vulkan::CreateKHRSurface(const VkInstance& _VkInstance)
{
    VkXlibSurfaceCreateInfoKHR vkxsfci = {};
    vkxsfci.sType   = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    vkxsfci.pNext   = nullptr;
    vkxsfci.window  = _vkImpl->_pIWindow->_wImpl->xWindow;
    vkxsfci.dpy     = _vkImpl->_pIWindow->_wImpl->xDisplay;

    VkSurfaceKHR vkskhr;
    if (vkCreateXlibSurfaceKHR(_VkInstance, &vkxsfci, nullptr, &vkskhr) != VK_SUCCESS)
    {
        fputs("IVulkan [IX11Vulkan]: vkCreateXlibSurfaceKHR Failed...", stderr);
        return nullptr;
    }

    return vkskhr;
}