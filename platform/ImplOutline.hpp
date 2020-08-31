#ifndef _KOT_WINDOW_IMPLEMENTATION_OUTLINE_
#define _KOT_WINDOW_IMPLEMENTATION_OUTLINE_

#include "EventStack.hpp"
#include <Window.hpp>

typedef struct __WImplOutline
{
	IWindow::Attributes wAttribs;
	DisplayScreen dScreen;
	EventStack eStack;
}ImplOutline;

#endif