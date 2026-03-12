/* Start Header ************************************************************************/
/*!
\file         AssetManager.cpp
\brief        Implementation of the AssetManager namespace.

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "AssetManager.h"
#include "AEGraphics.h"
#include <cstdio>

namespace AssetManager {

    // -------------------------------------------------------------------------
    // Internal storage
    // -------------------------------------------------------------------------
    static std::unordered_map<std::string, AEGfxTexture*>    sTextures;
    static std::unordered_map<std::string, AEGfxVertexList*> sMeshes;

    // -------------------------------------------------------------------------
    // Textures
    // -------------------------------------------------------------------------

    AEGfxTexture* LoadTexture(const std::string& name, const std::string& filePath)
    {
        auto it = sTextures.find(name);
        if (it != sTextures.end())
            return it->second; // already loaded – skip duplicate load

        AEGfxTexture* tex = AEGfxTextureLoad(filePath.c_str());
        if (!tex)
            printf("[AssetManager] WARNING: Failed to load texture \"%s\" from \"%s\"\n",
                   name.c_str(), filePath.c_str());

        sTextures[name] = tex;
        return tex;
    }

    AEGfxTexture* GetTexture(const std::string& name)
    {
        auto it = sTextures.find(name);
        if (it != sTextures.end()) return it->second;

        printf("[AssetManager] WARNING: Texture \"%s\" not found.\n", name.c_str());
        return nullptr;
    }

    void UnloadAllTextures()
    {
        for (auto& pair : sTextures)
        {
            if (pair.second)
                AEGfxTextureUnload(pair.second);
        }
        sTextures.clear();
    }

    // -------------------------------------------------------------------------
    // Meshes
    // -------------------------------------------------------------------------
    // Creates a 1x1 mesh(full quad) and automatically stores it in the cache. 
    AEGfxVertexList* Build1x1Mesh(const std::string& name)
    {
        // Start mesh creation
        AEGfxMeshStart();

        // Triangle 1 (Bottom-Left, Bottom-Right, Top-Left)
        AEGfxTriAdd(
            -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
            0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
            -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

        // Triangle 2 (Bottom-Right, Top-Right, Top-Left)
        AEGfxTriAdd(
            0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
            0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
            -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

        // End creation and grab the pointer
        AEGfxVertexList* newMesh = AEGfxMeshEnd();

        // Prevent memory leak by freeing the old mesh if the name already exists
        auto it = sMeshes.find(name);
        if (it != sMeshes.end() && it->second != nullptr) {
            AEGfxMeshFree(it->second);
        }

        // Store it safely in the map
        sMeshes[name] = newMesh;

        // Return the pointer so the caller can use it immediately!
        return newMesh;
    }

    void StoreMesh(const std::string& name, AEGfxVertexList* mesh)
    {
        // Prevent memory leak by freeing the old mesh if the name already exists
        auto it = sMeshes.find(name);
        if (it != sMeshes.end() && it->second != nullptr) {
            AEGfxMeshFree(it->second);
        }

        sMeshes[name] = mesh;
    }

    AEGfxVertexList* GetMesh(const std::string& name)
    {
        auto it = sMeshes.find(name);
        if (it != sMeshes.end())
            return it->second;

        printf("[AssetManager] WARNING: Mesh \"%s\" not found.\n", name.c_str());
        return nullptr;
    }

    void FreeAllMeshes()
    {
        for (auto& pair : sMeshes)
        {
            if (pair.second)
                AEGfxMeshFree(pair.second);
        }
        sMeshes.clear();
    }

} // namespace AssetManager
