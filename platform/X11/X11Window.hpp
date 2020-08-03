#ifndef _KOT_X11_WINDOW_INTERFACE_
#define _KOT_X11_WINDOW_INTERFACE_

#include "Window.hpp"

#include "Events.hpp"

#include <string.h>
#include <stdio.h>
#include <vector>
#include <stack>
#include <map>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

struct __IWinImpl
{
    IWindow::Attributes _Attributes;
    DisplayScreen _Display;

    std::stack<IEvent> EventStack;

    Display* xDisplay;
    Window xWindow;
    Window xRoot;
    XEvent xEvent;
    int iScreen;

    Atom WMDeleteWindow;
};

class IX11Window : public IWindow
{
private:
    void CreateWindow();
    void DestroyWindow();

public:
    IX11Window(const IWindow::Attributes&);
    virtual ~IX11Window() override;
    
    virtual unsigned GetPlatform() const override;
    virtual DisplayScreen* GetDisplayData() const override;
    
    virtual void* SetNativeData(void* _Data) override;
    virtual void* GetNativeData() const override;

    virtual IEvent Event() const override;
    virtual bool IsEvent() const override;

    virtual void Update() override;

};

#endif