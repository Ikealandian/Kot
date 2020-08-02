// inc/Window.hpp

// platform/X11/Window.hpp
// platform/Windows/Window.hpp

#ifndef _KOT_WINDOW_INTERFACE_
#define _KOT_WINDOW_INTERFACE_

#include <memory>
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

    // Window Flags
    enum Flags : unsigned
    {
        PositionCentered        = (1 << 0),
    };

    // Window Attributes
    typedef struct
    {
        unsigned Width, Height;
        unsigned X, Y;
        unsigned aFlags;
    }Attributes;

protected:
    // Platform Implementation
    struct __IWinImpl* _wImpl;
    unsigned _wResult;

public:
    virtual ~__IWinInterface() = default;

    virtual unsigned GetPlatform() const = 0;
    virtual DisplayScreen* GetDisplayData() const = 0;
    
    virtual void* SetNativeData(void* _Data) = 0;
    virtual void* GetNativeData() const = 0;

    virtual IEvent Event() const = 0;
    virtual bool IsEvent() const = 0;

    virtual void Update() = 0;

public:
    // Create Window for current plaform
    static __IWinInterface* Create(const __IWinInterface::Attributes& _Attributes);

}IWindow;

#endif