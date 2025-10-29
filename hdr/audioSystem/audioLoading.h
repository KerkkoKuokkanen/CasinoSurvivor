
#ifndef AUDIO_LOADING_H
# define AUDIO_LOADING_H

#include "audio.h"

std::tuple<std::unordered_map<uint64_t, SoundByte>, std::unordered_map<std::string, uint64_t>> LoadAudioFromAFolder();

#endif
