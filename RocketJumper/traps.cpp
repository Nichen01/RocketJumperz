#include "traps.h"
#include "collision.h"
#include "player.h"
#include "enemies.h"
#include "binaryMap.h"

namespace traps {
	s8 trapDamage = 5;
	s8 trapInstanceCooldown = 0;
	s8 trapRange = 3;
	bool nearTrap = 0;
	AEVec2 nearestTrap = {};
	void initTraps() {
		nearTrap = 0;
		AEVec2Zero(&nearestTrap);
	}
	AEVec2 checkNearestTrap(float NposX,float NposY, int* map, int x) {
		nearTrap = 0;
		AEVec2 temp = {};
		for (int dx = -trapRange; dx <= trapRange; ++dx) {
			for (int dy = -trapRange; dy <= trapRange; ++dy) {
				int checkX = static_cast<int>(NposX + dx);
				int checkY = static_cast<int>(NposY + dy);
				if (checkX < 0) checkX = 0;
				if (checkY < 0) checkY = 0;
				if (map[checkY*x+checkX] == 2) {
					nearTrap = 1;
					AEVec2Set(&temp, static_cast<f32>(checkX), static_cast<f32>(checkY));
					return temp;
				}
			}
		}
		return temp;
	}
	void suckPlayer(objectsquares& player, AEVec2 trap,float s) {
		float trapX = gamelogic::index_to_posX(static_cast<f32>(trap.x+0.5), s);
		float trapY = gamelogic::index_to_posY(static_cast<f32>(trap.y+0.5), s);
		f32 dx = player.xPos - trapX;
		f32 dy = player.yPos - trapY;
		f32 dist = sqrtf(dx * dx + dy * dy);
		player.velocityX -= dx / dist * trapSuction;
		player.velocityY -= dy / dist * trapSuction;
		
	}
	void updateTraps(Enemy enemies[],objectsquares objectinfo[],int* map,int x, float s) {
		trapInstanceCooldown -= 1;
		float NposX = gamelogic::posX_to_index(objectinfo[player].xPos+ objectinfo[player].velocityX, s);
		float NposY = gamelogic::posY_to_index(objectinfo[player].yPos + objectinfo[player].velocityY, s);
		float NScaleX = objectinfo[player].xScale / s;
		float NScaleY = objectinfo[player].yScale / s;
		nearestTrap = checkNearestTrap(NposX, NposY, map, x);
		if (nearTrap) suckPlayer(objectinfo[player], nearestTrap, s);
		if (gamelogic::CheckInstanceBinaryMapCollision(NposX, NposY, NScaleX, NScaleY, map, 2, x)) {
			if (trapInstanceCooldown <= 0.0f)
			{
				if (PlayerTakeDamage(objectinfo[player], trapDamage))
				{
					trapInstanceCooldown = trapCooldown;
				}
			}
		}
		gamelogic::Collision_movement(&enemies[0].shape, map, x, (int)s, 2);
		gamelogic::Collision_movement(&enemies[1].shape, map, x, (int)s, 2);
		gamelogic::Collision_movement(&objectinfo[player], map, x, (int)s, 2);
	}
}