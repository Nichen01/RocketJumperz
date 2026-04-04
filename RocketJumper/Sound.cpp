/* Start Header ************************************************************************/
/*!
\file		  sound.cpp
\date         April, 04, 2026
\brief        Contain functions to handle audio

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Sound.h"

AEAudio Level;
AEAudio LaserBlast;
AEAudio Punch;
AEAudio Error;
AEAudio Pickup;
AEAudio ArrowSound;
AEAudio GravityToggle;
AEAudio MainMenu;
AEAudioGroup bgm;
AEAudioGroup soundEffects;
f32 MainVolume = 1.0f;
s8 sCount = 10;

namespace audio {
	void loadsound() {
		// Load the music file once when the level loads
		Level = AEAudioLoadMusic("Assets/Sounds/L1_bgm.mp3");
		MainMenu = AEAudioLoadMusic("Assets/Sounds/mainmenu.mp3");

		// Create the audio group
		bgm = AEAudioCreateGroup();

		// Configure sound effects
		LaserBlast = AEAudioLoadSound("Assets/Sounds/LaserBlast.mp3");
		Punch = AEAudioLoadSound("Assets/Sounds/Punch.wav");
		Error = AEAudioLoadSound("Assets/Sounds/Error.wav");
		Pickup = AEAudioLoadSound("Assets/Sounds/Pickup.wav");
		ArrowSound = AEAudioLoadSound("Assets/Sounds/ArrowSound.wav");
		GravityToggle = AEAudioLoadSound("Assets/Sounds/gravity.mp3");
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
		AEAudioUnloadAudio(GravityToggle);
		AEAudioUnloadAudio(MainMenu);
		AEAudioUnloadAudioGroup(bgm);
		AEAudioUnloadAudioGroup(soundEffects);
	}
}