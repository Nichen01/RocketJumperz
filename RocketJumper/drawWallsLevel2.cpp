#include "drawWallsLevel2.h"

AEGfxTexture* characterPic_2 = NULL;
AEGfxTexture* base1_2 = NULL;
AEGfxTexture* base2_2 = NULL;
AEGfxTexture* base3_2 = NULL;
AEGfxTexture* base4_2 = NULL;
AEGfxTexture* base5_2 = NULL;
AEGfxTexture* base6_2 = NULL;
AEGfxTexture* base7_2 = NULL;
AEGfxTexture* base8_2 = NULL;
AEGfxTexture* base9_2 = NULL;
AEGfxTexture* base10_2 = NULL;
AEGfxTexture* base11_2 = NULL;
AEGfxTexture* base12_2 = NULL;
AEGfxTexture* enemyPic = NULL;

AEMtx33 platformTransform65, platformTransform66, platformTransform67, platformTransform68, platformTransform69;
AEMtx33 platformTransform70, platformTransform71, platformTransform72;
AEMtx33 platformTransform73, platformTransform74, platformTransform75, platformTransform76;
AEMtx33 transformEnemy, transformEnemy2, transformChar2, transformDoor2, transformDoor3, transformDoor4, transformDoor5;

AEGfxVertexList* textureMesh2 = nullptr;

void initMesh2(void) {
	AEGfxMeshStart();
	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f, 0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, 0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f, -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	textureMesh2 = AEGfxMeshEnd();
}

void loadTextures2(void) {

	characterPic_2 = AEGfxTextureLoad("Assets/CharacterRight.png");
	base1_2 = AEGfxTextureLoad("Assets/Base1.png");
	base2_2 = AEGfxTextureLoad("Assets/Base2.png");
	base3_2 = AEGfxTextureLoad("Assets/Base3.png");
	base4_2 = AEGfxTextureLoad("Assets/Base4.png");
	base5_2 = AEGfxTextureLoad("Assets/Base5.png");
	base6_2 = AEGfxTextureLoad("Assets/Base6.png");
	base7_2 = AEGfxTextureLoad("Assets/Base7.png");
	base8_2 = AEGfxTextureLoad("Assets/Base8.png");
	base9_2 = AEGfxTextureLoad("Assets/Base9.png");
	base10_2 = AEGfxTextureLoad("Assets/Base10.png");
	base11_2 = AEGfxTextureLoad("Assets/Base11.png");
	base12_2 = AEGfxTextureLoad("Assets/Base12.png");
	enemyPic = AEGfxTextureLoad("Assets/Enemy.png");
}

void unloadTextures2(void)
{
	AEGfxTextureUnload(characterPic_2);
	AEGfxTextureUnload(base1_2);
	AEGfxTextureUnload(base2_2);
	AEGfxTextureUnload(base3_2);
	AEGfxTextureUnload(base4_2);
	AEGfxTextureUnload(base5_2);
	AEGfxTextureUnload(base6_2);
	AEGfxTextureUnload(base7_2);
	AEGfxTextureUnload(base8_2);
	AEGfxTextureUnload(base9_2);
	AEGfxTextureUnload(base10_2);
	AEGfxTextureUnload(base11_2);
	AEGfxTextureUnload(base12_2);
	AEGfxTextureUnload(enemyPic);
}

