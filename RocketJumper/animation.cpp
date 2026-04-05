/* Start Header ************************************************************************/
/*!
\file         animation.cpp
\author       Nicholas Chen (c.chen)
\brief        Implementation of the modular sprite animation system.

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Animation.h"

namespace animSystem
{
    /*!*************************************************************************
     * INIT
     * @brief Initializes sprite animation details for an entity
     *
     * @param anim                Access struct containing anim data
     * @param cols                Number of columns in the sprite sheet
     * @param rows                Number of rows in the sprite sheet
     * @param totalFrames         Total number of animation frames needed
     * @param frameDelay          Amount of time spent showing ONE frame
     * @param mode                Animation playback mode (e.g., Loop, Once)
     * @param startFrame          Frame to begin the animation on
     * @return VOID
     ***************************************************************************/
    void init(SpriteAnimation& anim,
        int          cols,
        int          rows,
        int          totalFrames,
        f32          frameDelay,
        AnimPlayMode mode,
        int          startFrame)
    {
        // set basic grid configuration for texture sheet
        anim.rows = rows;
        anim.cols = cols;
        anim.totalFrames = totalFrames;
        // set timings and playmode
        anim.frameDelay = frameDelay;
        anim.playMode = mode;
        anim.currentFrame = startFrame;
        anim.frameTimer = 0.f;
        // set to play unless initialized as idle
        anim.isPlaying = (mode != ANIM_IDLE) ? 1 : 0;
        anim.pingPongDir = 1;
        anim.justFinished = 0;
    }

    /*!*************************************************************************
     * UPDATE
     * @brief Function to update and play the animation
     *
     * @param anim                Access struct containing anim data
     * @param deltaTime           Time elapsed between the previous frame and current frame
     * @return VOID
     ***************************************************************************/
    void update(SpriteAnimation& anim, f32 deltaTime)
    {
        // set completion flag to 0 every frame
        anim.justFinished = 0;

        // if animation is paused or idle, no need to calculate timers.
        if (!anim.isPlaying || anim.playMode == ANIM_IDLE) return;

        // Guard: cannot advance frames if totalFrames is zero or negative
        if (anim.totalFrames <= 0) return;

        //Increment the time elapsed
        anim.frameTimer += deltaTime;

        //while the frame isn't as much as the frameDelay, do nothing
        if (anim.frameTimer < anim.frameDelay) return;
        // subtract the delay to carry over remaining time (prevents desync in diff framerates)
        anim.frameTimer -= anim.frameDelay;

        switch (anim.playMode)
        {
        case ANIM_PLAY_ONCE:
            if (anim.currentFrame < anim.totalFrames - 1)
                anim.currentFrame++;
            else
            {
                // reached end, stop animation, flag as finished
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
                // reached beginning, stop animation, flag as finished
                anim.isPlaying = 0;
                anim.playMode = ANIM_IDLE;
                anim.justFinished = 1;
            }
            break;

        case ANIM_LOOP:
            // keep going back to 0 when reaching totalframes
            anim.currentFrame = (anim.currentFrame + 1) % anim.totalFrames;
            break;

        case ANIM_OSC:
            // go forward or back depending on pingpongdir
            anim.currentFrame += anim.pingPongDir;
            // reached the end, set flag to forward
            if (anim.currentFrame >= anim.totalFrames - 1)
            {
                anim.currentFrame = anim.totalFrames - 1;
                anim.pingPongDir = -1;
            }
            // reached beginning, set flag to foward
            else if (anim.currentFrame <= 0)
            {
                anim.currentFrame = 0;
                anim.pingPongDir = 1;
            }
            break;

        default: break;
        }
    }

    /*!*************************************************************************
     * PLAY
     * @brief Initiates playback of an animation with a specific mode
     *
     * @param anim                Access struct containing anim data
     * @param mode                Animation playback mode to set
     * @return VOID
     ***************************************************************************/
    void play(SpriteAnimation& anim, AnimPlayMode mode)
    {
        anim.playMode = mode;
        anim.isPlaying = (mode != ANIM_IDLE) ? 1 : 0;
        anim.frameTimer = 0.f;
        anim.pingPongDir = 1;
        anim.justFinished = 0;
    }

    /*!*************************************************************************
     * STOP
     * @brief Pauses the animation on its current frame
     *
     * @param anim                Access struct containing anim data
     * @return VOID
     ***************************************************************************/
    void stop(SpriteAnimation& anim)
    {
        anim.isPlaying = 0;
        anim.playMode = ANIM_IDLE;
        anim.frameTimer = 0.f;
        anim.justFinished = 0;
    }

    /*!*************************************************************************
     * RESET
     * @brief Stops the animation and jumps back to the first frame
     *
     * @param anim                Access struct containing anim data
     * @return VOID
     ***************************************************************************/
    void reset(SpriteAnimation& anim)
    {
        anim.currentFrame = 0;
        anim.frameTimer = 0.f;
        anim.isPlaying = 0;
        anim.playMode = ANIM_IDLE;
        anim.pingPongDir = 1;
        anim.justFinished = 0;
    }

    /*!*************************************************************************
     * SET FRAME
     * @brief Manually overrides the current frame index
     *
     * @param anim                Access struct containing anim data
     * @param frame               The target frame index to jump to
     * @return VOID
     ***************************************************************************/
    void setFrame(SpriteAnimation& anim, int frame)
    {
        // guard: ensure frame is not out of bounds
        if (frame >= 0 && frame < anim.totalFrames)
            anim.currentFrame = frame;
    }


    /*!*************************************************************************
     * GET U OFFSET
     * @brief Calculates the horizontal (U) UV coordinate for texture mapping
     *
     * @param anim                Constant reference to the animation struct
     * @return f32                The resulting U offset float
     ***************************************************************************/
    f32 getUOffset(const SpriteAnimation& anim)
    {
        // Guard: avoid integer division by zero if cols was never set
        if (anim.cols <= 0) return 0.f;

        // get the number of frames in the row by checking the columns
        int col = anim.currentFrame % anim.cols;

        // return percentage of the width representing the U offset
        return static_cast<f32>(col) /
            static_cast<f32>(anim.cols);
    }

    /*!*************************************************************************
     * GET V OFFSET
     * @brief Calculates the vertical (V) UV coordinate for texture mapping
     *
     * @param anim                Constant reference to the animation struct
     * @return f32                The resulting v offset float
     ***************************************************************************/
    f32 getVOffset(const SpriteAnimation& anim)
    {
        // Guard: avoid integer division by zero if cols or rows was never set
        if (anim.cols <= 0 || anim.rows <= 0) return 0.f;

        int row = anim.currentFrame / anim.cols;
        return static_cast<f32>(row) /
            static_cast<f32>(anim.rows);
    }



} // namespace animSystem