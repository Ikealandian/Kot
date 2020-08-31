#include "EventStack.hpp"

__UEventStack::__UEventStack()
	: eLast({ WEventType::NoEvent }),
	  pX(0), pY(0)
{
}

__UEventStack::~__UEventStack()
{
}

bool __UEventStack::IsEventStackNotEmpty() const
{
	return !(Stack.empty());
}

void __UEventStack::PushEvent(const WEvent& _Event)
{
	eLast = _Event;
	Stack.push(_Event);
}

WEvent __UEventStack::PopEvent()
{
	WEvent& eTop = Stack.top();
	Stack.pop();
	return eTop;
}

void __UEventStack::PushWindowCreatedEvent()
{
	PushEvent({ WEventType::WindowCreated });
}

void __UEventStack::PushWindowCloseEvent()
{
	PushEvent({ WEventType::WindowClosed });
}

void __UEventStack::PushWindowExposedEvent()
{
	PushEvent({ WEventType::WindowExposed });
}

void __UEventStack::PushWindowMinimizedEvent()
{
	PushEvent({ WEventType::WindowMinimized });
}

void __UEventStack::PushWindowMaximizedEvent()
{
	PushEvent({ WEventType::WindowMaximized });
}

void __UEventStack::PushPointerInEvent()
{
	PushEvent({ WEventType::PointerIn });
}

void __UEventStack::PushPointerOutEvent()
{
	PushEvent({ WEventType::PointerOut });
}

void __UEventStack::PushCharacterInputEvent(char& _Character)
{
	WEvent CharEvent = { WEventType::CharEvent };
	CharEvent.eChar = _Character;

	PushEvent(CharEvent);
}

void __UEventStack::PushKeyInputEvent(Keys& _Key, KeyAction& _Action)
{
	WEvent KeyEvent = { WEventType::KeyEvent };
	KeyEvent.eKey.Code = _Key;
	KeyEvent.eKey.Action = _Action;

	PushEvent(KeyEvent);
}

void __UEventStack::PushButtonInputEvent(Buttons& _Button, ButtonAction& _Action, int& _PointerX, int& _PointerY)
{
	WEvent ButtonEvent = { WEventType::KeyEvent };
	ButtonEvent.eButton.Code = _Button;
	ButtonEvent.eButton.Action = _Action;
	ButtonEvent.eButton.PointerX = _PointerX;
	ButtonEvent.eButton.PointerY = _PointerY;

	PushEvent(ButtonEvent);
}

void __UEventStack::PushMouseScrollEvent(float& _Delta, Scroll& _Direction, int& _PointerX, int& _PointerY)
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

void __UEventStack::PushPointerMovedEvent(int& _PointerX, int& _PointerY)
{
	WEvent PointerMovedEvent = { WEventType::PointerMoved };

	PointerMovedEvent.ePMoved.PointerX = _PointerX;
	PointerMovedEvent.ePMoved.PointerY = _PointerY;

	PointerMovedEvent.ePMoved.DeltaX = pX - _PointerX;
	PointerMovedEvent.ePMoved.DeltaY = pY - _PointerY;

	PushEvent(PointerMovedEvent);
}

void __UEventStack::PushWindowChangedEvent(int& _X, int& _Y, int& _Width, int& _Height)
{
	WEvent WindowChangedEvent = { WEventType::WindowChanged };

	WindowChangedEvent.eWChanged.X = _X;
	WindowChangedEvent.eWChanged.Y = _Y;

	WindowChangedEvent.eWChanged.Width = _Width;
	WindowChangedEvent.eWChanged.Height = _Height;

	PushEvent(WindowChangedEvent);
}