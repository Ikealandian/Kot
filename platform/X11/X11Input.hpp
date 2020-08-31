#ifndef _KOT_X11_INPUT_
#define _KOT_X11_INPUT_

#include "X11Impl.hpp"

#include <map>
#include <vector>
#include <string.h>

typedef struct __X11Input
{
    static std::map<const char*, Keys> X11InputMap;
    static std::vector<unsigned int> X11KeyTable;
    static std::vector<Keys> KeyTable;

    static XIC IC;
    static XIM IM;

    static char32_t UTF82C32(char _utf8[4])
    {
        char32_t Char32 = 0;
        if ((_utf8[0] & 0x80) == 0x00)
        {
            Char32 = _utf8[0];
        }
        else if ((_utf8[0] & 0xE0) == 0xC0)
        {
            Char32
                = (_utf8[0] & 0x1F) << 6
                | (_utf8[1] & 0x3F);
        }
        else if ((_utf8[2] & 0xF0) == 0xE0)
        {
            Char32
                = (_utf8[0] & 0x0F) << 12
                | (_utf8[1] & 0x3F) << 6
                | (_utf8[2] & 0x3F);
        }
        else if ((_utf8[0] & 0xF8) == 0xF0)
        {
            Char32
                = (_utf8[0] & 0x07) << 18
                | (_utf8[1] & 0x3F) << 12
                | (_utf8[2] & 0x3F) << 6
                | (_utf8[3] & 0x3F);
        }
        return Char32;
    }

    static void InitKeyTables(Display** _xdis)
    {
        XkbDescPtr pMapDesc = XkbGetMap(*_xdis, 0, XkbUseCoreKbd);
        XkbGetNames(*_xdis, XkbKeyNamesMask, pMapDesc);

        X11KeyTable.resize(pMapDesc->max_key_code + 1);
        KeyTable.resize(pMapDesc->max_key_code + 1);

        for (unsigned int X11Code = 0; X11Code <= pMapDesc->max_key_code; X11Code++) {
            
            bool Found = false;
            for (auto it : X11InputMap) {
                if (strncmp(it.first, pMapDesc->names->keys[X11Code].name, XkbKeyNameLength) == 0) {

                    KeyTable[X11Code] = it.second;
                    X11KeyTable[(std::size_t)it.second] = X11Code;
                    
                    Found = true;
                    break;
                }
            }
            
            if (!Found)
            {
                KeyTable[X11Code] = Keys::NoKey;
            }
        }

        XkbFreeNames(pMapDesc, XkbKeyNamesMask, True);
        XkbFreeKeyboard(pMapDesc, 0, True);
    }

    static void Init(Display** _xdis, Window* _xwin)
    {
        InitKeyTables(_xdis);

        XSetLocaleModifiers("");
        IM = XOpenIM(*_xdis, nullptr, nullptr, nullptr);

        if (!IM)
        {
            XSetLocaleModifiers("@im=none");
            IM = XOpenIM(*_xdis, nullptr, nullptr, nullptr);    
        }

        IC = XCreateIC
        (
            IM,
            XNInputStyle,
            XIMPreeditNothing | XIMStatusNothing,
            XNClientWindow,
            *_xwin,
            XNFocusWindow,
            *_xwin,
            nullptr
        );
    }

    static Keys FromX11Key(int Key)
    {
        if (Key > (int)Keys::Last)
            return Keys::NoKey;
        return KeyTable[Key];
    }
}X11Input;

std::map<const char*, Keys>
X11Input::X11InputMap = {
    { "AD01", Keys::Q },
    { "AD02", Keys::W },
    { "AD03", Keys::E },
    { "AD04", Keys::R },
    { "AD05", Keys::T },
    { "AD06", Keys::Y },
    { "AD07", Keys::U },
    { "AD08", Keys::I },
    { "AD09", Keys::O },
    { "AD10", Keys::P },
    { "AC01", Keys::A },
    { "AC02", Keys::S },
    { "AC03", Keys::D },
    { "AC04", Keys::F },
    { "AC05", Keys::G },
    { "AC06", Keys::H },
    { "AC07", Keys::J },
    { "AC08", Keys::K },
    { "AC09", Keys::L },
    { "AB01", Keys::Z },
    { "AB02", Keys::X },
    { "AB03", Keys::C },
    { "AB04", Keys::V },
    { "AB05", Keys::B },
    { "AB06", Keys::N },
    { "AB07", Keys::M },
    { "SPCE", Keys::Space },
    { "ESC",  Keys::Escape },
    { "FK01", Keys::F1 },
    { "FK02", Keys::F2 },
    { "FK03", Keys::F3 },
    { "FK04", Keys::F4 },
    { "FK05", Keys::F5 },
    { "FK06", Keys::F6 },
    { "FK07", Keys::F7 },
    { "FK08", Keys::F8 },
    { "FK09", Keys::F9 },
    { "FK10", Keys::F10 },
    { "FK11", Keys::F11 },
    { "FK12", Keys::F12 },
    { "AE01", Keys::Num1 },
    { "AE02", Keys::Num2 },
    { "AE03", Keys::Num3 },
    { "AE04", Keys::Num4 },
    { "AE05", Keys::Num5 },
    { "AE06", Keys::Num6 },
    { "AE07", Keys::Num7 },
    { "AE08", Keys::Num8 },
    { "AE09", Keys::Num9 },
    { "AE10", Keys::Num0 },
    { "PRSC", Keys::PrintScreen },
    { "SCLK", Keys::ScrollLock },
    { "PAUS", Keys::Pause},
    { "TLDE", Keys::Tilde },
    { "BKSP", Keys::Backspace },
    { "TAB",  Keys::Tab },
    { "CAPS", Keys::Caps },
    { "RTRN", Keys::Return },
    { "LFSH", Keys::LShift },
    { "RTSH", Keys::RShift },
    { "LCTL", Keys::LControl },
    { "RCTL", Keys::RControl },
    { "LWIN", Keys::LMeta },
    { "RWIN", Keys::RMeta },
    { "LALT", Keys::LAlt },
    { "RLAT", Keys::RAlt },
    { "MENU", Keys::Menu },
    { "INS",  Keys::Insert },
    { "HOME", Keys::Home },
    { "DELE", Keys::Delete },
    { "END",  Keys::End },
    { "PGUP", Keys::PageUp },
    { "PGDN", Keys::PageDown },
    { "UP",   Keys::ArrowUp },
    { "LEFT", Keys::ArrowLeft },
    { "DOWN", Keys::ArrowDown },
    { "RGHT", Keys::ArrowRight },
};

XIC                             X11Input::IC;
XIM                             X11Input::IM;
std::vector<unsigned int>       X11Input::X11KeyTable;
std::vector<Keys>               X11Input::KeyTable;

#endif