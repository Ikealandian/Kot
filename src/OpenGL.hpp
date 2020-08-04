#ifndef _KOT_OPENGL_INTERFACE_
#define _KOT_OPENGL_INTERFACE_

#include "Window.hpp"

// Resources:
//  https://stackoverflow.com/questions/12856820/setup-opengl-on-x11
//  https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)

// TODO: X11 OpenGL
typedef class __IGLInterface
{
protected:
    // Platform Implementation
    struct __IVkImpl* _glImpl;
    unsigned _glResult;

public:
    virtual ~__IGLInterface() = default;

    virtual void MakeContextCurrent();

public:
    static __IGLInterface* Create(IWindow** _Window);

}IOpenGL;

#endif