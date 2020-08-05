#include <Window.hpp>
#include <Vulkan.hpp>

#include <stdio.h>

int main()
{
    IWindow::Attributes Attribs;
    Attribs.Width = 800;
    Attribs.Height = 600;
    Attribs.aFlags = IWindow::Flags::PositionCentered | IWindow::Flags::NoResizing;

    IWindow* X11Win = IWindow::Create(Attribs);
    IVulkan* X11Vk  = IVulkan::Create(&X11Win);

    VkApplicationInfo vkAppInfo             {};
    vkAppInfo.sType                         = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vkAppInfo.pNext                         = nullptr;
    vkAppInfo.pApplicationName              = "Kot Vulkan Test";
    vkAppInfo.applicationVersion            = VK_MAKE_VERSION(0, 0, 0);
    vkAppInfo.pEngineName                   = "Kot";
    vkAppInfo.engineVersion                 = VK_MAKE_VERSION(0, 0, 0);
    vkAppInfo.apiVersion                    = VK_API_VERSION_1_2;

    std::vector<const char*> vkiExtensions  = X11Vk->GetRequiredExtentions();
    std::vector<const char*> vkiLayers      = 
    {
        "VK_LAYER_KHRONOS_validation"
    };

    VkInstanceCreateInfo VkInstanceInfo     {};
    VkInstanceInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    VkInstanceInfo.pNext                    = nullptr;
    VkInstanceInfo.pApplicationInfo         = &vkAppInfo;
    VkInstanceInfo.enabledExtensionCount    = vkiExtensions.size();
    VkInstanceInfo.ppEnabledExtensionNames  = vkiExtensions.data();
    VkInstanceInfo.enabledLayerCount        = vkiLayers.size();
    VkInstanceInfo.ppEnabledLayerNames      = vkiLayers.data();

    VkInstance vkInstance;
    if (vkCreateInstance(&VkInstanceInfo, nullptr, &vkInstance) != VK_SUCCESS)
    {
        puts("vkCreateInstance Failed...");
        return -1;
    }

    uint32_t vkExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);

    std::vector<VkExtensionProperties> vkExtensions(vkExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, vkExtensions.data());

    for (const auto& vkExtension : vkExtensions)
        puts(vkExtension.extensionName);

    VkSurfaceKHR vkSurface = X11Vk->CreateKHRSurface(vkInstance);

    bool Running = true;

    while (Running)
    {
        X11Win->Update();
        while (X11Win->IsEvent())
        {
            IEvent Event = X11Win->Event();
            switch (Event.Type)
            {
            case IEventType::ButtonEvent:
            {
                if (Event.eButton.Action == ButtonAction::Pressed)
                    printf("Button Pressed: %d\nLocation:\nX: %d\nY: %d\n",
                        (int)Event.eButton.Code,
                        Event.eButton.PointerX,
                        Event.eButton.PointerY
                    );
                break;
            }
            case IEventType::CharEvent:
                printf("%c : %d\n", Event.eChar, Event.eChar);
                break;
            case IEventType::KeyEvent:
            {
                if (Event.eKey.Code == Keys::Escape)
                    Running = false;
                break;
            }
            case IEventType::WindowClosed:
                // Break Loop
                Running = false;
                break;
            default:
                break;
            }
        }
    }

    vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
    vkDestroyInstance(vkInstance, nullptr);

    delete X11Vk;
    delete X11Win;
    return 0;
}