#include <Window.hpp>
#include <stdio.h>

int main()
{
    IWindow::Attributes Attribs;
    Attribs.Width = 800;
    Attribs.Height = 600;
    Attribs.aFlags = IWindow::Flags::PositionCentered;

    IWindow* X11Win = IWindow::Create(Attribs);
    bool Running = true;

    // TODO: OpenGL + Vulkan Support
    while (Running)
    {
        X11Win->Update();
        while (X11Win->IsEvent())
        {
            IEvent Event = X11Win->Event();
            switch (Event.Type)
            {
            case IEventType::PointerMoved:
            {
                //printf("Cursor Position:\nX: %d\nY: %d\n",
                //    Event.ePMoved.PointerX,
                //    Event.ePMoved.PointerY
                //);
                break;
            }
            case IEventType::ButtonEvent:
            {
                if (Event.eButton.Action == ButtonAction::Pressed)
                    printf("Button Pressed: %d\nLocation:\nX: %d\nY: %d\n",
                        (int)Event.eButton.Code,
                        Event.eButton.PointerX,
                        Event.eButton.PointerY
                    );
                break;
            }
            case IEventType::CharEvent:
                printf("%c : %d\n", Event.eChar, Event.eChar);
                break;
            case IEventType::KeyEvent:
            {
                if (Event.eKey.Code == Keys::Escape)
                    Running = false;
                break;
            }
            case IEventType::WindowClosed:
                // Break Loop
                Running = false;
                break;
            default:
                break;
            }
        }
    }

    delete X11Win;
    return 0;
}