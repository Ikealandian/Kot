#ifndef _KOT_WIN_WINDOW_INTERFACE_
#define _KOT_WIN_WINDOW_INTERFACE_

#include "Window.hpp"
#include "Events.hpp"

#include "WindowsImpl.hpp"

#include <string.h>
#include <stdio.h>
#include <vector>
#include <stack>
#include <map>

#include <Windows.h>
#undef CreateWindow

class IWinWindow : public IWindow
{
private:
    void CreateWindow();
    void DestroyWindow();

    static LRESULT CALLBACK SetupWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK HandleMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    IWinWindow(const IWindow::Attributes&);
    virtual ~IWinWindow() override;

    virtual unsigned GetPlatform() const override;
    virtual DisplayScreen* GetDisplayData() const override;

    virtual void SetCursorMode(const WCursorMode& _Cursor) override;
    virtual void SetCursorState(const WCursorState& _Cursor) override;

    virtual void* SetNativeData(void* _Data) override;
    virtual void* GetNativeData() const override;

    virtual WEvent Event() const override;
    virtual bool IsEvent() const override;

    virtual void Update() override;

};

#endif