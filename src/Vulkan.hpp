#ifndef _KOT_VULKAN_INTERFACE_
#define _KOT_VULKAN_INTERFACE_

#include <vulkan/vulkan.hpp>
#include "Window.hpp"

typedef class __IVkInterface
{
public:
    typedef struct 
    {
        IWindow* Window;
    }Attributes;

private:
    // Platform Implementation
    struct __IVkImpl* _vkImpl;
    unsigned _vkResult;

public:
    virtual ~__IVkInterface() = default;

    virtual vk::SurfaceKHR* GetKHRSurface() = 0;

public:
    static __IVkInterface* Create(const __IVkInterface::Attributes& _Attributes);

}IVulkan;

#endif