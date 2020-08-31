#ifndef _KOT_X11_WINDOW_INTERFACE_
#define _KOT_X11_WINDOW_INTERFACE_

#include "Window.hpp"

#include "Events.hpp"

#include <string.h>
#include <stdio.h>
#include <vector>
#include <stack>
#include <map>

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
    
    virtual void SetCursorMode(const CursorMode& _Cursor) override;
    virtual void SetCursorState(const CursorState& _Cursor) override;

    virtual void* SetNativeData(void* _Data) override;
    virtual void* GetNativeData() const override;

    virtual WEvent Event() const override;
    virtual bool IsEvent() const override;

    virtual void Update() override;

};

#endif