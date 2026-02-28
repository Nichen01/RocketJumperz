#include "sound.h"

AEAudio Level;
AEAudio LaserBlast;
AEAudio Punch;
AEAudioGroup bgm;
AEAudioGroup soundEffects;

namespace audio {
	void loadsound() {
		// Load the music file once when the level loads
		Level = AEAudioLoadMusic("Assets/Sounds/L1_bgm.mp3");

		// Create the audio group
		bgm = AEAudioCreateGroup();

		// Configure sound effects
		LaserBlast = AEAudioLoadSound("Assets/Sounds/LaserBlast.mp3");
		Punch = AEAudioLoadSound("Assets/Sounds/Punch.wav");
		soundEffects = AEAudioCreateGroup();   // short for 'sound effect'
	}
}