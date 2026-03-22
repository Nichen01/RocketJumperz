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
    // Loads the plasma gun texture into the AssetManager cache.
    // ------------------------------------------------------------------
    void Load();

    // ------------------------------------------------------------------
    // Update -- call once per frame in Level_Update.
    // Calculates the aim angle from the player to the mouse cursor
    // and builds the gun's world-space transform matrix (Scale -> Rot -> Trans).
    // ------------------------------------------------------------------
    void Update(const objectsquares& playerObj);

    // ------------------------------------------------------------------
    // Draw -- call once per frame in Level_Draw, right after the player sprite.
    // Sets the gun texture, applies the cached transform, and draws the mesh.
    // ------------------------------------------------------------------
    void Draw();

    // ------------------------------------------------------------------
    // Unload -- call once during Level_Unload.
    // Releases the gun texture through AssetManager (handled by
    // AssetManager::UnloadAllTextures, but this nulls the local pointer).
    // ------------------------------------------------------------------
    void Unload();

}
