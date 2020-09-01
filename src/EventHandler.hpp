#ifndef _KOT_EVENT_HANDLER_
#define _KOT_EVENT_HANDLER_

#include <Window.hpp>
#include <Events.hpp>

#include <map>

typedef bool (*EventCallback)(const WEvent&);
typedef class __UEventHandler
{
private:
	IWindow* Window;
	std::map<WEventType, EventCallback> Callbacks;

	void OnWindowUpdateEnd(void);

public:

	__UEventHandler(IWindow** _Window);
	~__UEventHandler();

	void AddCallback(const WEventType& _Event, const EventCallback& _Callback);

}WEventHandler;

#endif