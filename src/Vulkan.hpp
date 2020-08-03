#ifndef _KOT_VULKAN_INTERFACE_
#define _KOT_VULKAN_INTERFACE_

#include <vulkan/vulkan.h>
#include "Window.hpp"

typedef class __IVkInterface
{
protected:
    // Platform Implementation
    struct __IVkImpl* _vkImpl;
    unsigned _vkResult;

public:
    virtual ~__IVkInterface() = default;
    virtual VkSurfaceKHR CreateKHRSurface(const VkInstance& _VkInstance) = 0;

public:
    static __IVkInterface* Create(IWindow* _Window);

}IVulkan;

#endif