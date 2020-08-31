#ifndef _KOT_EVENT_STACK_
#define _KOT_EVENT_STACK_

#include <Events.hpp>
#include <stack>

typedef class __UEventStack
{
private:
	std::stack<WEvent> Stack;
	WEvent eLast;

	int pX, pY;

public:

	__UEventStack();
	~__UEventStack();

	bool IsEventStackNotEmpty() const;

	void PushEvent(const WEvent& _Event);
	WEvent PopEvent();

	void PushWindowCreatedEvent();
	void PushWindowCloseEvent();
	void PushWindowExposedEvent();
	void PushWindowMinimizedEvent();
	void PushWindowMaximizedEvent();

	void PushPointerInEvent();
	void PushPointerOutEvent();

	void PushCharacterInputEvent(char& _Character);

	void PushKeyInputEvent(Keys& _Key, KeyAction& _Action);

	void PushButtonInputEvent(
		Buttons& _Button, ButtonAction& _Action,
		int& _PointerX, int& _PointerY
	);

	void PushMouseScrollEvent(
		float& _Delta, Scroll& _Direction,
		int& _PointerX, int& _PointerY
	);

	void PushPointerMovedEvent(int& _PointerX, int& _PointerY);

	void PushWindowChangedEvent(
		int& _X, int& _Y,
		int& _Width, int& _Height
	);

}EventStack;

#endif