void initTransform2(void)
{
	AEMtx33 scale, rotate, translate;
	AEMtx33Scale(&scale, 70.f, 70.f);
	AEMtx33Rot(&rotate, 0);

	AEMtx33Trans(&translate, -350.f, 150.f);
	AEMtx33Concat(&platformTransform65, &rotate, &scale);
	AEMtx33Concat(&platformTransform65, &translate, &platformTransform65);
	AEMtx33Trans(&translate, -280.f, 150.f);
	AEMtx33Concat(&platformTransform66, &rotate, &scale);
	AEMtx33Concat(&platformTransform66, &translate, &platformTransform66);
	AEMtx33Trans(&translate, -210.f, 150.f);
	AEMtx33Concat(&platformTransform67, &rotate, &scale);
	AEMtx33Concat(&platformTransform67, &translate, &platformTransform67);
	AEMtx33Trans(&translate, -140.f, 150.f);
	AEMtx33Concat(&platformTransform68, &rotate, &scale);
	AEMtx33Concat(&platformTransform68, &translate, &platformTransform68);
	AEMtx33Trans(&translate, -70.f, 150.f);
	AEMtx33Concat(&platformTransform69, &rotate, &scale);
	AEMtx33Concat(&platformTransform69, &translate, &platformTransform69);
	AEMtx33Trans(&translate, -280.f, 80.f);
	AEMtx33Concat(&platformTransform70, &rotate, &scale);
	AEMtx33Concat(&platformTransform70, &translate, &platformTransform70);
	AEMtx33Trans(&translate, -210.f, 80.f);
	AEMtx33Concat(&platformTransform71, &rotate, &scale);
	AEMtx33Concat(&platformTransform71, &translate, &platformTransform71);
	AEMtx33Trans(&translate, -140.f, 80.f);
	AEMtx33Concat(&platformTransform72, &rotate, &scale);
	AEMtx33Concat(&platformTransform72, &translate, &platformTransform72);

	AEMtx33Trans(&translate, 140.f, -210.f);
	AEMtx33Concat(&platformTransform73, &rotate, &scale);
	AEMtx33Concat(&platformTransform73, &translate, &platformTransform73);
	AEMtx33Trans(&translate, 210.f, -210.f);
	AEMtx33Concat(&platformTransform74, &rotate, &scale);
	AEMtx33Concat(&platformTransform74, &translate, &platformTransform74);
	AEMtx33Trans(&translate, 280.f, -210.f);
	AEMtx33Concat(&platformTransform75, &rotate, &scale);
	AEMtx33Concat(&platformTransform75, &translate, &platformTransform75);
	AEMtx33Trans(&translate, 350.f, -210.f);
	AEMtx33Concat(&platformTransform76, &rotate, &scale);
	AEMtx33Concat(&platformTransform76, &translate, &platformTransform76);

	AEMtx33 scaleEnemy, rotateEnemy, translateEnemy;
	AEMtx33Scale(&scaleEnemy, 90.f, 90.f); 
	AEMtx33Rot(&rotateEnemy, 0);
	AEMtx33Trans(&translateEnemy, 280.f, -110.f);
	AEMtx33Concat(&transformEnemy, &rotateEnemy, &scaleEnemy);
	AEMtx33Concat(&transformEnemy, &translateEnemy, &transformEnemy);

	AEMtx33 scaleEnemy2, rotateEnemy2, translateEnemy2;
	AEMtx33Scale(&scaleEnemy2, 90.f, 90.f); AEMtx33Rot(&rotateEnemy2, 0);
	AEMtx33Trans(&translateEnemy2, -400.f, -300.f);
	AEMtx33Concat(&transformEnemy2, &rotateEnemy2, &scaleEnemy2);
	AEMtx33Concat(&transformEnemy2, &translateEnemy2, &transformEnemy2);

	AEMtx33 scaleChar, rotateChar, translateChar;
	AEMtx33Scale(&scaleChar, 250.f, 250.f); AEMtx33Rot(&rotateChar, 0);
	AEMtx33Trans(&translateChar, -100.f, -340.f);
	AEMtx33Concat(&transformChar2, &rotateChar, &scaleChar);
	AEMtx33Concat(&transformChar2, &translateChar, &transformChar2);

	AEMtx33 scaleDoor, rotateDoor, translateDoor;
	AEMtx33Scale(&scaleDoor, 30.f, 200.f); AEMtx33Rot(&rotateDoor, 0);
	AEMtx33Trans(&translateDoor, -790.f, 0.f);
	AEMtx33Concat(&transformDoor2, &rotateDoor, &scaleDoor);
	AEMtx33Concat(&transformDoor2, &translateDoor, &transformDoor2);

	AEMtx33Trans(&translateDoor, 790.f, 0.f);
	AEMtx33Concat(&transformDoor4, &rotateDoor, &scaleDoor);
	AEMtx33Concat(&transformDoor4, &translateDoor, &transformDoor4);

	AEMtx33Trans(&translateDoor, 0.f, -440.f);
	AEMtx33Scale(&scaleDoor, 200.f, 30.f);
	AEMtx33Concat(&transformDoor3, &rotateDoor, &scaleDoor);
	AEMtx33Concat(&transformDoor3, &translateDoor, &transformDoor3);

	AEMtx33Trans(&translateDoor, 0.f, 440.f);
	AEMtx33Concat(&transformDoor5, &rotateDoor, &scaleDoor);
	AEMtx33Concat(&transformDoor5, &translateDoor, &transformDoor5);

}

void drawWalls2(void) 
{
	AEGfxTextureSet(base10_2, 0, 0);
	AEGfxSetTransform(platformTransform65.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2_2, 0, 0);
	AEGfxSetTransform(platformTransform66.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2_2, 0, 0);
	AEGfxSetTransform(platformTransform67.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2_2, 0, 0);
	AEGfxSetTransform(platformTransform68.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base12_2, 0, 0);
	AEGfxSetTransform(platformTransform69.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base7_2, 0, 0);
	AEGfxSetTransform(platformTransform70.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8_2, 0, 0);
	AEGfxSetTransform(platformTransform71.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base9_2, 0, 0);
	AEGfxSetTransform(platformTransform72.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base10_2, 0, 0);
	AEGfxSetTransform(platformTransform73.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base11_2, 0, 0);
	AEGfxSetTransform(platformTransform74.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base11_2, 0, 0);
	AEGfxSetTransform(platformTransform75.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base12_2, 0, 0);
	AEGfxSetTransform(platformTransform76.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(enemyPic, 0, 0);
	AEGfxSetTransform(transformEnemy.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(enemyPic, 0, 0);
	AEGfxSetTransform(transformEnemy2.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(characterPic_2, 0, 0);
	AEGfxSetTransform(transformChar2.m);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(transformDoor2.m);
	AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 0.5f);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(transformDoor3.m);
	AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 0.5f);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(transformDoor4.m);
	AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 0.5f);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(transformDoor5.m);
	AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 0.5f);
	AEGfxMeshDraw(textureMesh2, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
}