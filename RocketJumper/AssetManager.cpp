/* Start Header ************************************************************************/
/*!
\file         AssetManager.cpp
\author	      Nicholas Chen (c.chen)
\date         January, 31, 2026
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
    // To store the mehes and textures into an ordered map
    static std::unordered_map<std::string, AEGfxTexture*>    sTextures;
    static std::unordered_map<std::string, AEGfxVertexList*> sMeshes;

    // -------------------------------------------------------------------------
    // Textures
    // Load -> get -> unload
    // -------------------------------------------------------------------------
    /*!*************************************************************************
     * LOAD TEXTURE
     * @brief Loads a texture from the disk, caches it, and returns the pointer.
     *
     * Usage in level files: Call this in the level's Load() function to
     * ensure all necessary textures are loaded into memory before the level
     * begins. 
     * If a texture with the same name is already loaded, it will
     * return the existing texture to prevent duplicate loading.
     * * Example: AssetManager::LoadTexture("HeroTex", "Assets/Hero.png");
     *
     * @param name       The custom string identifier to cache the texture under
     * @param filePath   The relative or absolute path to the texture file
     * @return AEGfxTexture* ***************************************************************************/
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


    /*!*************************************************************************
     * GET TEXTURE
     * @brief Retrieves a previously loaded texture from the asset cache.
     *
     * Usage in level files: Call this during Initialize(), Update(), or Draw()
     * when you need to assign a texture to a game object or bind it for
     * rendering. 
     * - must ensure LoadTexture() was called first.
     * * Example: myObject.pTex = AssetManager::GetTexture("HeroTex");
     *
     * @param name       The custom string identifier of the desired texture
     * @return AEGfxTexture* (Returns nullptr if not found)
     ***************************************************************************/
    AEGfxTexture* GetTexture(const std::string& name)
    {
        auto it = sTextures.find(name);
        if (it != sTextures.end()) return it->second;

        printf("[AssetManager] WARNING: Texture \"%s\" not found.\n", name.c_str());
        return nullptr;
    }

    /*!*************************************************************************
     * UNLOAD ALL TEXTURES
     * @brief Frees all cached textures from memory.
     *
     * Usage in level files: Call this inside the level's UNLOAD() function
     * to prevent memory leaks when transitioning to a new level or quitting.
     * * Example: AssetManager::UnloadAllTextures();
     *
     * @return VOID
     ***************************************************************************/
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
    // MESHES
    // -------------------------------------------------------------------------
    /*!*************************************************************************
     * BUILD 1X1 MESH
     * @brief Creates a 1x1 quad mesh with UVs and stores it in the cache.
     *
     * Usage: Call in LOAD() or INIT() to generate standard square meshes for 2D sprites. 
     * It automatically handles CREATING and RETURNING the new mesh. 
     * NO NEED TO STORE MESH 
     * * Example: AssetManager::Build1x1Mesh("QuadMesh");
     *
     * @param name       To name the mesh
     * @return AEGfxVertexList*
     ***************************************************************************/
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

    /*!*************************************************************************
     * STORE MESH
     * @brief Saves a custom mesh that you have manually built into the manager's memory. 
     * You give it a name and the mesh data
     *
     * Usage: Use this in Load() IF you have built a custom
     * shape (usually for animination meshes) using AEGfxMeshStart/End and want
     * the AssetManager to handle its lifecycle.
     * * Example: AssetManager::StoreMesh("CustomTriangle", myTriMeshPtr);
     *
     * @param name       The custom string identifier to cache the mesh under
     * @param mesh       Pointer to the manually constructed AEGfxVertexList
     * @return VOID
     ***************************************************************************/
    void StoreMesh(const std::string& name, AEGfxVertexList* mesh)
    {
        // Search the mesh map
        auto it = sMeshes.find(name);
        if (it != sMeshes.end() && it->second != nullptr) {
            AEGfxMeshFree(it->second);
        }

        // store the mesh pointer in the unordered map under the name
        sMeshes[name] = mesh;
    }


    /*!*************************************************************************
     * GET MESH
     * @brief Retrieve a mesh that is already being held by the manager. 
     * You ask for it by its name, and the manager hands you the pointer so you can use it to draw your object.
     *
     * Usage: Call in Inite() or Draw() to ASSIGN
     * the mesh to a game object or bind it for drawing.
     * * Example: myObject.pMesh = AssetManager::GetMesh("QuadMesh");
     *
     * @param name       The custom string identifier of the desired mesh
     * @return AEGfxVertexList* (Returns nullptr if not found)
     ***************************************************************************/
    AEGfxVertexList* GetMesh(const std::string& name)
    {
        // Search the texture map for the provided name
        auto it = sMeshes.find(name);
        // If mesh alrdy exists, return pointer to that mesh
        if (it != sMeshes.end())
            return it->second;

        // if path/mesh not found, print warning and return null
        printf("[AssetManager] WARNING: Mesh \"%s\" not found.\n", name.c_str());
        return nullptr;
    }


    /*!*************************************************************************
     * FREE ALL MESHES
     * @brief Frees all cached meshes from GPU memory and clears the map.
     *
     * Usage in level files: Call this inside your level's Unload() function
     * alongside UnloadAllTextures() to ensure a clean state for the next level.
     * * Example: AssetManager::FreeAllMeshes();
     *
     * @return VOID
     ***************************************************************************/
    void FreeAllMeshes()
    {
        // Loop through every key-value pair in the mesh map
        for (auto& pair : sMeshes)
        {
            // Check if mesh pointer is valid
            if (pair.second) AEGfxMeshFree(pair.second); // delete from memory
        }
        // Remove all entries from the map now that memory is freed
        sMeshes.clear();
    }

} // namespace AssetManager
