#ifndef _KOT_WINDOW_IMPLEMENTATION_OUTLINE_
#define _KOT_WINDOW_IMPLEMENTATION_OUTLINE_

#include "EventStack.hpp"
#include <Window.hpp>

#include <functional>

typedef std::function<void(void)> IWindowUpdateEndCallback;

typedef struct __WImplOutline
{
	IWindow::Attributes wAttribs = {};
	DisplayScreen dScreen = {};
	EventStack eStack;

	IWindowUpdateEndCallback uCallback;
}ImplOutline;

#endif