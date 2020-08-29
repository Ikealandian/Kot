#ifndef _KOT_WINDOW_INTERFACE_
#define _KOT_WINDOW_INTERFACE_

#include <Events.hpp>

// Current Display/Screen Data
typedef struct __IWinDisplay
{
    unsigned Width, Height;
}DisplayScreen;

// Window Interface
typedef class __IWinInterface
{
public:
    // OS Platform
    enum Platform : unsigned
    {
        Linux,
        Windows,
        Mach,
    };

    // Cursor Mode
    enum struct CursorMode
    {
        Free,
        Confined,
        Locked
    };

    // Cursor State
    enum struct CursorState
    {
        Shown,
        Hidden
    };

    // Window Flags
    enum Flags : unsigned
    {
        PositionCentered        = (1 << 0),
        NoResizing              = (1 << 1),
        WindowFullScreen        = (1 << 2),
        WindowMaximized         = (1 << 3),
        BorderlessWindow        = (1 << 4)
    };

    // Window Attributes
    typedef struct
    {
        const char* Title;
        unsigned Width, Height;
        unsigned X, Y;
        unsigned aFlags;
    }Attributes;

protected:
    // Friends
    friend class IX11Vulkan;
    friend class IWinVulkan;

    // Platform Implementation
    struct __IWinImpl* _wImpl;

public:
    virtual ~__IWinInterface() = default;

    virtual unsigned GetPlatform() const = 0;
    virtual DisplayScreen* GetDisplayData() const = 0;
    
    virtual void SetCursorMode(const CursorMode& _Cursor) = 0;
    virtual void SetCursorState(const CursorState& _Cursor) = 0;

    virtual void* SetNativeData(void* _Data) = 0;
    virtual void* GetNativeData() const = 0;

    virtual WEvent Event() const = 0;
    virtual bool IsEvent() const = 0;

    virtual void Update() = 0;

public:
    // Create Window for current plaform
    static __IWinInterface* Create(const Attributes& _Attributes);

}IWindow;

#endif