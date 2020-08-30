#include <Window.hpp>
#include <stdio.h>

void ToggleCursorState(IWindow::CursorState& _State)
{
    switch (_State)
    {
    case IWindow::CursorState::Shown:
        _State = IWindow::CursorState::Hidden;
        break;
    case IWindow::CursorState::Hidden:
        _State = IWindow::CursorState::Shown;
        break;
    default: break;
    }
}

int main()
{
    IWindow::Attributes Attribs;
    Attribs.Title = "Window Test";
    Attribs.X = 0;
    Attribs.Y = 0;
    Attribs.Width = 800;
    Attribs.Height = 600;
    Attribs.aFlags = IWindow::Flags::WindowMaximized;

	IWindow* Window = IWindow::Create(Attribs);

	bool Running = true;
    IWindow::CursorState CState = IWindow::CursorState::Shown;

    while (Running)
    {
        Window->SetCursorState(CState);

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
            case WEventType::KeyEvent:
            {
                if (Event.eKey.Code == Keys::Escape)
                    ToggleCursorState(CState);
                break;
            }
            case WEventType::WindowChanged:
                printf("Window Moved:\n X: %d\n Y: %d\n W: %d\n H: %d\n",
                    Event.eWChanged.X,
                    Event.eWChanged.Y,
                    Event.eWChanged.W,
                    Event.eWChanged.H);
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
            case WEventType::WindowMinimized:
                printf("Window Minimized\n");
                break;
            case WEventType::WindowMaximized:
                printf("Window Maximized\n");
                break;
            default:
                break;
            }
        }
    }

	delete Window;
	return 0;
}