/* Start Header ************************************************************************/
/*!
\file         AssetManager.h
\brief        Centralized cache for AEGfxTexture* and AEGfxVertexList* assets.
              Prevents duplicate loads and provides a single unload/free point
              for each level's Load/Unload and Initialize/Free phases.

              Usage:
                Load phase   -> AssetManager::LoadTexture("name", "path/to/file.png")
                Use          -> AssetManager::GetTexture("name")
                Unload phase -> AssetManager::UnloadAllTextures()

                Build phase  -> AssetManager::StoreMesh("name", pMeshPtr)
                Use          -> AssetManager::GetMesh("name")
                Free phase   -> AssetManager::FreeAllMeshes()

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include <string>
#include <unordered_map>
#include "AEEngine.h"

namespace AssetManager {

    // -------------------------------------------------------------------------
    // Textures
    // -------------------------------------------------------------------------

    // Loads the texture at filePath and caches it under name.
    // If name is already cached the existing pointer is returned without
    // reloading.  Returns nullptr on load failure.
    AEGfxTexture*    LoadTexture(const std::string& name, const std::string& filePath);

    // Returns the cached texture for name, or nullptr if not found.
    AEGfxTexture*    GetTexture(const std::string& name);

    // Calls AEGfxTextureUnload on every cached texture and clears the cache.
    void             UnloadAllTextures();

    // -------------------------------------------------------------------------
    // Meshes  (built procedurally, not loaded from a file)
    // -------------------------------------------------------------------------
    // Creates a 1x1 mesh and automatically stores it in the cache.
    AEGfxVertexList* Build1x1Mesh(const std::string& name);

    // Registers a mesh pointer under name.  Overwrites any previous entry
    // without freeing it, so only call once per name per lifecycle.
    void             StoreMesh(const std::string& name, AEGfxVertexList* mesh);

    // Returns the cached mesh for name, or nullptr if not found.
    AEGfxVertexList* GetMesh(const std::string& name);

    // Calls AEGfxMeshFree on every cached mesh and clears the cache.
    void             FreeAllMeshes();

} // namespace AssetManager
