#include "EventStack.hpp"

EventStack::__UEventStack()
	: eLast({ WEventType::NoEvent }),
	  pX(0), pY(0)
{
}

EventStack::~__UEventStack()
{
}

bool EventStack::IsEventStackNotEmpty() const
{
	return !(Stack.empty());
}

void EventStack::PushEvent(const WEvent& _Event)
{
	eLast = _Event;
	Stack.push(_Event);
}

WEvent EventStack::PopEvent()
{
	WEvent& eTop = Stack.top();
	Stack.pop();
	return eTop;
}

void EventStack::PushWindowCreatedEvent()
{
	PushEvent({ WEventType::WindowCreated });
}

void EventStack::PushWindowCloseEvent()
{
	PushEvent({ WEventType::WindowClosed });
}

void EventStack::PushWindowExposedEvent()
{
	PushEvent({ WEventType::WindowExposed });
}

void EventStack::PushWindowMinimizedEvent()
{
	PushEvent({ WEventType::WindowMinimized });
}

void EventStack::PushWindowMaximizedEvent()
{
	PushEvent({ WEventType::WindowMaximized });
}

void EventStack::PushPointerInEvent()
{
	PushEvent({ WEventType::PointerIn });
}

void EventStack::PushPointerOutEvent()
{
	PushEvent({ WEventType::PointerOut });
}

void EventStack::PushCharacterInputEvent(char& _Character)
{
	WEvent CharEvent = { WEventType::CharEvent };
	CharEvent.eChar = _Character;

	PushEvent(CharEvent);
}

void EventStack::PushKeyInputEvent(Keys& _Key, KeyAction& _Action)
{
	WEvent KeyEvent = { WEventType::KeyEvent };
	KeyEvent.eKey.Code = _Key;
	KeyEvent.eKey.Action = _Action;

	PushEvent(KeyEvent);
}

void EventStack::PushButtonInputEvent(Buttons& _Button, ButtonAction& _Action, int& _PointerX, int& _PointerY)
{
	WEvent ButtonEvent = { WEventType::KeyEvent };
	ButtonEvent.eButton.Code = _Button;
	ButtonEvent.eButton.Action = _Action;
	ButtonEvent.eButton.PointerX = _PointerX;
	ButtonEvent.eButton.PointerY = _PointerY;

	PushEvent(ButtonEvent);
}

void EventStack::PushMouseScrollEvent(float& _Delta, Scroll& _Direction, int& _PointerX, int& _PointerY)
{
	WEvent ScrollEvent = { WEventType::ScrollEvent };

	ScrollEvent.eScroll.Delta = _Delta;
	ScrollEvent.eScroll.AbsDelta = abs(_Delta);

	ScrollEvent.eScroll.Direction = _Direction;

	switch (_Direction)
	{
	case Scroll::ScrollUp:
	case Scroll::ScrollDown:
		ScrollEvent.eScroll.Axis = ScrollAxis::ScrollVertical;
		break;
	case Scroll::ScrollLeft:
	case Scroll::ScrollRight:
		ScrollEvent.eScroll.Axis = ScrollAxis::ScrollHorizontal;
		break;
	default:
		ScrollEvent.eScroll.Axis = ScrollAxis::NoAxis;
		break;
	}

	ScrollEvent.eScroll.PointerX = _PointerX;
	ScrollEvent.eScroll.PointerY = _PointerY;

	PushEvent(ScrollEvent);
}

void EventStack::PushPointerMovedEvent(int& _PointerX, int& _PointerY)
{
	WEvent PointerMovedEvent = { WEventType::PointerMoved };

	PointerMovedEvent.ePMoved.PointerX = _PointerX;
	PointerMovedEvent.ePMoved.PointerY = _PointerY;

	PointerMovedEvent.ePMoved.DeltaX = pX - _PointerX;
	PointerMovedEvent.ePMoved.DeltaY = pY - _PointerY;

	PushEvent(PointerMovedEvent);
}

void EventStack::PushWindowChangedEvent(int& _X, int& _Y, int& _Width, int& _Height)
{
	WEvent WindowChangedEvent = { WEventType::WindowChanged };

	WindowChangedEvent.eWChanged.X = _X;
	WindowChangedEvent.eWChanged.Y = _Y;

	WindowChangedEvent.eWChanged.Width = _Width;
	WindowChangedEvent.eWChanged.Height = _Height;

	PushEvent(WindowChangedEvent);
}