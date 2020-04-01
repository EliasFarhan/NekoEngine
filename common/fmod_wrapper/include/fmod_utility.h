#pragma once
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <string_view>


//Vector3 struct to place sound in 3D space
struct Vector3		//TODO: Change with Neko Vector3s
{
	float x;
	float y;
	float z;
};





//It will do calls to the Implementation struct to start, stop and update FMOD
using ChannelId = std::uint32_t;
class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();
    //Engine Execution
    void Update();

    //Loading Data
    void LoadBank(std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
    void LoadEvent(std::string& eventName);
    void LoadSound(std::string& soundName, bool is3d = true, bool isLooping = false, bool isStream = false);
    void UnLoadSound(std::string& soundName);

    //Playing Data
    void PlaySound(std::string& soundName, const Vector3& position = Vector3{0, 0, 0}, float volumeDB = 0.0f);
    void PlayEvent(std::string& eventName);
    void StopChannel(ChannelId channelID);
    void StopEvent(std::string& eventName, bool stopImmediate = false);
    float GetEventParameter(std::string& eventName, std::string& eventParameter);
    void SetEventParameter(std::string& eventName, std::string& parameterName, float parameterValue);
    void StopAllChannels();
    void SetChannel3dPosition(ChannelId channelID, const Vector3& position);
    void SetChannelVolume(ChannelId channelID, float volumeDB);
    bool IsPlaying(ChannelId channelID);
    bool IsEventPlaying(std::string& eventName);
    void SetListener(const Vector3& position = Vector3{ 0, 0, 0 }, float volumeDB = 0.0f);
    float DbToVolume(float dB);
    float VolumeToDb(float volume);
    FMOD_VECTOR Vec3ToFmod(const Vector3& position);
  
private:
    FMOD::Studio::System* fmodStudioSystem;
    FMOD::System* fmodSystem;
    typedef std::map<std::string, FMOD::Sound*> SoundMap;
    typedef std::map<unsigned int, FMOD::Channel*> ChannelMap;
    typedef std::map<std::string, FMOD::Studio::EventInstance*> EventMap;
    typedef std::map<std::string, FMOD::Studio::Bank*> BankMap;

    SoundMap soundMap;
    ChannelMap channelMap;
    EventMap eventMap;
    BankMap bankMap;
    unsigned int nextChannelID;
    const int MAX_CHANNELS = 32;
};
