/* Start Header ************************************************************************/
/*!
\file         InstructionsMenu.h
\brief        In-game "?" icon overlay that pauses gameplay and shows an instructions image.
              Integrated into each level's Load/Init/Update/Draw/Unload cycle.
*/
/* End Header **************************************************************************/
#pragma once

#include "AEEngine.h"

// ---------------------------------------------------------------------------
// InstructionsMenu -- static helper namespace (no instances needed).
// Each level calls these functions in its own game-state callbacks.
// ---------------------------------------------------------------------------
namespace InstructionsMenu {

    // Load textures through AssetManager (call in each level's Load).
    void Load();

    // Reset the overlay state so it starts closed (call in each level's Init).
    void Init();

    // Handle "?" icon click and Back / Escape to close.
    // Returns true when the menu is ACTIVE (meaning the caller should early-return
    // from its own Update to pause gameplay).
    bool Update();

    // Draw the "?" icon during normal play, and the full overlay when active.
    // Call at the very END of each level's Draw so it renders on top of everything.
    void Draw();

    // Unload textures through AssetManager (call in each level's Unload).
    // Textures are already freed by AssetManager::UnloadAllTextures(), so this
    // is intentionally a no-op -- provided for symmetry and future-proofing.
    void Unload();
}
