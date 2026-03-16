#include "Sound.h"

AEAudio Level;
AEAudio LaserBlast;
AEAudio Punch;
AEAudio Error;
AEAudio Pickup;
AEAudio ArrowSound;
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
		Error = AEAudioLoadSound("Assets/Sounds/Error.mp3");
		Pickup = AEAudioLoadSound("Assets/Sounds/Pickup.wav");
		ArrowSound = AEAudioLoadSound("Assets/Sounds/ArrowSound.wav");
		soundEffects = AEAudioCreateGroup();   // short for 'sound effect'

	}
	void audiolevel( f32 level) {
		AEAudioSetGroupVolume(bgm, level);
	}
	void unloadsound() {
		AEAudioUnloadAudio(Level);
		AEAudioUnloadAudio(LaserBlast);
		AEAudioUnloadAudio(Punch);
		AEAudioUnloadAudio(Error);
		AEAudioUnloadAudio(Pickup);
		AEAudioUnloadAudio(ArrowSound);
		AEAudioUnloadAudioGroup(bgm);
		AEAudioUnloadAudioGroup(soundEffects);
	}
}