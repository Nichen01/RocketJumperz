#include "sound.h"

AEAudio Level;
AEAudio LaserBlast;
AEAudio Punch;
AEAudioGroup bgm;
AEAudioGroup soundEffects;

namespace audio {
	void audiolevel( f32 level) {
		AEAudioSetGroupVolume(bgm, level);
	}
}