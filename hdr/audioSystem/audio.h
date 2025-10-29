
#ifndef AUDIO_H
# define AUDIO_H

#include "SDL_mixer.h"
#include <unordered_map>

struct SoundByte {
	Mix_Chunk *sound;
	double duration;
};

void InitAudio();
void AudioThread();
Mix_Chunk *GetSoundWithName(std::string sound);
uint64_t GetSoundKey(std::string sound);
uint64_t GetUniqueKeyForAudio();
uint64_t PlaySound(uint64_t sound, float volume, uint32_t loops);
uint64_t PlaySound(std::string sound, float volume, uint32_t loops);
uint64_t RePlaySound(uint64_t sound, float volume, uint32_t loops, uint64_t key = 0);
uint64_t RePlaySound(std::string sound, float volume, uint32_t loops, uint64_t key = 0);

void PauseSound(uint64_t key);
void ReusmeSound(uint64_t key);
void StopSound(uint64_t key);
void ChangeVolume(float volume, uint64_t key);
void ReserveChannel(int channel);

void ChangeSystemVolume(float volume);
void ClearSoundSystem();

#endif
