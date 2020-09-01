#include <Window.hpp>
#include <EventHandler.hpp>

#include <stdio.h>
#ifdef _WIN32
#   include <crtdbg.h>
#endif

bool g_Running = true;

bool WindowClosedEvent(const WEvent& _Event)
{
    g_Running = false;
    return true; // Stop looking through events
}

bool ButtonEvent(const WEvent& _Event)
{
    puts("Button Event");
    return false; // Continue looking through events
}

int main()
{
#   ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#   endif

    IWindow::Attributes Attribs;
    Attribs.Title = "Window Test";
    Attribs.X = 0;
    Attribs.Y = 0;
    Attribs.Width = 800;
    Attribs.Height = 600;
    Attribs.aFlags = WFlags::NoResizing | WFlags::PositionCentered;

	IWindow* Window = IWindow::Create(Attribs);
    WEventHandler Handler(&Window);

    // Callbacks
    Handler.AddCallback(WEventType::WindowClosed, WindowClosedEvent);
    Handler.AddCallback(WEventType::ButtonEvent, ButtonEvent);

    while (g_Running)
    {
        Window->Update();

        // Can still look through events manually
        while (Window->IsEvent())
        {
            WEvent Event = Window->Event();
            if (Event.Type == WEventType::CharEvent)
            {
                putchar(Event.eChar);
            }
        }
    }

	delete Window;
	return 0;
}