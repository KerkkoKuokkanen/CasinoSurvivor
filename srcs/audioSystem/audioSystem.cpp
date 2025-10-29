
#include "SDL_mixer.h"
#include <chrono>
#include <mutex>
#include <deque>
#include <random>
#include "audio.h"
#include "audioLoading.h"
#include <optional>

#define CHANNEL_AMOUNT 64

std::mutex soundQueueLock;
std::mutex moddingQueueLock;
std::mutex soundClearLock;

auto startTime = std::chrono::high_resolution_clock::now();

std::unordered_map<uint64_t, SoundByte> audioWithKeys;
std::unordered_map<std::string, uint64_t> audioWithNames;
std::deque<std::tuple<SoundByte, uint64_t, int, bool, uint32_t>> soundQueue = {};
std::deque<std::tuple<bool, bool, bool, float, uint64_t>> moddingQueue = {};

float audioVolume = 1.0f;
bool clearSounds = false;

struct Channel
{
	bool occupied;
	double duration;
	uint64_t key;
	bool paused;
	bool reserved;
};

Channel channels[64];

static uint64_t GenerateRandomNumber()
{
	static std::mt19937_64 gen(std::random_device{}());
	static std::uniform_int_distribution<uint64_t> distrib(0, (1ULL << 42) - 1);
	return distrib(gen);
}

uint64_t GetUniqueKeyForAudio()
{
	static uint64_t key = rand() % 4194304;
	uint64_t randomPart = GenerateRandomNumber();
	key += 1;
	if (key >= 4194304)
		key = 1;
	uint64_t ret = (key << 42) | randomPart;
	return (ret);
}

static double DeltaTime()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> deltaTime = currentTime - startTime;
	startTime = currentTime;
	return (deltaTime.count());
}

static void AddToPlayQueue(SoundByte sound, uint64_t key, int volume, bool replay, uint32_t loops)
{
	std::lock_guard<std::mutex> guard(soundQueueLock);
	soundQueue.push_back({sound, key, volume, replay, loops});
}

static std::optional<std::tuple<SoundByte, uint64_t, int, bool, uint32_t>> GetFromSoundQueue()
{
	std::lock_guard<std::mutex> guard(soundQueueLock);
	if (soundQueue.size() == 0)
		return (std::nullopt);
	auto item = soundQueue.front();
	soundQueue.pop_front();
	return (item);
}

static std::optional<std::tuple<bool, bool, bool, float, uint64_t>> GetFromModdingQueue()
{
	std::lock_guard<std::mutex> guard(moddingQueueLock);
	if (moddingQueue.size() == 0)
		return (std::nullopt);
	auto item = moddingQueue.front();
	moddingQueue.pop_front();
	return (item);
}

static uint64_t PlaySoundOwn(SoundByte sound, float volume, bool replay, uint64_t key, uint32_t loops)
{
	if (volume < 0.0f)
		volume = 0.0f;
	if (volume > 100.0f)
		volume = 100.0f;
	int vol = (int)((1.28f * volume) * audioVolume);
	if (key == 0)
		key = GetUniqueKeyForAudio();
	AddToPlayQueue(sound, key, vol, replay, loops);
	return (key);
}

Mix_Chunk *GetSoundWithName(std::string sound)
{
	uint64_t key = GetSoundKey(sound);
	if (key == 0)
		return (NULL);
	auto snd = audioWithKeys.find(key);
	if (snd == audioWithKeys.end())
		return (NULL);
	return (snd->second.sound);
}

uint64_t GetSoundKey(std::string sound)
{
	auto it = audioWithNames.find(sound);
	if (it == audioWithNames.end())
		return (0);
	return (it->second);
}

uint64_t PlaySound(uint64_t sound, float volume, uint32_t loops)
{
	auto it = audioWithKeys.find(sound);
	if (it == audioWithKeys.end())
		return (0);
	return (PlaySoundOwn(it->second, volume, false, 0, loops));
}

uint64_t PlaySound(std::string sound, float volume, uint32_t loops)
{
	auto it = audioWithNames.find(sound);
	if (it == audioWithNames.end())
		return (0);
	return (PlaySound(it->second, volume, loops));
}

uint64_t RePlaySound(uint64_t sound, float volume, uint32_t loops, uint64_t key)
{
	auto it = audioWithKeys.find(sound);
	if (it == audioWithKeys.end())
		return (0);
	if (key == 0)
		key = GetUniqueKeyForAudio();
	return (PlaySoundOwn(it->second, volume, true, key, loops));
}

uint64_t RePlaySound(std::string sound, float volume, uint32_t loops, uint64_t key)
{
	auto it = audioWithNames.find(sound);
	if (it == audioWithNames.end())
		return (0);
	auto it2 = audioWithKeys.find(it->second);
	if (it2 == audioWithKeys.end())
		return (0);
	if (key == 0)
		key = GetUniqueKeyForAudio();
	return (PlaySoundOwn(it2->second, volume, true, key, loops));
}

