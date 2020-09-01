#ifndef _KOT_EVENT_STACK_
#define _KOT_EVENT_STACK_

#include <Events.hpp>
#include <vector>

typedef class __UEventStack
{
private:
	std::vector<WEvent> Stack;
	WEvent eLast;

	int pX, pY;

public:

	__UEventStack();
	~__UEventStack();

	bool IsEventStackNotEmpty() const;

	void ClearStack();
	
	void PushEvent(const WEvent& _Event);
	WEvent PopEvent();

	WEvent PeekEvent(long _Position = 0) const;
	WEvent& PokeEvent(long _Position = 0);
	void EraseEvent(long _Position = 0);

	void PushWindowCreatedEvent();
	void PushWindowCloseEvent();
	void PushWindowExposedEvent();
	void PushWindowMinimizedEvent();
	void PushWindowMaximizedEvent();

	void PushPointerInEvent();
	void PushPointerOutEvent();

	void PushCharacterInputEvent(const char32_t& _Character);

	void PushKeyInputEvent(const Keys& _Key, const KeyAction& _Action);

	void PushButtonInputEvent(
		const Buttons& _Button, const ButtonAction& _Action,
		const long& _PointerX, const long& _PointerY
	);

	void PushMouseScrollEvent(
		const float& _Delta, const ScrollAxis& _Axis,
		const long& _PointerX, const long& _PointerY
	);

	void PushPointerMovedEvent(const long& _PointerX, const long& _PointerY);

	void PushWindowChangedEvent(
		const long& _X, const long& _Y,
		const long& _Width, const long& _Height
	);

}EventStack;

#endif