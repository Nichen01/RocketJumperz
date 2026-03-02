/* Start Header ************************************************************************/
/*!
\file         animation.cpp
\author       Nicholas Chen
\date         February, 22, 2026
\brief        Implementation of the general-purpose sprite animation system.

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "animation.h"

namespace animSystem
{
    /*!*************************************************************************
     * animSystem::init
     * @brief Initializes sprite animation details for entity
     *
     * @param SpriteAnimation&    Access struct containing anim data
     * @param int totalFrames     Total number of animation frames needed
     * @param f32 frameDelay      Amount of time spent showing ONE frame
     * @return                    void
     ***************************************************************************/
    void init(SpriteAnimation& anim,
        int          totalFrames,
        f32          frameDelay,
        AnimPlayMode mode,
        int          startFrame)
    {
        anim.totalFrames = totalFrames;
        anim.frameDelay = frameDelay;
        anim.playMode = mode;
        anim.currentFrame = startFrame;
        anim.frameTimer = 0.f;
        anim.isPlaying = (mode != ANIM_IDLE) ? 1 : 0;
        anim.pingPongDir = 1;
        anim.justFinished = 0;
    }

    /*!*************************************************************************
     * animSystem::update
     * @brief Function to update and play the animation
     *
     * @param SpriteAnimation&    Access struct containing anim data
     * @param f32 deltaTime       Time elapsed between the previous frame and current frame
     * @return                    void
     ***************************************************************************/
    void update(SpriteAnimation& anim, f32 deltaTime)
    {
        // To check if animation finished in the previous frame, yes = 1, no = 0
        anim.justFinished = 0;  
        // if animation is paused or idle, no need to calculate timers.
        if (!anim.isPlaying || anim.playMode == ANIM_IDLE) return; 

        //Increment the time elapsed
        anim.frameTimer += deltaTime;
        //while the frame isn't as much as the frameDelay, do nothing
        if (anim.frameTimer < anim.frameDelay) return;
        //
        anim.frameTimer -= anim.frameDelay;

        switch (anim.playMode)
        {
        case ANIM_PLAY_ONCE:
            if (anim.currentFrame < anim.totalFrames - 1)
                anim.currentFrame++;
            else
            {
                anim.isPlaying = 0;
                anim.playMode = ANIM_IDLE;
                anim.justFinished = 1;
            }
            break;

        case ANIM_PLAY_REVERSE:
            if (anim.currentFrame > 0)
                anim.currentFrame--;
            else
            {
                anim.isPlaying = 0;
                anim.playMode = ANIM_IDLE;
                anim.justFinished = 1;
            }
            break;

        case ANIM_LOOP:
            anim.currentFrame = (anim.currentFrame + 1) % anim.totalFrames;
            break;

        case ANIM_PING_PONG:
            anim.currentFrame += anim.pingPongDir;
            if (anim.currentFrame >= anim.totalFrames - 1)
            {
                anim.currentFrame = anim.totalFrames - 1;
                anim.pingPongDir = -1;
            }
            else if (anim.currentFrame <= 0)
            {
                anim.currentFrame = 0;
                anim.pingPongDir = 1;
            }
            break;

        default: break;
        }
    }

    void play(SpriteAnimation& anim, AnimPlayMode mode)
    {
        anim.playMode = mode;
        anim.isPlaying = (mode != ANIM_IDLE) ? 1 : 0;
        anim.frameTimer = 0.f;
        anim.pingPongDir = 1;
        anim.justFinished = 0;
    }

    void stop(SpriteAnimation& anim)
    {
        anim.isPlaying = 0;
        anim.playMode = ANIM_IDLE;
        anim.frameTimer = 0.f;
        anim.justFinished = 0;
    }

    void reset(SpriteAnimation& anim)
    {
        anim.currentFrame = 0;
        anim.frameTimer = 0.f;
        anim.isPlaying = 0;
        anim.playMode = ANIM_IDLE;
        anim.pingPongDir = 1;
        anim.justFinished = 0;
    }

    void setFrame(SpriteAnimation& anim, int frame)
    {
        if (frame >= 0 && frame < anim.totalFrames)
            anim.currentFrame = frame;
    }

    f32 getUOffset(const SpriteAnimation& anim)
    {
        return static_cast<f32>(anim.currentFrame) /
            static_cast<f32>(anim.totalFrames);
    }

    void buildMesh(AEGfxVertexList** outMesh, int totalFrames)
    {
        if (!outMesh || totalFrames <= 0) return;

        // UVs span exactly one frame: [0.0, 1/totalFrames]
        // AEGfxTextureSet's UV offset then slides this window to the right frame.
        f32 uMax = 1.f / static_cast<f32>(totalFrames);

        AEGfxMeshStart();
        AEGfxTriAdd(
            -0.5f, -0.5f, 0xFFFFFFFF, 0.f, 1.f,
            0.5f, -0.5f, 0xFFFFFFFF, uMax, 1.f,
            -0.5f, 0.5f, 0xFFFFFFFF, 0.f, 0.f);
        AEGfxTriAdd(
            0.5f, -0.5f, 0xFFFFFFFF, uMax, 1.f,
            0.5f, 0.5f, 0xFFFFFFFF, uMax, 0.f,
            -0.5f, 0.5f, 0xFFFFFFFF, 0.f, 0.f);
        *outMesh = AEGfxMeshEnd();
    }

} // namespace animSystem