#include <EventHandler.hpp>
#include <ImplOutline.hpp>
#include "EventHandler.hpp"

#include <functional>

struct __IWinImpl : public ImplOutline {};

void __UEventHandler::OnWindowUpdateEnd(void)
{
	EventStack StackCopy = Window->_wImpl->eStack;
	long Position = 0;

	while (StackCopy.IsEventStackNotEmpty())
	{
		WEvent Event = StackCopy.PopEvent();
		if (Callbacks.find(Event.Type) != Callbacks.end())
		{
			Window->_wImpl->eStack.EraseEvent(Position);
			if (Callbacks[Event.Type](Event))
				return;
		}
		Position++;
	}
}

__UEventHandler::__UEventHandler(IWindow** _Window)
{
	Window = *_Window;
	Window->_wImpl->uCallback = std::bind(&__UEventHandler::OnWindowUpdateEnd, this);
}

__UEventHandler::~__UEventHandler()
{
}

void __UEventHandler::AddCallback(const WEventType& _Event, const EventCallback& _Callback)
{
	Callbacks[_Event] = _Callback;
}