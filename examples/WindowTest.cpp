#include <Window.hpp>
#include <stdio.h>

int main()
{
	IWindow::Attributes Attribs;
    Attribs.Title = "Window Test";
    Attribs.X = 0;
    Attribs.Y = 0;
	Attribs.Width = 800;
	Attribs.Height = 600;
	Attribs.aFlags = IWindow::Flags::NoResizing | IWindow::Flags::PositionCentered;

	IWindow* Window = IWindow::Create(Attribs);

    Window->SetCursorMode(IWindow::CursorMode::Confined);

	bool Running = true;

    while (Running)
    {
        Window->Update();
        while (Window->IsEvent())
        {
            WEvent Event = Window->Event();
            switch (Event.Type)
            {
            case WEventType::WindowClosed:
                // Break Loop
                Running = false;
                break;
            case WEventType::ButtonEvent:
                printf("Button: %d:%d\n X: %d\n Y: %d\n",
                    (int)Event.eButton.Code,
                    (int)Event.eButton.Action,
                    Event.eButton.PointerX,
                    Event.eButton.PointerY);
                break;
            case WEventType::ScrollEvent:
                printf("Scroll: %d\n X: %d\n Y: %d\n",
                    (int)Event.eScroll.Delta,
                    Event.eScroll.PointerX,
                    Event.eScroll.PointerY);
                break;
            default:
                break;
            }
        }
    }

	delete Window;
	return 0;
}