#pragma once
#include "AEEngine.h"

extern AEAudio Level;
extern AEAudio LaserBlast;
extern AEAudio Punch;
extern AEAudio Error;
extern AEAudio Pickup;
extern AEAudio ArrowSound;
extern AEAudio MainMenu;
extern AEAudioGroup bgm;
extern AEAudioGroup soundEffects;

namespace audio {
	void loadsound();
	void unloadsound();
	void audiolevel(f32 level);
}