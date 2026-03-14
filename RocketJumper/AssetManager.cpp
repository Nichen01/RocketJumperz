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
AEGfxTexture* AssetManager::s_Textures[TEX_MAX] = { nullptr };
AEGfxVertexList* AssetManager::s_Meshes[MESH_MAX] = { nullptr };

// --- TEXTURES ---

void AssetManager::LoadTexture(TextureID id, const char* path) {
    if (id < 0 || id >= TEX_MAX) {
        std::cerr << "AssetManager Error: Invalid TextureID!" << std::endl;
        return;
    }

    // If a texture already exists at this ID, free it first to prevent leaks
    if (s_Textures[id] != nullptr) {
        AEGfxTextureUnload(s_Textures[id]);
    }

    s_Textures[id] = AEGfxTextureLoad(path);

    if (s_Textures[id] == nullptr) {
        std::cerr << "AssetManager Error: Failed to load texture at " << path << std::endl;
    }
}

AEGfxTexture* AssetManager::GetTexture(TextureID id) {
    if (id < 0 || id >= TEX_MAX) return nullptr;
    return s_Textures[id];
}

void AssetManager::UnloadAllTextures() {
    for (int i = 0; i < TEX_MAX; ++i) {
        if (s_Textures[i] != nullptr) {
            AEGfxTextureUnload(s_Textures[i]);
            s_Textures[i] = nullptr; // True auto-nulling
        }
    }
}

// --- MESHES ---

void AssetManager::BuildSqrMesh(MeshID id) {
    if (id < 0 || id >= MESH_MAX) return;

    if (s_Meshes[id] != nullptr) {
        AEGfxMeshFree(s_Meshes[id]);
    }

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

    s_Meshes[id] = AEGfxMeshEnd();
}

void AssetManager::StoreMesh(MeshID id, AEGfxVertexList* pMesh) {
    if (id < 0 || id >= MESH_MAX) return;

    if (s_Meshes[id] != nullptr) {
        AEGfxMeshFree(s_Meshes[id]);
    }

    s_Meshes[id] = pMesh;
}

AEGfxVertexList* AssetManager::GetMesh(MeshID id) {
    if (id < 0 || id >= MESH_MAX) return nullptr;
    return s_Meshes[id];
}

void AssetManager::FreeAllMeshes() {
    for (int i = 0; i < MESH_MAX; ++i) {
        if (s_Meshes[i] != nullptr) {
            AEGfxMeshFree(s_Meshes[i]);
            s_Meshes[i] = nullptr; // True auto-nulling
        }
    }
}