#ifndef _KOT_KEYCODE_ENUMS_
#define _KOT_KEYCODE_ENUMS_

enum class ButtonAction
{
    Pressed,
    Released,
};

enum class KeyAction
{
    Pressed,
    Released,
    Repeat
};

enum struct Buttons
{
    NoButton = 0,

    Button_1,
    Button_2,
    Button_3,
    Button_4,
    Button_5,

    ScrollUp,
    ScrollDown
};

enum struct Keys
{
    NoKey = 0,

    Escape,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,

    PrntScr,
    PrintScreen = PrntScr,

    ScrlLck,
    ScrollLock = ScrlLck,

    Pause,

    Grave,
    Tilde = Grave,

    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    Num0,

    Minus,
    Equal,

    Backspace,
    Return,
    Tab,
    Caps,

    LCtrl,
    RCtrl,
    LControl = LCtrl,
    RControl = RCtrl,

    LShift,
    RShift,

    LAlt,
    RAlt,

    LMeta,
    RMeta,
    Menu,

    ArrowUp,
    ArrowDown,
    ArrowLeft,
    ArrowRight,

    Insert, 
    Home,
    Del,
    Delete = Del,
    End,
    PageUp,
    PageDown,

    Space,

    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    Last
};

#endif