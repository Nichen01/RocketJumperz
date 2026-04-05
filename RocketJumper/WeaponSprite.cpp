/* Start Header ************************************************************************/
/*!
\file         WeaponSprite.cpp
\author       Nicholas Chen (c.chen)
\brief        Implements 360-degree mouse-aimed weapon sprite rendering.

              The gun sprite continuously rotates to point at the mouse cursor.
              When the mouse crosses to the left side of the player, both the
              player sprite (handled elsewhere) and the gun sprite are flipped
              so that the weapon stays visually correct:
                - Aiming RIGHT: positive X scale, positive Y scale
                - Aiming LEFT:  positive X scale, NEGATIVE Y scale (vertical flip)
                  The vertical flip prevents the gun from appearing upside-down
                  when the rotation exceeds +/-90 degrees.

              Transform order is Scale -> Rotate -> Translate, where translate
              incorporates the player position plus a small hand-offset.
*/
/* End Header **************************************************************************/

#include "WeaponSprite.h"
#include "AssetManager.h"
#include "movement.h"   // movement::playerFacingLeft, movement::getMouse
#include "draw.h"       // pMesh (shared unit quad)
#include <cmath>        // atan2f

namespace weaponSprite {

    // ====================================================================
    // Constants -- tweak these to adjust the gun's size and grip position
    // ====================================================================

    // How large the gun sprite is rendered (world units).
    // Roughly half the player's 80-unit tile so the gun isn't enormous.
    const f32 kGunScaleX = 75.0f;
    const f32 kGunScaleY = 45.0f;

    // Offset from the player's center to the "hand" position.
    // Positive X = forward (gun side), positive Y = upward.
    // These are in the player's LOCAL frame before rotation is applied.
    const f32 kGunOffsetX = 10.0f;
    const f32 kGunOffsetY = -5.0f;

    // ====================================================================
    // Module-level state
    // ====================================================================

    // Cached texture pointers (owned by AssetManager, not freed here)
    static AEGfxTexture* plasmaTexture  = nullptr;
    static AEGfxTexture* shotgunTexture = nullptr;

    // Points to whichever texture matches the player's currentWeapon.
    // Updated every frame in Update() so the Draw() call is simple.
    static AEGfxTexture* activeTexture  = nullptr;

    // The final world-space transform matrix, rebuilt every frame in Update
    static AEMtx33 gunTransform = { 0 };

    // ====================================================================
    // Load
    // ====================================================================
    void Load()
    {
        // Load both weapon textures into AssetManager
        AssetManager::LoadTexture(TEX_PLASMA_GUN, "Assets/Weapons/plasmaGun.png");
        AssetManager::LoadTexture(TEX_SHOTGUN,    "Assets/Weapons/shotGun.png");

        plasmaTexture  = AssetManager::GetTexture(TEX_PLASMA_GUN);
        shotgunTexture = AssetManager::GetTexture(TEX_SHOTGUN);

        // Default to plasma texture (matches WEAPON_PLASMA default)
        activeTexture = plasmaTexture;

        if (!plasmaTexture)
            printf("WeaponSprite: Failed to load plasmaGun.png!\n");
        if (!shotgunTexture)
            printf("WeaponSprite: Failed to load shotGun.png!\n");
    }

    // ====================================================================
    // Update
    // ====================================================================
    void Update(const objectsquares& playerObj)
    {
        // Refresh texture pointers in case AssetManager reloaded them
        plasmaTexture  = AssetManager::GetTexture(TEX_PLASMA_GUN);
        shotgunTexture = AssetManager::GetTexture(TEX_SHOTGUN);

        // Swap the active texture to match the player's equipped weapon
        if (playerObj.currentWeapon == WEAPON_SHOTGUN)
            activeTexture = shotgunTexture;
        else
            activeTexture = plasmaTexture;

        // ----------------------------------------------------------------
        // 1. Calculate aim angle: direction from player center to mouse
        //    getMouse returns a normalized direction vector, but we need
        //    the raw offset to compute a proper atan2 angle.
        //    We'll reuse the same screen-to-world math that getMouse uses.
        // ----------------------------------------------------------------
        s32 rawMouseX, rawMouseY;
        AEInputGetCursorPosition(&rawMouseX, &rawMouseY);

        // Convert screen coords to world coords relative to the player
        f32 dx = static_cast<f32>(rawMouseX)
               - static_cast<f32>(screenWidth  / 2)
               - playerObj.xPos;
        f32 dy = static_cast<f32>(screenLength / 2)
               - static_cast<f32>(rawMouseY)
               - playerObj.yPos;

        // atan2 gives us the angle from the positive X axis to the vector (dx, dy)
        f32 aimAngle = atan2f(dy, dx);

        // ----------------------------------------------------------------
        // 2. Determine facing direction for sprite flipping
        //    movement::playerFacingLeft is already updated before this call.
        // ----------------------------------------------------------------
        bool facingLeft = movement::playerFacingLeft;

        // ----------------------------------------------------------------
        // 3. Build the gun transform: Scale -> Rotate -> Translate
        // ----------------------------------------------------------------

        // --- SCALE ---
        // When aiming left the rotation wraps past 90 degrees, which would
        // make the gun appear upside-down. Flipping the Y scale (negative)
        // corrects this so the gun graphic stays right-side-up.
        f32 scaleX = kGunScaleX;
        f32 scaleY = facingLeft ? -kGunScaleY : kGunScaleY;

        AEMtx33 scaleMtx = { 0 };
        AEMtx33Scale(&scaleMtx, scaleX, scaleY);

        // --- ROTATE ---
        AEMtx33 rotateMtx = { 0 };
        AEMtx33Rot(&rotateMtx, aimAngle);

        // --- TRANSLATE ---
        // The gun pivots around the player's center, offset slightly toward the hand.
        // We rotate the offset by the aim angle so it always sits at the grip point.
        f32 rotatedOffsetX = kGunOffsetX * cosf(aimAngle) - kGunOffsetY * sinf(aimAngle);
        f32 rotatedOffsetY = kGunOffsetX * sinf(aimAngle) + kGunOffsetY * cosf(aimAngle);

        f32 worldX = playerObj.xPos + rotatedOffsetX;
        f32 worldY = playerObj.yPos + rotatedOffsetY;

        AEMtx33 translateMtx = { 0 };
        AEMtx33Trans(&translateMtx, worldX, worldY);

        // --- CONCATENATE: (Translate * (Rotate * Scale)) ---
        AEMtx33 temp = { 0 };
        AEMtx33Concat(&temp, &rotateMtx, &scaleMtx);        // temp = Rotate * Scale
        AEMtx33Concat(&gunTransform, &translateMtx, &temp);  // final = Translate * temp
    }

    // ====================================================================
    // Draw
    // ====================================================================
    void Draw()
    {
        if (!activeTexture || !pMesh)
            return;

        // Set up render state for the gun sprite
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        // Bind the active weapon texture (swapped in Update based on currentWeapon)
        AEGfxTextureSet(activeTexture, 0.0f, 0.0f);

        // Apply the cached transform and draw
        AEGfxSetTransform(gunTransform.m);
        AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
    }

    // ====================================================================
    // Unload
    // ====================================================================
    void Unload()
    {
        // The texture memory itself is freed by AssetManager::UnloadAllTextures()
        // which each level already calls. We just null our local pointers.
        plasmaTexture  = nullptr;
        shotgunTexture = nullptr;
        activeTexture  = nullptr;
    }

}
