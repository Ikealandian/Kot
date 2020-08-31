#ifndef _KOT_X11_IMPLEMENTATION_
#define _KOT_X11_IMPLEMENTATION_

#include <ImplOutline.hpp>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xinerama.h>
#include <X11/XKBlib.h>
#include <X11/cursorfont.h>

struct __IWinImpl : public ImplOutline
{
    Display* xDisplay;
    Window xWindow;
    Window xRoot;
    XEvent xEvent;
    Cursor xCursor;
    int iScreen;

    Atom WMDeleteWindow;
    Atom WMState;
    Atom WMNetState;
    Atom WMNetStateFullscreen;
    Atom WMNetStateHidden;
    Atom WMNetActionMinimise;
    Atom WMNetMaximizedVertical;
    Atom WMNetMaximizedHorizontal;
};

#endif