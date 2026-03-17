/* Start Header ************************************************************************/
/*!
\file         AssetManager.cpp
\brief        Implementation of the centralized asset registry.
*/
/* End Header **************************************************************************/

#include "pch.h"
#include "AssetManager.h"
#include <iostream>

// Initialize the pointer arrays to null
AEGfxTexture* AssetManager::sTextures[TEX_MAX] = { nullptr };
AEGfxVertexList* AssetManager::sMeshes[MESH_MAX] = { nullptr };

// --- TEXTURES ---
/*!*************************************************************************
 * LOAD TEXTURE
 * @brief Loads a texture from a file path and caches it in the centralized
 * array at the specified ID. Prevents memory leaks by freeing any
 * existing texture at that ID before loading.
 * Usage: AssetManager::LoadTexture(TEX_PLAYER, "Assets/player.png");
 *
 * @param id          The TextureID enum value to store the texture at
 * @param path        File path string pointing to the texture asset
 * @return VOID
 ***************************************************************************/
void AssetManager::LoadTexture(TextureID id, const char* path) {
    // Safety check: Ensure the requested ID isn't negative or larger than our array size.
    // If it accesses index 12 in an 11-slot array, the game will crash (Out of Bounds). 
    if (id < 0 || id >= TEX_MAX) {
        std::cerr << "AssetManager Error: Invalid TextureID!" << std::endl;
        return;
    }

    // If a texture already exists at this ID, free it to prevent leaks
    if (sTextures[id] != nullptr) {
        AEGfxTextureUnload(sTextures[id]);
    }

    // load the new texture from the path
    sTextures[id] = AEGfxTextureLoad(path);
     
    // if that path returns null, then path is wrong
    if (sTextures[id] == nullptr) {
        std::cerr << "AssetManager Error: Failed to load texture at " << path << std::endl;
    }
}

/*!*************************************************************************
 * GET TEXTURE
 * @brief Retrieves a cached texture pointer from the centralized array.
 * Usage: AEGfxTextureSet(AssetManager::GetTexture(TEX_PLAYER), 0, 0);
 *
 * @param id          The TextureID enum value of the desired texture
 * @return AEGfxTexture* Pointer to the texture, or nullptr if invalid/unloaded
 ***************************************************************************/
AEGfxTexture* AssetManager::GetTexture(TextureID id) {
    // validity check
    if (id < 0 || id >= TEX_MAX) return nullptr;
    // return the memory address at index
    return sTextures[id];
}

/*!*************************************************************************
 * UNLOAD ALL TEXTURES
 * @brief Iterates through the texture array, safely unloads all active
 * textures from memory, and auto-nulls the pointers. Best used in
 * Level_Unload() functions.
 * Usage: AssetManager::UnloadAllTextures();
 *
 * @return VOID
 ***************************************************************************/
void AssetManager::UnloadAllTextures() {
    // loop through and check if the texture is still being stored in mem, if so then unload it and set to null
    for (int i = 0; i < TEX_MAX; ++i) {
        if (sTextures[i] != nullptr) {
            AEGfxTextureUnload(sTextures[i]);
            sTextures[i] = nullptr; // True auto-nulling
        }
    }
}

// --- MESHES ---
/*!*************************************************************************
 * BUILD SQUARE MESH
 * @brief Procedurally builds a standard 1x1 square mesh and caches it in
 * the centralized array at the specified ID.
 * Usage: AssetManager::BuildSqrMesh(MESH_PLAYER);
 *
 * @param id          The MeshID enum value to store the new mesh at
 * @return VOID
 ***************************************************************************/
void AssetManager::BuildSqrMesh(MeshID id) {
    // check if the mesh id is valid
    if (id < 0 || id >= MESH_MAX) return;

    // if the mesh is live, then free it 
    if (sMeshes[id] != nullptr) {
        AEGfxMeshFree(sMeshes[id]);
    }

    //build square mesh with 2 triangles
    AEGfxMeshStart();
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f
    );
    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f
    );

    // assign mesh to the array id
    sMeshes[id] = AEGfxMeshEnd();
}

/*!*************************************************************************
 * STORE MESH
 * @brief Registers a manually built mesh pointer (like animated sprite meshes)
 * into the centralized array at the specified ID for automatic tracking.
 * Usage: AssetManager::StoreMesh(MESH_DOOR, pMyDoorMesh);
 *
 * @param id          The MeshID enum value to store the custom mesh at
 * @param pMesh       Pointer to the custom AEGfxVertexList mesh to be tracked
 * @return VOID
 ***************************************************************************/
void AssetManager::StoreMesh(MeshID id, AEGfxVertexList* pMesh) {
    // Ensure that the ID is valid
    if (id < 0 || id >= MESH_MAX) return;

    // if the mesh is still live, then free it 
    if (sMeshes[id] != nullptr) {
        AEGfxMeshFree(sMeshes[id]);
    }
    // assign the mesh to that array id
    sMeshes[id] = pMesh;
}

/*!*************************************************************************
 * GET MESH
 * @brief Retrieves a cached mesh pointer from the centralized array in
 * O(1) time using its ID.
 * Usage: AEGfxMeshDraw(AssetManager::GetMesh(MESH_PLAYER), AE_GFX_MDM_TRIANGLES);
 *
 * @param id          The MeshID enum value of the desired mesh
 * @return AEGfxVertexList* Pointer to the mesh, or nullptr if invalid/unloaded
 ***************************************************************************/
AEGfxVertexList* AssetManager::GetMesh(MeshID id) {
    // check if the mesh id is invalid
    if (id < 0 || id >= MESH_MAX) return nullptr;
    // return the mesh pointer stored at this array index
    return sMeshes[id];
}

/*!*************************************************************************
 * FREE ALL MESHES
 * @brief Iterates through the mesh array, safely frees all tracked meshes
 * from memory, and auto-nulls the pointers. Best used in
 * Level_Free() functions.
 * Usage: AssetManager::FreeAllMeshes();
 *
 * @return VOID
 ***************************************************************************/
void AssetManager::FreeAllMeshes() {
    // Loop through every single index in our mesh array.
    for (int i = 0; i < MESH_MAX; ++i) {
        // if mesh is still live, then free it and null 
        if (sMeshes[i] != nullptr) {
            AEGfxMeshFree(sMeshes[i]);
            sMeshes[i] = nullptr; 
        }
    }
}