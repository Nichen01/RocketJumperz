#pragma once
#include "AEEngine.h"

extern AEAudio Level;
extern AEAudio LaserBlast;
extern AEAudio Punch;
extern AEAudioGroup bgm;
extern AEAudioGroup soundEffects;

namespace audio {
	void loadsound();
	void unloadsound();
	void audiolevel(f32 level);
}