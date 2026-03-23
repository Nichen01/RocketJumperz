/* Start Header ************************************************************************/
/*!
\file         WeaponSprite.h
\brief        360-degree mouse-aimed weapon sprite that tracks the cursor.
              Handles rotation, horizontal/vertical flipping, and positional offset
              so the gun looks held in the player's hands.
*/
/* End Header **************************************************************************/
#pragma once

#include "AEEngine.h"
#include "player.h"

namespace weaponSprite {

    // ------------------------------------------------------------------
    // Load -- call once during Level_Load.
    // Loads both weapon textures (plasma gun + shotgun) into AssetManager.
    // ------------------------------------------------------------------
    void Load();

    // ------------------------------------------------------------------
    // Update -- call once per frame in Level_Update.
    // Calculates the aim angle from the player to the mouse cursor
    // and builds the gun's world-space transform matrix (Scale -> Rot -> Trans).
    // Also picks the correct texture based on the player's currentWeapon.
    // ------------------------------------------------------------------
    void Update(const objectsquares& playerObj);

    // ------------------------------------------------------------------
    // Draw -- call once per frame in Level_Draw, right after the player sprite.
    // Sets the active weapon texture, applies the cached transform, and draws.
    // ------------------------------------------------------------------
    void Draw();

    // ------------------------------------------------------------------
    // Unload -- call once during Level_Unload.
    // Nulls local texture pointers (AssetManager owns the memory).
    // ------------------------------------------------------------------
    void Unload();

}
