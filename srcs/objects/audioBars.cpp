
#include "kiss_fft.h"
#include "kiss_fftr.h"
#include "SDL_mixer.h"
#include <mutex>
#include <vector>
#include "audio.h"
#include "audioBars.h"
#include "deltaTime.h"

const int RESERVED_CHANNEL = 60;

const int FFT_SIZE = 1024;
const int NUM_BINS = (FFT_SIZE / 2) + 1;

std::vector<float> g_barHeights(NUM_BARS, 0.0f);
std::mutex g_dataMutex;

kiss_fftr_cfg g_fftConfig;
std::vector<float> g_fftInput(FFT_SIZE);
std::vector<kiss_fft_cpx> g_fftOutput(NUM_BINS);

void channelEffectCallback(int chan, void *stream, int len, void *udata)
{
	Sint16* samples = (Sint16*)stream;
	int numFrames = len / 4; // Number of stereo frames
		
	// We need FFT_SIZE samples. Process what we can.
	int samplesToProcess = std::min(numFrames, FFT_SIZE);
		
	for (int i = 0; i < samplesToProcess; i++) {
		// Average the left and right channels to get a mono signal
		float leftSample = (float)samples[i * 2];
		float rightSample = (float)samples[i * 2 + 1];
		g_fftInput[i] = (leftSample + rightSample) / 2.0f;
	}
		
	// Zero-pad the rest of the input buffer if we didn't get enough samples
	for (int i = samplesToProcess; i < FFT_SIZE; i++) {
		g_fftInput[i] = 0.0f;
	}

	// 2. Run the FFT
	kiss_fftr(g_fftConfig, g_fftInput.data(), g_fftOutput.data());

	// 3. Calculate magnitudes and group into 21 bars
	std::vector<float> newBarHeights(NUM_BARS);
	int binsPerBar = (NUM_BINS - 1) / NUM_BARS; // -1 to ignore DC offset bin
		
	for (int i = 0; i < NUM_BARS; i++) {
		int startBin = (i * binsPerBar) + 1; // +1 to ignore DC
		int endBin = (i + 1) * binsPerBar;
		
		// Ensure the last bar goes all the way to the end
		if (i == NUM_BARS - 1) {
			endBin = NUM_BINS;
		}

		float averageMagnitude = 0.0f;
		for (int j = startBin; j < endBin; ++j) {
			float real = g_fftOutput[j].r;
			float imag = g_fftOutput[j].i;
			// Get magnitude. sqrt(real*real + imag*imag)
			averageMagnitude += std::sqrt(real * real + imag * imag);
		}
		
		// Avoid division by zero if endBin <= startBin (shouldn't happen here, but safe)
		if (endBin > startBin) {
			averageMagnitude /= (float)(endBin - startBin);
		}
		newBarHeights[i] = averageMagnitude;
	}

	// 4. Safely update the global bar heights for the render thread
	std::lock_guard<std::mutex> lock(g_dataMutex);
	g_barHeights = newBarHeights;
}

void PlayMusic(std::string sound)
{
	Mix_Chunk *chunk = GetSoundWithName(sound);
	if (chunk == NULL)
		return ;
	Mix_PlayChannel(RESERVED_CHANNEL, chunk, -1);
	Mix_RegisterEffect(RESERVED_CHANNEL, channelEffectCallback, NULL, NULL);
}

void AudioBars::GrabBarHeights()
{
	std::lock_guard<std::mutex> lock(g_dataMutex);
	localBarHeights = g_barHeights;
}

void AudioBars::Update()
{
	GrabBarHeights();
	if (grid != NULL)
		grid->GiveFrequencies(localBarHeights);
}

void AudioBars::Start()
{
	grid = (FloorGrid*)self->GetComponent("FloorGrid");
	ReserveChannel(RESERVED_CHANNEL);
	Mix_Volume(RESERVED_CHANNEL, 28);
	PlayMusic("testmus2");
}

AudioBars::AudioBars()
{
	g_fftConfig = kiss_fftr_alloc(FFT_SIZE, 0, NULL, NULL);
	localBarHeights = std::vector<float>(NUM_BARS, 0.0f);
}

AudioBars::~AudioBars()
{
	Mix_UnregisterEffect(RESERVED_CHANNEL, channelEffectCallback);
	if (g_fftConfig)
		kiss_fftr_free(g_fftConfig);
}
