/* Start Header ************************************************************************/
/*!
\file         AssetManager.h
\author       Nicholas Chen (c.chen)
\brief        Centralized cache for AEGfxTexture* and AEGfxVertexList* assets.
*/
/* End Header **************************************************************************/
#pragma once

#include "pch.h" // Use pch.h to bring in AEEngine types for Visual Studio

// Define all possible textures in Rocket Jumperz here
enum TextureID {
    TEX_PLAYER,
    TEX_PLASMA,
    TEX_DOOR,
    TEX_MELEE_ENEMY,
    TEX_RANGED_ENEMY,
    TEX_BASE5TEST,

    // Platform tile textures (9 variants)
    TEX_PLATFORM1,
    TEX_PLATFORM2,
    TEX_PLATFORM3,
    TEX_PLATFORM4,
    TEX_PLATFORM5,
    TEX_PLATFORM6,
    TEX_PLATFORM7,
    TEX_PLATFORM8,
    TEX_PLATFORM9,

    // Glass tile textures (5 variants)
    TEX_GLASS0,
    TEX_GLASS1,
    TEX_GLASS2,
    TEX_GLASS3,
    TEX_GLASS4,

    //trap tile textures
    TEX_EYETRAP,
    TEX_TRAPBEAM,

    // UI textures
    TEX_CTRL1,
    TEX_CTRL2,
    TEX_LEFT_ARROW,
    TEX_RIGHT_ARROW,
    TEX_LEFT_CLICK,
    TEX_RIGHT_CLICK,
    TEX_E_BUTTON,
    TEX_G_BUTTON,
    TEX_L_BUTTON,
    TEX_S_BUTTON,
    TEX_Q_BUTTON,
    TEX_Z_BUTTON,
    TEX_BUTTON1,
    TEX_BUTTON2,
    TEX_BUTTON3,
    TEX_BUTTON4,
    TEX_SPACEBAR,

    // Mushroom animation frames (individual textures)
    TEX_MUSHROOM_DIE0,
    TEX_MUSHROOM_DIE1,
    TEX_MUSHROOM_DIE2,
    TEX_MUSHROOM_DIE3,
    TEX_MUSHROOM_DIE4,
    TEX_MUSHROOM_DIE5,
    TEX_MUSHROOM_DIE6,
    TEX_MUSHROOM_DIE7,
    TEX_MUSHROOM_DIE8,

    TEX_MUSHROOM_HIT0,
    TEX_MUSHROOM_HIT1,
    TEX_MUSHROOM_HIT2,
    TEX_MUSHROOM_HIT3,
    TEX_MUSHROOM_HIT4,

    TEX_MUSHROOM_IDLE0,
    TEX_MUSHROOM_IDLE1,
    TEX_MUSHROOM_IDLE2,
    TEX_MUSHROOM_IDLE3,
    TEX_MUSHROOM_IDLE4,
    TEX_MUSHROOM_IDLE5,
    TEX_MUSHROOM_IDLE6,

    // Main Menu textures
    TEX_MAIN_MENU_BG,
    TEX_TITLE,
    TEX_DIGIPEN_LOGO,   // DigiPenWhite.png used on the credits screen

    // Level editor: static door thumbnail
    TEX_STATIC_DOOR,

    // Mushroom idle sheet (spritesheet used by animation system)
    TEX_MUSHROOM_IDLE_SHEET,

    // Key texture
    TEX_KEYCARD,

    TEX_REDBUTTON,
    TEX_GREENBUTTON,

    // Inventory texture
    TEX_INVENTORY,
    TEX_KEYCARDINVENTORY,

    // Broken door(s)
    TEX_BROKENDOOR0,
    TEX_BROKENDOOR1,
    TEX_BROKENDOOR2,

    // Wire (world drop item)
    TEX_WIRE,

    // Wire Inventory
    TEX_WIREINVENTORY0,
    TEX_WIREINVENTORY1,
    TEX_WIREINVENTORY2,
    TEX_WIREINVENTORY3,
    //ammo drop
    TEX_DROP,
    // Ranged enemy state spritesheets
    TEX_RANGED_IDLE,    // RangedMove.png frame 0 (reused as idle)
    TEX_RANGED_MOVE,    // RangedMove.png   (1 row, 4 cols)
    TEX_RANGED_ATTACK,  // RangedAttack.png (1 row, 6 cols)
    TEX_RANGED_DEATH,   // RangedDeath.png  (1 row, 4 cols)
    TEX_RANGED_HURT,    // RangedHurt.png   (1 row, 2 cols)