static void AddToModdingQueue(bool pause, bool resume, bool halt, float volume, uint64_t key)
{
	std::lock_guard<std::mutex> guard(moddingQueueLock);
	moddingQueue.push_back({pause, resume, halt, volume, key});
}

void ClearSoundSystem()
{
	std::lock_guard<std::mutex> guard(soundClearLock);
	clearSounds = true;
}

static bool GetClearSoundSystem()
{
	std::lock_guard<std::mutex> guard(soundClearLock);
	bool ret = clearSounds;
	clearSounds = false;
	return (ret);
}

void PauseSound(uint64_t key)
{
	AddToModdingQueue(true, false, false, -1.0f, key);
}

void ReusmeSound(uint64_t key)
{
	AddToModdingQueue(false, true, false, -1.0f, key);
}

void StopSound(uint64_t key)
{
	AddToModdingQueue(false, false, true, -1.0f, key);
}

void ChangeVolume(float volume, uint64_t key)
{
	if (volume < 0.0f)
		volume = 0.0f;
	if (volume > 100.0f)
		volume = 100.0f;
	AddToModdingQueue(false, false, false, volume, key);
}

void ChangeSystemVolume(float volume)
{
	if (volume < 0.0f)
		volume = 0.0f;
	if (volume > 100.0f)
		volume = 100.0f;
	audioVolume = 0.01f * volume;
}

static int GetChannelReplay(uint64_t key)
{
	int ret = -1;
	for (int i = 0; i < CHANNEL_AMOUNT; i++)
	{
		if (channels[i].occupied == false && channels[i].reserved == false)
			ret = i;
		if (channels[i].key == key)
			return (i);
	}
	return (ret);
}

static int GetChannel()
{
	for (int i = 0; i < CHANNEL_AMOUNT; i++)
	{
		if (channels[i].occupied == false && channels[i].reserved == false)
			return (i);
	}
	return (-1);
}

static void ProcessSoundQueue()
{
	int channel = 0;
	while (true)
	{
		auto item = GetFromSoundQueue();
		if (!item)
			return ;
		auto [sound, key, volume, replay, loops] = *item;
		if (replay)
			channel = GetChannelReplay(key);
		else
			channel = GetChannel();
		if (channel == -1)
			continue ;
		channels[channel].paused = false;
		channels[channel].duration = sound.duration * (loops + 1);
		channels[channel].key = key;
		channels[channel].occupied = true;
		Mix_Volume(channel, volume);
		Mix_PlayChannel(channel, sound.sound, loops);
	}
}

static void HandleChannels()
{
	double deltaTime = DeltaTime();
	for (int i = 0; i < CHANNEL_AMOUNT; i++)
	{
		if (channels[i].occupied && !channels[i].paused)
		{
			channels[i].duration -= deltaTime;
			if (channels[i].duration <= 0.0)
			{
				channels[i].occupied = false;
				channels[i].paused = false;
			}
		}
	}
}

static int FindChannel(uint64_t key)
{
	for (int i = 0; i < CHANNEL_AMOUNT; i++)
	{
		if (channels[i].key == key && channels[i].occupied)
			return (i);
	}
	return (-1);
}

static void HandleSoundModding()
{
	while (true)
	{
		auto item = GetFromModdingQueue();
		if (!item)
			return ;
		auto [pause, resume, halt, volume, key] = *item;
		int channel = FindChannel(key);
		if (channel == (-1))
			continue ;
		if (pause)
		{
			channels[channel].paused = true;
			Mix_Pause(channel);
		}
		else if (resume)
		{
			channels[channel].paused = false;
			Mix_Resume(channel);
		}
		else if (halt)
		{
			channels[channel].occupied = false;
			channels[channel].paused = false;
			Mix_HaltChannel(channel);
		}
		else if (volume >= 0.0f)
			Mix_Volume(channel, (int)((1.28f * volume) * audioVolume));
	}
}

static void SoundSystemClearing()
{
	bool willClear = GetClearSoundSystem();
	if (willClear == false)
		return ;
	for (int i = 0; i < CHANNEL_AMOUNT; i++)
	{
		channels[i].occupied = false;
		channels[i].paused = false;
		Mix_HaltChannel(i);
	}
}

void AudioThread()
{
	HandleSoundModding();
	HandleChannels();
	ProcessSoundQueue();
	SoundSystemClearing();
}

void ReserveChannel(int channel)
{
	std::lock_guard<std::mutex> guard(moddingQueueLock);
	channels[channel].reserved = true;
}

void InitAudio()
{
	bzero(channels, sizeof(Channel) * 64);
	std::tie(audioWithKeys, audioWithNames) = LoadAudioFromAFolder();
	Mix_AllocateChannels(CHANNEL_AMOUNT);
}
