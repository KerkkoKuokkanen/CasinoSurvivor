
#include <unordered_map>
#include <iostream>
#include <filesystem>
#include "audio.h"
#include "commonTools.h"

namespace fs = std::filesystem;

static double GetDuration(Mix_Chunk *sound)
{
	uint32_t totalSamples = sound->alen / (2 * 2);
	double duration = (double)totalSamples / 44100.0;
	return (duration + 0.1);
}

std::tuple<std::unordered_map<uint64_t, SoundByte>, std::unordered_map<std::string, uint64_t>> LoadAudioFromAFolder()
{
	std::unordered_map<uint64_t, SoundByte> audioWithKeys;
	std::unordered_map<std::string, uint64_t> audioWithNames;
	for (const auto& entry : fs::recursive_directory_iterator("assets/audio/"))
	{
		if (fs::is_regular_file(entry.path()))
		{
			std::string filepath = entry.path().string();
			std::string spriteName = entry.path().stem().string(); 
			if (entry.path().extension() == ".wav")
			{
				Mix_Chunk *loaded = Mix_LoadWAV(filepath.c_str());
				if (loaded == NULL)
					continue ;
				uint64_t hash = HashData64(filepath.data(), filepath.size());
				double duration = GetDuration(loaded);
				audioWithKeys[hash] = {loaded, duration};
				audioWithNames[spriteName] = hash;
			}
		}
	}
	return {audioWithKeys, audioWithNames};
}