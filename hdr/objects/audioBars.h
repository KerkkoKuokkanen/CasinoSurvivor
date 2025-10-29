
#ifndef AUDIO_BARS_H
# define AUDIO_BARS_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "floorGrid.h"

# define NUM_BARS 43

class AudioBars : public CustomComponent
{
	private:
		FloorGrid *grid = NULL;
		std::vector<float> localBarHeights;
		void GrabBarHeights();
	public:
		AudioBars();
		~AudioBars();
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(AudioBars);

#endif
