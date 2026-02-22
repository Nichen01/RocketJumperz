#pragma once
/* Start Header ************************************************************************/
/*!
\file         animation.h
\author       Nicholas Chen
\date         February, 22, 2026
\brief        General-purpose sprite sheet animation component.
              Handles HORIZONTAL sprite strips only (one row of frames).

              Usage:
                1. Call animSystem::buildMesh() once at load time to create a
                   mesh whose UVs span exactly one frame of your strip.
                2. Embed a SpriteAnimation in any struct that needs animation.
                3. Call animSystem::update() once per frame.
                4. Pass animSystem::getUOffset() to AEGfxTextureSet() at draw time.

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "AEEngine.h"

// ---------------------------------------------------------------------------
// Playback modes
// ---------------------------------------------------------------------------
enum AnimPlayMode
{
    ANIM_PLAY_ONCE,     // Play forward to last frame, then stop
    ANIM_PLAY_REVERSE,  // Play backward to frame 0, then stop
    ANIM_LOOP,          // Play forward, loop back to frame 0
    ANIM_PING_PONG,     // Play forward then backward, repeat
    ANIM_IDLE           // Do not advance frames
};

// ---------------------------------------------------------------------------
// SpriteAnimation  —  embed one of these in any struct that needs animation
// ---------------------------------------------------------------------------
struct SpriteAnimation
{
    int          currentFrame;  // 0 .. (totalFrames - 1)
    int          totalFrames;   // Number of frames in the horizontal strip
    f32          frameTimer;    // Accumulated dt since last frame advance
    f32          frameDelay;    // Seconds per frame (e.g. 0.08f ≈ 12 fps)
    AnimPlayMode playMode;
    s8           isPlaying;     // 0 = frozen, 1 = actively advancing
    s8           pingPongDir;   // Internal: +1 forward, -1 backward

    // Raised to 1 for exactly ONE update() call the moment PLAY_ONCE or
    // PLAY_REVERSE finishes. Cleared automatically at the top of the next
    // update() call. Check this to react to an animation completing.
    s8           justFinished;
};

// ---------------------------------------------------------------------------
namespace animSystem
{
    void init(SpriteAnimation& anim,
        int          totalFrames,
        f32          frameDelay,
        AnimPlayMode mode = ANIM_IDLE,
        int          startFrame = 0);

    // Call once per frame — clears justFinished first, then advances frames
    void update(SpriteAnimation& anim, f32 deltaTime);

    void play(SpriteAnimation& anim, AnimPlayMode mode);
    void stop(SpriteAnimation& anim);   // Freeze on current frame
    void reset(SpriteAnimation& anim);   // Jump to frame 0 and stop
    void setFrame(SpriteAnimation& anim, int frame);

    // Pass the return value directly to AEGfxTextureSet(tex, getUOffset(anim), 0.f)
    f32  getUOffset(const SpriteAnimation& anim);

    // Build a mesh whose UVs span exactly one frame of a horizontal strip.
    // Call once at load time; free with AEGfxMeshFree().
    void buildMesh(AEGfxVertexList** outMesh, int totalFrames);

} // namespace animSystem