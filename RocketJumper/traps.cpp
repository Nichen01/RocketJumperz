#include "traps.h"
#include "collision.h"
#include "player.h"
#include "enemies.h"

namespace traps {
	s8 trapDamage = 5;
	s8 trapInstanceCooldown = 0;
	void updateTraps(Enemy enemies[],objectsquares objectinfo[],int* map,int x, int s) {
		trapInstanceCooldown -= 1;
		float NposX = gamelogic::posX_to_index(objectinfo[player].xPos+ objectinfo[player].velocityX, (float)s);
		float NposY = gamelogic::posY_to_index(objectinfo[player].yPos + objectinfo[player].velocityY, (float)s);
		float NScaleX = objectinfo[player].xScale / s;
		float NScaleY = objectinfo[player].yScale / s;
		if (gamelogic::CheckInstanceBinaryMapCollision(NposX, NposY, NScaleX, NScaleY, map, 2, x)) {
			if (trapInstanceCooldown <= 0.0f)
			{
				// Try to apply damage through the central health system.
				// PlayerTakeDamage checks the player's invincibility timer.
				if (PlayerTakeDamage(objectinfo[player], trapDamage))
				{
					//reset the attack cooldown, to prevent damage every frame
					trapInstanceCooldown = trapCooldown;
				}
			}
		}
		gamelogic::Collision_movement(&enemies[0].shape, map, x, s, 2);
		gamelogic::Collision_movement(&enemies[1].shape, map, x, s, 2);
		gamelogic::Collision_movement(&objectinfo[player], map, x, s, 2);
	}
}