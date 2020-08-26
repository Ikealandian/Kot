#include "Linux/LinuxAudio.hpp"
#include <X11/Xlib.h>

ILinuxPlayer::ILinuxPlayer(IWindow** _Window)
{
    _plyrImpl = new __IPlayerImpl;
    _plyrImpl->_pIWindow = *_Window;
}

ILinuxPlayer::~ILinuxPlayer()
{
    _plyrImpl->_pIWindow = nullptr;
    delete _plyrImpl;
}

Sound ILinuxPlayer::Load(const char* _File)
{
    
}

Sound ILinuxPlayer::Load(const char* _File, const SoundLoadData& _SLD)
{

}

void ILinuxPlayer::Play(const Sound& _Sound)
{

}

void ILinuxPlayer::Play(const Sound& _Sound, const SoundPlayBack& _SD)
{

}