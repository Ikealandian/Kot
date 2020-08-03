#ifndef _KOT_X11_VULKAN_INTERFACE_
#define _KOT_X11_VULKAN_INTERFACE_

#include "Vulkan.hpp"

struct __IVkImpl
{
    IWindow* _pIWindow;
};

class IX11Vulkan : public IVulkan
{
public:
    IX11Vulkan(IWindow** _Window);
    virtual ~IX11Vulkan() override;

    virtual std::vector<const char*> GetRequiredExtentions() const override;
    virtual VkSurfaceKHR CreateKHRSurface(const VkInstance& _VkInstance) override;
};

#endif