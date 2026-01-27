/* Start Header ************************************************************************/
/*!
\file		  physicsEngine.cpp
\author       Jeremiah He Shuo, jeremiahshuo.he,2400702
\par          jeremiahshuo.he@digipen.edu
\date         January, 19, 2026
\brief        Contains functions to simulate physics

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "physicsEngine.h"

#include <cmath>



extern int screenWidth; extern int screenLength;
namespace physics {
	f32 terminalVelocity, gravity, absoluteRecoil; //maximum speed, strength, recoil strength
	//KIV additions: variable weight(for gun),variable recoil
	AEVec2 playerVelocity; //directional velocity
	s32 bulletCount, jetPackCooldown;
	bool enableGravity;
	s32 mouseX, mouseY;
	void initPhysics() {
		playerVelocity.y = 0;
		playerVelocity.x = 0;
		terminalVelocity = 10;
		gravity = 0.1;
		absoluteRecoil = 10;
		bulletCount = 90;
		jetPackCooldown = 10;
		enableGravity = 1;
	}
	void physicsInput() {
		if (AEInputCheckTriggered(AEVK_G)) {
			enableGravity = (enableGravity) ? 0 : 1;
			printf("%d",enableGravity);
		}
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			AEInputGetCursorPosition(&mouseX, &mouseY);
			mouseX -= (screenWidth / 2);
			mouseY -= (screenLength / 2);
			f32 mouseDistance = static_cast<f32>(sqrt(mouseX * mouseX + mouseY * mouseY));
			playerVelocity.x -= (mouseX / mouseDistance) * absoluteRecoil;
			playerVelocity.y += (mouseY / mouseDistance) * absoluteRecoil;
			bulletCount -= 1;
		}
		if (AEInputCheckTriggered(AEVK_R)) {
			initPhysics();
		}
	}
	void updatePhysics(objectsquares &player) {
		if (enableGravity) {
			playerVelocity.y -= gravity;
		}
		f32 velocity = static_cast<f32>(sqrt(playerVelocity.y * playerVelocity.y + playerVelocity.x + playerVelocity.x));
		if (velocity > terminalVelocity) {
			playerVelocity.x = playerVelocity.x / velocity * terminalVelocity;
			playerVelocity.y = playerVelocity.y / velocity * terminalVelocity;
		}
		printf("velocity: %.2f \n", velocity);
		player.velocityX = playerVelocity.x;
		player.velocityY = playerVelocity.y;
	}
}
