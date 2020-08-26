#ifndef _KOT_X11_AUDIO_INTERFACE_
#define _KOT_X11_AUDIO_INTERFACE_

#include "Audio.hpp"

#include <alsa/asoundlib.h>

struct AudioData
{

};

struct __IPlayerImpl
{
    IWindow* _pIWindow;
    Sound _thisSound;
};

class ILinuxPlayer : public IPlayer
{
public:
    ILinuxPlayer(IWindow** _Window);
    virtual ~ILinuxPlayer() override;

    virtual Sound Load(const char* _File) override;
    virtual Sound Load(const char* _File, const SoundLoadData& _SLD) override;

    virtual void Play(const Sound& _Sound) override;
    virtual void Play(const Sound& _Sound, const SoundPlayBack& _SD) override;
};

#endif