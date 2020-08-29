#include <Window.hpp>

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
            default:
                break;
            }
        }
    }

	delete Window;
	return 0;
}