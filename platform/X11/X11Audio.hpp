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

class IX11Player : public IPlayer
{
public:
    IX11Player(IWindow** _Window);
    virtual ~IX11Player() override;

    virtual Sound Load(const char* _File) override;
    virtual Sound Load(const char* _File, const SoundLoadData& _SLD) override;

    virtual void Play(const Sound& _Sound) override;
    virtual void Play(const Sound& _Sound, const SoundData& _SD) override;
};

#endif