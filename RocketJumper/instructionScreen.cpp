#include "instructionScreen.h"
s8 pFont = AEGfxCreateFont("Assets/PressStart2P-Regular.ttf", 72);

void instructionScreen_Load() {

}

void instructionScreen_Initialize() {

}

void instructionScreen_Update() {

}

void instructionScreen_Draw() {

        // Draw translucent overlay
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetTransparency(0.5f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f); // dark overlay

        // Draw text
        // Put this code between AESysFrameStart() and
// AESysFrameEnd(), of course! 
        AEGfxPrint(pFont,           // font handle given by AEGfxCreateFont()
            "Press SPACE to continue",   // null-terminated c-string to print
            0.f,             // x position on normalized coordinates, ranging from -1.f to 1.f 
            0.f,             // y position in normalized coordinates, ranging from -1.f to 1.f 
            1.f,             // how much to scale the text by 
            1.f,             // percentage of red, ranging from 0.f to 1.f 
            1.f,             // percentage of green, ranging from 0.f to 1.f
            1.f,             // percentage of blue, ranging from 0.f to 1.f 
            1.f);            // percentage of alpha, ranging from 0.f to 1.f
}

void instructionScreen_Free() {

}

void instructionScreen_Unload() {
    AEGfxDestroyFont(pFont);
}