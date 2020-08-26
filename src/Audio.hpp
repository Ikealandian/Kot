#ifndef _KOT_AUDIO_
#define _KOT_AUDIO_

#include <Window.hpp>

// IPlayer* Player = IPlayer::Create(&Window);
// Sound Effect = Player->Load("Effect.mp3");
// Player.Play(Effect);
// Player.Stop(Effect);

using Sound = unsigned;

typedef struct __IPlayerLoadData
{
}SoundLoadData;

typedef struct __IPlayerSoundData
{
    float Volume;
    float Speed;
}SoundPlayBack;

typedef class __IPlayerInterface
{
protected:
    struct __IPlayerImpl* _plyrImpl;

public:
    virtual ~__IPlayerInterface() = default;

    virtual Sound Load(const char* _File) = 0;
    virtual Sound Load(const char* _File, const SoundLoadData& _SLD) = 0;

    virtual void Play(const Sound& _Sound) = 0;
    virtual void Play(const Sound& _Sound, const SoundPlayBack& _SD) = 0;

public:
    static __IPlayerInterface* Create(IWindow** _Window);

}IPlayer;

#endif