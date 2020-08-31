#ifndef _KOT_WINDOWS_INPUT_
#define _KOT_WINDOWS_INPUT_

#include <Events.hpp>
#include <InputCodes.hpp>

#include <Windows.h>
#include <map>

typedef struct __WinInput
{
    static std::map<LONG, Keys> WinInputMap;
    static bool InWindow;

    static ButtonAction ActionFromMessage(int Message)
    {
        switch (Message)
        {
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_XBUTTONDOWN:
            return ButtonAction::Pressed;
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
        case WM_XBUTTONUP:
            return ButtonAction::Released;
        default: break;
        }
        return ButtonAction::NoAction;
    }

    static ScrollAxis GetScrollAxis(int Message)
    {
        switch (Message)
        {
        case WM_MOUSEHWHEEL:
            return ScrollAxis::ScrollHorizontal;
        case WM_MOUSEWHEEL:
            return ScrollAxis::ScrollVertical;
        default: break;
        }
        return ScrollAxis::NoAxis;
    }

    static Buttons FromWinButton(int Button, LPARAM wParam)
    {
        switch (Button)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            return Buttons::Button_1;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            return Buttons::Button_2;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            return Buttons::Button_3;
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        {
            DWORD vkButton = GET_XBUTTON_WPARAM(wParam);
            switch (vkButton)
            {
            case XBUTTON1:
                return Buttons::Button_4;
            case XBUTTON2:
                return Buttons::Button_5;
            default: break;
            }
            break;
        }
        default: break;
        }
        return Buttons::NoButton;
    }

    static Keys FromVirtualKey(int Key)
    {
        if (WinInputMap.find(Key) == WinInputMap.end())
            return Keys::NoKey;
        return WinInputMap[Key];
    }
}WinInput;

std::map<LONG, Keys>
WinInput::WinInputMap = {
     { 'Q',         Keys::Q             },
     { 'W',         Keys::W             },
     { 'E',         Keys::E             },
     { 'R',         Keys::R             },
     { 'T',         Keys::T             },
     { 'Y',         Keys::Y             },
     { 'U',         Keys::U             },
     { 'I',         Keys::I             },
     { 'O',         Keys::O             },
     { 'P',         Keys::P             },
     { 'A',         Keys::A             },
     { 'S',         Keys::S             },
     { 'D',         Keys::D             },
     { 'F',         Keys::F             },
     { 'G',         Keys::G             },
     { 'H',         Keys::H             },
     { 'J',         Keys::J             },
     { 'K',         Keys::K             },
     { 'L',         Keys::L             },
     { 'Z',         Keys::Z             },
     { 'X',         Keys::X             },
     { 'C',         Keys::C             },
     { 'V',         Keys::V             },
     { 'B',         Keys::B             },
     { 'N',         Keys::N             },
     { 'M',         Keys::M             },
     { VK_SPACE,    Keys::Space         },
     { VK_ESCAPE,   Keys::Escape        },
     { VK_F1,       Keys::F1            },
     { VK_F2,       Keys::F2            },
     { VK_F3,       Keys::F3            },
     { VK_F4,       Keys::F4            },
     { VK_F5,       Keys::F5            },
     { VK_F6,       Keys::F6            },
     { VK_F7,       Keys::F7            },
     { VK_F8,       Keys::F8            },
     { VK_F9,       Keys::F9            },
     { VK_F10,      Keys::F10           },
     { VK_F11,      Keys::F11           },
     { VK_F12,      Keys::F12           },
     { VK_NUMPAD1,  Keys::Num1          },
     { VK_NUMPAD2,  Keys::Num2          },
     { VK_NUMPAD3,  Keys::Num3          },
     { VK_NUMPAD4,  Keys::Num4          },
     { VK_NUMPAD5,  Keys::Num5          },
     { VK_NUMPAD6,  Keys::Num6          },
     { VK_NUMPAD7,  Keys::Num7          },
     { VK_NUMPAD8,  Keys::Num8          },
     { VK_NUMPAD9,  Keys::Num9          },
     { VK_NUMPAD0,  Keys::Num0          },
     { VK_SNAPSHOT, Keys::PrintScreen   },
     { VK_SCROLL,   Keys::ScrollLock    },
     { VK_PAUSE,    Keys::Pause         },
     { VK_OEM_3,    Keys::Tilde         },
     { VK_BACK,     Keys::Backspace     },
     { VK_TAB,      Keys::Tab           },
     { VK_CAPITAL,  Keys::Caps          },
     { VK_RETURN,   Keys::Return        },
     { VK_LSHIFT,   Keys::LShift        },
     { VK_RSHIFT,   Keys::RShift        },
     { VK_LCONTROL, Keys::LControl      },
     { VK_RCONTROL, Keys::RControl      },
     { 0L,          Keys::LMeta         },
     { 0L,          Keys::RMeta         },
     { VK_MENU,     Keys::LAlt          },
     { VK_RMENU,    Keys::RAlt          },
     { 0L,          Keys::Menu          },
     { VK_INSERT,   Keys::Insert        },
     { VK_HOME,     Keys::Home          },
     { VK_DELETE,   Keys::Delete        },
     { VK_END,      Keys::End           },
     { 0L,          Keys::PageUp        },
     { 0L,          Keys::PageDown      },
     { VK_UP,       Keys::ArrowUp       },
     { VK_LEFT,     Keys::ArrowLeft     },
     { VK_DOWN,     Keys::ArrowDown     },
     { VK_RIGHT,    Keys::ArrowRight    },
};

bool WinInput::InWindow = false;

#endif