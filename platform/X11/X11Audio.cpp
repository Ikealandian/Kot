#include "X11/X11Audio.hpp"
#include "X11/X11Window.hpp"

#include <X11/Xlib.h>

IX11Player::IX11Player(IWindow** _Window)
{
    _plyrImpl = new __IPlayerImpl;
    _plyrImpl->_pIWindow = *_Window;
}

IX11Player::~IX11Player()
{
    _plyrImpl->_pIWindow = nullptr;
    delete _plyrImpl;
}

Sound IX11Player::Load(const char* _File)
{
    
}

Sound IX11Player::Load(const char* _File, const SoundLoadData& _SLD)
{

}

void IX11Player::Play(const Sound& _Sound)
{

}

void IX11Player::Play(const Sound& _Sound, const SoundData& _SD)
{

}