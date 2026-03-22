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

    //trap tile textures
    TEX_TRAP1,
    TEX_TRAP2,
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

    // Level editor: static door thumbnail
    TEX_STATIC_DOOR,

    // Mushroom idle sheet (spritesheet used by animation system)
    TEX_MUSHROOM_IDLE_SHEET,

    // Add new textures above this line
    TEX_MAX
};

// Define all meshes here
enum MeshID {
    MESH_PLAYER,
    MESH_PLATFORM,
    MESH_PROJECTILE,
    MESH_ENEMY,
    MESH_DOOR,
    MESH_UI,
    MESH_MELEE_ENEMY, // animated melee enemy mesh (built by animSystem::buildMesh)
    MESH_TEST,        // pTestMesh used in Level1
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

    // Creates a 1x1 mesh and automatically stores it in the cache.
    static void BuildSqrMesh(MeshID id);

    // Registers a mesh pointer under name. 
    static void StoreMesh(MeshID id, AEGfxVertexList* pMesh);

    // Returns the cached mesh for name, or nullptr if not found.
    static AEGfxVertexList* GetMesh(MeshID id);

    // Calls AEGfxMeshFree on every cached mesh and clears the cache.
    static void FreeAllMeshes();

};