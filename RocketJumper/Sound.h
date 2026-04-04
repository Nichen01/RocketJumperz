#pragma once
#include "AEEngine.h"

extern f32 MainVolume;
extern s8 sCount;

extern AEAudio Level;
extern AEAudio LaserBlast;
extern AEAudio Punch;
extern AEAudio Error;
extern AEAudio Pickup;
extern AEAudio ArrowSound;
extern AEAudio GravityToggle;
extern AEAudio MainMenu;
extern AEAudioGroup bgm;
extern AEAudioGroup soundEffects;

namespace audio {
	void loadsound();
	void unloadsound();
	void audiolevel(f32 level);
}