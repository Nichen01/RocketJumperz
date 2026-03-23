/* Start Header ************************************************************************/
/*!
\file         AssetManager.h
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

    // UI textures
    TEX_CTRL1,
    TEX_CTRL2,
    TEX_LEFT_ARROW,
    TEX_RIGHT_ARROW,
    TEX_LEFT_CLICK,
    TEX_RIGHT_CLICK,
    TEX_E_BUTTON,
    TEX_G_BUTTON,
    TEX_S_BUTTON,
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

    // Ranged enemy state spritesheets
    TEX_RANGED_IDLE,    // RangedMove.png frame 0 (reused as idle)
    TEX_RANGED_MOVE,    // RangedMove.png   (1 row, 4 cols)
    TEX_RANGED_ATTACK,  // RangedAttack.png (1 row, 6 cols)
    TEX_RANGED_DEATH,   // RangedDeath.png  (1 row, 4 cols)
    TEX_RANGED_HURT,    // RangedHurt.png   (1 row, 2 cols)

    // Key texture
    TEX_KEYCARD,

    // Error prompt
    TEX_ERRORPROMPT,
    TEX_PROMPT1,
    TEX_PROMPT2,
    TEX_PROMPT3,
    TEX_PROMPT4,
    TEX_PROMPT5,
    TEX_PROMPT6,
    TEX_PROMPT7,
    TEX_PROMPT8,
    TEX_PROMPT9,
    TEX_REDBUTTON,
    TEX_GREENBUTTON,

    // Inventory texture
    TEX_INVENTORY,
    TEX_KEYCARDINVENTORY,

    // Broken door(s)
    TEX_BROKENDOOR0,
    TEX_BROKENDOOR1,
    TEX_BROKENDOOR2,

    // Wire Inventory
    TEX_WIREINVENTORY0,
    TEX_WIREINVENTORY1,
    TEX_WIREINVENTORY2,
    // Weapon textures
    TEX_PLASMA_GUN,     // Assets/Weapons/plasmaGun.png
    TEX_SHOTGUN,        // Assets/Weapons/shotGun.png

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