    // Weapon textures
    TEX_PLASMA_GUN,     // Assets/Weapons/plasmaGun.png
    TEX_SHOTGUN,        // Assets/Weapons/shotGun.png

    TEX_COOLDOWNBAR0,
    TEX_COOLDOWNBAR1,
    TEX_COOLDOWNBAR2,
    TEX_COOLDOWNBAR3,
    TEX_COOLDOWNBAR4,
    TEX_COOLDOWNBAR5,

    TEX_PADLOCK,
    TEX_MENU,
    TEX_BUTTON,

    TEX_BACKGROUND,
    TEX_BACKGROUND2,

    // Saw trap spritesheet (1 row, 7 columns)
    TEX_STATICSAW,
    TEX_SAW,

    // Instructions menu full-screen image
    TEX_INSTRUCTIONS_MENU,

    // "?" icon shown in gameplay HUD (opens instructions overlay)
    TEX_INSTRUCTIONS_ICON,

    // Health pack drop
    TEX_HEALTH,
    TEX_SETTINGS,
    TEX_DOORPROMPT,
    TEX_CANCEL,

    // Background wall tile (non-collidable, drawn over glass)
    TEX_WALL_BG_1,

    // Border tile textures (8 directional variants, used for wall borders)
    TEX_BORDER_TL,  // top-left corner
    TEX_BORDER_T,   // top edge
    TEX_BORDER_TR,  // top-right corner
    TEX_BORDER_CL,  // center-left edge
    TEX_BORDER_CR,  // center-right edge
    TEX_BORDER_BL,  // bottom-left corner
    TEX_BORDER_B,   // bottom edge
    TEX_BORDER_BR,  // bottom-right corner

    // Volume control button textures (pause menu sound bar)
    TEX_VOLUME_UP,
    TEX_VOLUME_DOWN,

    // Add new textures above this line
    TEX_MAX
};

// Define all meshes here
enum MeshID {
    MESH_QUAD,        // single reusable 1x1 quad for all standard static objects
    MESH_MELEE_ENEMY, // animated melee enemy mesh (spritesheet-sliced)
    MESH_DOOR,        // animated door mesh (spritesheet-sliced)
    MESH_MENU_BUTTON, // unit quad with black vertex color for color-mode button rendering
    MESH_RANGED_MOVE,   // spritesheet mesh (1 row, 4 cols)
    MESH_RANGED_ATTACK, // spritesheet mesh (1 row, 6 cols)
    MESH_RANGED_DEATH,  // spritesheet mesh (1 row, 4 cols)
    MESH_RANGED_HURT,   // spritesheet mesh (1 row, 2 cols)
    MESH_BUTTON,
    MESH_SAW,       // spritesheet mesh (1 row, 7 cols) for saw trap animation
    MESH_PARTICLE,  // 1x1 white quad used by the particle system
    // Add new meshes above this line
    MESH_MAX
};

class AssetManager {
private:
    static AEGfxTexture* sTextures[TEX_MAX];
    static AEGfxVertexList* sMeshes[MESH_MAX];

public:

    // -------------------------------------------------------------------------
    // Textures
    // -------------------------------------------------------------------------

    // Loads the texture at filePath and caches it under name.
    static void LoadTexture(TextureID id, const char* path);

    // Returns the cached texture for name, or nullptr if not found.
    static AEGfxTexture* GetTexture(TextureID id);

    // Calls AEGfxTextureUnload on every cached texture and clears the cache.
    static void UnloadAllTextures();

    // -------------------------------------------------------------------------
    // Meshes  (built procedurally, not loaded from a file)
    // -------------------------------------------------------------------------

    // Creates a square mesh and automatically stores it in the cache.
    // When rows/cols are 1 (default), builds a standard 1x1 quad.
    // When rows/cols > 1, slices UVs for spritesheet animation frames.
    static void BuildSqrMesh(MeshID id, int rows = 1, int cols = 1, u32 vertexColor = 0xFFFFFFFF);

    // Registers a mesh pointer under name. 
    static void StoreMesh(MeshID id, AEGfxVertexList* pMesh);

    // Returns the cached mesh for name, or nullptr if not found.
    static AEGfxVertexList* GetMesh(MeshID id);

    // Calls AEGfxMeshFree on every cached mesh and clears the cache.
    static void FreeAllMeshes();

};