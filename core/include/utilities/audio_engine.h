#ifndef _AUDIO_ENGINE_H_	//Prevents multiple definition of the objects
#define _AUDIO_ENGINE_H_

#include <mathematics/vector.h>
#include <string_view>

template<typename Sound, typename Channel, typename  Event, typename Bank>
class AudioEngine
{
    using ChannelId = size_t;
public:
    AudioEngine();
    ~AudioEngine();

    void Update();

    //Loading
    void LoadBank(std::string_view bankName);
    void LoadEvent(std::string_view eventName);
    void LoadSound(std::string_view soundName);
    void UnloadSound(std::string_view soundName);

    //Playing/Stopping
    void PlaySound(std::string_view soundName);
    void PlayEvent(std::string_view eventName);
    void StopChannel(ChannelId channelID);
    void StopEvent(std::string_view eventName);
    void StopAllChannels();

    bool IsPlaying(Channel channelID);
    bool IsEventPlaying(std::string_view eventName);
	
private:
    typedef std::map<std::string, Sound*> SoundMap;
    typedef std::map<std::string, Channel*> ChannelMap;
    typedef std::map<std::string, Event*> EventMap;
    typedef std::map<std::string, Bank*> BankMap;

    SoundMap soundMap;
    ChannelMap channelMap;
    EventMap eventMap;
    BankMap bankMap;
};
#endif
