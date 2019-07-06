#include <sound/sound.h>

namespace neko {

	std::set<Index> Sound::index_;
	std::map<Index, std::unique_ptr<sf::Music>> Sound::musicMap_;
	std::map<Index, std::unique_ptr<sf::SoundBuffer>> Sound::soundBufferMap_;
	std::map<Index, std::vector<Index>> Sound::soundBufferSoundMap_;
	std::map<Index, sf::Sound> Sound::soundMap_;

	Index Sound::LoadSoundBuffer(const std::string_view soundFile)
	{
		Index ind = GetNextIndex();
		if (ind == INDEX_INVALID) 
		{
			return INDEX_INVALID;
		}
		soundBufferMap_.insert(
			std::make_pair(ind, std::make_unique<sf::SoundBuffer>()));
		if (!soundBufferMap_[ind]->loadFromFile(std::string(soundFile)))
		{
			soundBufferMap_.erase(ind);
			return INDEX_INVALID;
		}
		soundBufferSoundMap_.insert({ ind, {} });
		index_.insert(ind);
		return ind;
	}

	Index Sound::CreateSoundFromBuffer(Index indBuffer)
	{
		Index ind = GetNextSoundIndex();
		if (ind == INDEX_INVALID)
		{
			return INDEX_INVALID;
		}
		auto it = soundBufferMap_.find(indBuffer);
		if (it == soundBufferMap_.end())
		{
			return INDEX_INVALID;
		}
		soundMap_.insert({ ind, {} });
		soundMap_[ind].setBuffer(*it->second);
		soundBufferSoundMap_[indBuffer].push_back(ind);
		return ind;
	}

	bool Sound::PlaySound(Index ind)
	{
		auto it = soundMap_.find(ind);
		if (it == soundMap_.end())
		{
			return false;
		}
		if (!it->second.getBuffer())
		{
			return false;
		}
		it->second.play();
		return true;
	}

	bool Sound::StopSound(Index ind)
	{
		auto it = soundMap_.find(ind);
		if (it == soundMap_.end())
		{
			return false;
		}
		if (!it->second.getBuffer())
		{
			return false;
		}
		it->second.stop();
		return true;
	}

	bool Sound::RemoveSoundBuffer(Index ind)
	{
		{
			auto it = soundBufferSoundMap_.find(ind);
			for (Index indSound : it->second)
			{
				soundMap_.erase(indSound);
			}
		}
		{
			auto it = soundBufferMap_.find(ind);
			if (it == soundBufferMap_.end())
			{
				return false;
			}
			soundBufferMap_.erase(ind);
		}
		index_.erase(ind);
		return true;
	}

	Index Sound::LoadMusic(const std::string_view musicFile)
	{
		Index ind = GetNextIndex();
		if (ind == INDEX_INVALID)
		{
			return INDEX_INVALID;
		}
		musicMap_.insert(std::make_pair(ind, std::make_unique<sf::Music>()));
		if (!musicMap_[ind]->openFromFile(std::string(musicFile)))
		{
			musicMap_.erase(ind);
			return INDEX_INVALID;
		}
		index_.insert(ind);
		return ind;
	}

	bool Sound::PlayMusic(Index ind)
	{
		auto it = musicMap_.find(ind);
		if (it == musicMap_.end())
		{
			return false;
		}
		it->second->play();
		return true;
	}

	bool Sound::StopMusic(Index ind)
	{
		auto it = musicMap_.find(ind);
		if (it == musicMap_.end())
		{
			return false;
		}
		it->second->stop();
		return true;
	}

	bool Sound::RemoveMusic(Index ind)
	{
		auto it = musicMap_.find(ind);
		if (it == musicMap_.end())
		{
			return false;
		}
		musicMap_.erase(ind);
		return true;
	}

	Index Sound::GetNextIndex()
	{
		static Index index = 0;
		if (musicMap_.size() + soundBufferMap_.size() > 255)
		{
			return INDEX_INVALID;
		}
		return index++;
	}

	Index Sound::GetNextSoundIndex()
	{
		static Index index = 0;
		return index++;
	}

}