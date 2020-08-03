#ifndef _KOT_VULKAN_INTERFACE_
#define _KOT_VULKAN_INTERFACE_

#include "Window.hpp"

#include <vector>
#include <vulkan/vulkan.h>

typedef class __IVkInterface
{
protected:
    // Platform Implementation
    struct __IVkImpl* _vkImpl;
    unsigned _vkResult;

public:
    virtual ~__IVkInterface() = default;

    virtual std::vector<const char*> GetRequiredExtentions() const = 0;
    virtual VkSurfaceKHR CreateKHRSurface(const VkInstance& _VkInstance) = 0;

public:
    static __IVkInterface* Create(IWindow** _Window);

}IVulkan;

#endif