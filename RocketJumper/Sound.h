#pragma once
#ifndef SOUND_H_
#define SOUND_H_
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

#endif