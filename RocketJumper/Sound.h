/* Start Header ************************************************************************/
/*!
\file		  sound.h
\date         April, 04, 2026
\brief        Header file for sound.cpp

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

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
}