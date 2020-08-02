#ifndef _KOT_X11_VULKAN_INTERFACE_
#define _KOT_X11_VULKAN_INTERFACE_

#include <Vulkan.hpp>

class IX11Vulkan : public IVulkan
{
private:
    void CreateSurface();
    void DestroySurface();

public:
    IX11Vulkan(const IVulkan::Attributes& _Attributes);
    virtual ~IX11Vulkan() override;

    virtual vk::SurfaceKHR* GetKHRSurface() override;

};

#endif