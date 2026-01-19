// ---------------------------------------------------------------------------
// includes
#include <crtdbg.h> // To check for memory leaks
#include <cmath>
#include <iostream>
#include "AEEngine.h"

// Useful macro to count number of items in an array
#define array_count(a) (sizeof(a)/sizeof(*a))

float movespeed, damage = 10;
float zone1centre[2] = { -300,0 };
float zone2centre[2] = { 300,0 };
float playerposition[2] = { 0,0 };
float healthbar = 1500;
int healthbarno = 10;
float zoneradius = 150;
float playerradius = 25;
float playerspeed = 300;

static void playermove() {
    if (AEInputCheckCurr(AEVK_W) && AEInputCheckCurr(AEVK_A)) {
        movespeed /= (float)sqrt(2);
        playerposition[0] -= movespeed;
        playerposition[1] += movespeed;
    }
    else if (AEInputCheckCurr(AEVK_W) && AEInputCheckCurr(AEVK_D)) {
        movespeed /= (float)sqrt(2);
        playerposition[0] += movespeed;
        playerposition[1] += movespeed;
    }
    else if (AEInputCheckCurr(AEVK_A) && AEInputCheckCurr(AEVK_S)) {
        movespeed /= (float)sqrt(2);
        playerposition[0] -= movespeed;
        playerposition[1] -= movespeed;
    }
    else if (AEInputCheckCurr(AEVK_S) && AEInputCheckCurr(AEVK_D)) {
        movespeed /= (float)sqrt(2);
        playerposition[0] += movespeed;
        playerposition[1] -= movespeed;
    }
    else if (AEInputCheckCurr(AEVK_W)) {
        playerposition[1] += movespeed;
    }
    else if (AEInputCheckCurr(AEVK_A)) {
        playerposition[0] -= movespeed;
    }
    else if (AEInputCheckCurr(AEVK_S)) {
        playerposition[1] -= movespeed;
    }
    else if (AEInputCheckCurr(AEVK_D)) {
        playerposition[0] += movespeed;
    }
    if (playerposition[0] >= 800) playerposition[0] = 800;
    if (playerposition[0] <= -800) playerposition[0] = -800;
    if (playerposition[1] >= 450) playerposition[1] = 450;
    if (playerposition[1] <= -450) playerposition[1] = 450;
    //std::cout << playerposition[0] << ',' << playerposition[1];
}

static void sethealth() {
    float xred = zone1centre[0] - playerposition[0];
    float yred = zone1centre[1] - playerposition[1];
    float xgreen = zone2centre[0] - playerposition[0];
    float ygreen = zone2centre[1] - playerposition[1];
    float disfromred = yred * yred + xred * xred;
    float disfromgreen = ygreen * ygreen + xgreen * xgreen;
    float collisiondis = ((playerradius + zoneradius) * (playerradius + zoneradius));
    //printf("%f,%f,%f\n", collisiondis, xred, disfromred);
    if (disfromred <= collisiondis && healthbar >= 0) healthbar -= damage;
    if (disfromgreen <= collisiondis && healthbar <= 1500) healthbar += damage;
    if (healthbar >= 1500) healthbar = 1500;
    if (healthbar <= 0) healthbar = 0;
    healthbarno = round(healthbar / 1500 * 10);
}

// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    int gGameRunning = 1;

    // Initialization of your own variables go here

    // Using custom window procedure
    AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);

    // Changing the window title
    AESysSetWindowTitle("UI Demo!");

    // Pointer to Mesh. All our sprites use the same mesh, so we can just have one.
    AEGfxVertexList* pMesh[3]{};
    AEGfxVertexList* SqMesh[2]{};

    // Informing the library that we're about to start adding triangles
    AEGfxMeshStart();
    // Color parameters represent colours as ARGB
    // UV coordinates to read from loaded textures
    for (int i = 1;i <= 75;i++) {
        AEGfxTriAdd(
            0.0f, 0.0f, 0xFF0000FF, 0.0f, 0.0f,
            (playerradius * cos(i * 2 * PI / 75)), (playerradius * sin(i * 2 * PI / 75)), 0xFF0000FF, 0.0f, 0.0f,
            (playerradius * cos((i - 1) * 2 * PI / 75)), (playerradius * sin((i - 1) * 2 * PI / 75)), 0xFF0000FF, 0.0f, 0.0f);
    };
    // Saving the mesh (list of triangles) in pMesh
    pMesh[0] = AEGfxMeshEnd();
    AEGfxMeshStart();
    for (int i = 1;i <= 75;i++) {
        AEGfxTriAdd(
            zone1centre[0], zone1centre[1], 0xFFFF0000, 0.0f, 0.0f,
            (zoneradius * cos(i * 2 * PI / 75) + zone1centre[0]), (zoneradius * sin(i * 2 * PI / 75) + zone1centre[1]), 0xFFFF0000, 0.0f, 0.0f,
            (zoneradius * cos((i - 1) * 2 * PI / 75) + zone1centre[0]), (zoneradius * sin((i - 1) * 2 * PI / 75) + zone1centre[1]), 0xFFFF0000, 0.0f, 0.0f);
    };
    pMesh[1] = AEGfxMeshEnd();
    AEGfxMeshStart();
    for (int i = 1;i <= 75;i++) {
        AEGfxTriAdd(
            zone2centre[0], zone2centre[1], 0xFF00FF00, 0.0f, 0.0f,
            (zoneradius * cos(i * 2 * PI / 75) + zone2centre[0]), (zoneradius * sin(i * 2 * PI / 75) + zone2centre[1]), 0xFF00FF00, 0.0f, 0.0f,
            (zoneradius * cos((i - 1) * 2 * PI / 75) + zone2centre[0]), (zoneradius * sin((i - 1) * 2 * PI / 75) + zone2centre[1]), 0xFF00FF00, 0.0f, 0.0f);
    };
    pMesh[2] = AEGfxMeshEnd();

    AEGfxMeshStart();
    AEGfxTriAdd(
        -750.f, 370.f, 0xFF323232, 0.0f, 0.0f,
        750.f, 370.f, 0xFF323232, 0.0f, 0.0f,
        -750.f, 420.f, 0xFF323232, 0.0f, 0.0f);
    AEGfxTriAdd(
        750.f, 370.f, 0xFF323232, 0.0f, 0.0f,
        750.f, 420.f, 0xFF323232, 0.0f, 0.0f,
        -750.f, 420.f, 0xFF323232, 0.0f, 0.0f);
    SqMesh[1] = AEGfxMeshEnd();

    AEGfxMeshStart();
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
        0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
    SqMesh[0] = AEGfxMeshEnd();

    // Create a variable to store the transforms of our sprites
    AEMtx33 identitym;
    AEMtx33Identity(&identitym);
    AEMtx33 transforms[2] = { 0 };

    //healthbar matrix
    AEMtx33 scale;
    AEMtx33 tran;

    // Game Loop
    while (gGameRunning)
    {
        // Informing the system about the loop's start
        AESysFrameStart();

        // Your own rendering logic goes here
        movespeed = 10;
        AEInputCheckCurr(AEVK_W);
        AEInputCheckCurr(AEVK_A);
        AEInputCheckCurr(AEVK_S);
        AEInputCheckCurr(AEVK_D);
        playermove();
        sethealth();

        // Set the background to grey.
        AEGfxSetBackgroundColor(0.75f, 0.75f, 0.75f);

        // Tell the engine to get ready to draw something by colour.
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);

        // Set the the color to multiply to white, so that the sprite can 
        // display the full range of colors (default is black).
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

        // Set the color to add to nothing, so that we don't alter the sprite's color
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

        // Set blend mode to AE_GFX_BM_BLEND
        // This will allow transparency.
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        // Tell Alpha Engine to use the matrix in 'transform' to apply onto all
        // the vertices of the mesh that we are about to choose to draw in the next line.
        AEGfxSetTransform(identitym.m);
        // Tell Alpha Engine to draw the mesh with the above settings.
        //drawing zones & total health
        AEGfxMeshDraw(pMesh[1], AE_GFX_MDM_TRIANGLES);
        AEGfxMeshDraw(pMesh[2], AE_GFX_MDM_TRIANGLES);
        //drawing player
        AEMtx33Trans(&transforms[0], playerposition[0], playerposition[1]);
        AEGfxSetTransform(transforms[0].m);
        AEGfxMeshDraw(pMesh[0], AE_GFX_MDM_TRIANGLES);
        //drawing healthbar
        AEGfxSetTransform(identitym.m);
        AEGfxMeshDraw(SqMesh[1], AE_GFX_MDM_TRIANGLES);
        AEMtx33Scale(&scale, healthbar, 50.f);
        AEMtx33Trans(&tran, -(1500 - healthbar) / 2, 395.f);
        AEMtx33Concat(&transforms[0], &tran, &scale);
        AEGfxSetTransform(transforms[0].m);
        AEGfxMeshDraw(SqMesh[0], AE_GFX_MDM_TRIANGLES);
        AEMtx33Scale(&transforms[1], 1500.f / 19, 50.f);
        for (int i = 0;i < healthbarno;i++) {
            AEMtx33Trans(&transforms[0], -(18 * 1500.f / 38) + (2 * i * 1500.f / 19), 300);
            AEMtx33Concat(&transforms[0], &transforms[0], &transforms[1]);
            AEGfxSetTransform(transforms[0].m);
            AEGfxMeshDraw(SqMesh[0], AE_GFX_MDM_TRIANGLES);
        }



        // Informing the system about the loop's end
        AESysFrameEnd();


        // check if forcing the application to quit
        if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
            gGameRunning = 0;
    }

    for (int i = 0;i < (sizeof(pMesh) / sizeof(pMesh[0]));i++) AEGfxMeshFree(pMesh[i]);
    for (int i = 0;i < (sizeof(SqMesh) / sizeof(SqMesh[0]));i++) AEGfxMeshFree(SqMesh[i]);
    // free the system
    AESysExit();

}
