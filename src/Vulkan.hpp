#ifndef _KOT_VULKAN_INTERFACE_
#define _KOT_VULKAN_INTERFACE_

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

}IVulkan;

#endif