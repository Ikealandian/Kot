#ifndef _KOT_LINUX_WINDOW_INTERFACE_
#define _KOT_LINUX_WINDOW_INTERFACE_

#include "Window.hpp"

class IX11Window : public __IWinInterface
{
private:
    void CreateWindow();
    void DestroyWindow();

public:
    IX11Window(const __IWinInterface::Attributes&);
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