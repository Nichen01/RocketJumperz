#include "drawWallsLevel1.h" 

AEGfxTexture* characterPic = nullptr;
AEGfxTexture* base1 = nullptr;
AEGfxTexture* base2 = nullptr;
AEGfxTexture* base3 = nullptr;
AEGfxTexture* base4 = nullptr;
AEGfxTexture* base5 = nullptr;
AEGfxTexture* base6 = nullptr;
AEGfxTexture* base7 = nullptr;
AEGfxTexture* base8 = nullptr;
AEGfxTexture* base9 = nullptr;

AEMtx33 platformTransform1, platformTransform2, platformTransform2_1; 
AEMtx33 platformTransform3, platformTransform4, platformTransform5, platformTransform5_1, platformTransform6; 
AEMtx33 platformTransform7, platformTransform8, platformTransform8_1, platformTransform9;
AEMtx33 platformTransform10, platformTransform11, platformTransform11_1, platformTransform12;
AEMtx33 transformChar;

AEGfxVertexList* textureMesh = nullptr;

void initMesh(void) {
	AEGfxMeshStart(); 
	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
				0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
				-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
				0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
				-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f); 
	textureMesh = AEGfxMeshEnd(); 
}

void loadTextures(void) {

	characterPic = AEGfxTextureLoad("Assets/CharacterRight.png");
	base1 = AEGfxTextureLoad("Assets/Base1.png");
	base2 = AEGfxTextureLoad("Assets/Base2.png");
	base3 = AEGfxTextureLoad("Assets/Base3.png");
	base4 = AEGfxTextureLoad("Assets/Base4.png");
	base5 = AEGfxTextureLoad("Assets/Base5.png");
	base6 = AEGfxTextureLoad("Assets/Base6.png");
	base7 = AEGfxTextureLoad("Assets/Base7.png");
	base8 = AEGfxTextureLoad("Assets/Base8.png");
	base9 = AEGfxTextureLoad("Assets/Base9.png");
}

void unloadTextures(void) 
{ 
	if (characterPic) AEGfxTextureUnload(characterPic); 
	if (base1) AEGfxTextureUnload(base1);
	if (base2) AEGfxTextureUnload(base2);
	if (base3) AEGfxTextureUnload(base3);
	if (base4) AEGfxTextureUnload(base4); 
	if (base5) AEGfxTextureUnload(base5);
	if (base6) AEGfxTextureUnload(base6);
	if (base7) AEGfxTextureUnload(base7);
	if (base8) AEGfxTextureUnload(base8);
	if (base9) AEGfxTextureUnload(base9);
}

void initTransforms(void) 
{
	AEMtx33 scale, rotate, translate;
	AEMtx33Scale(&scale, 70.f, 70.f); 
	AEMtx33Rot(&rotate, 0);

	AEMtx33Trans(&translate, -765.f, -280.f);
	AEMtx33Concat(&platformTransform1, &rotate, &scale);
	AEMtx33Concat(&platformTransform1, &translate, &platformTransform1);

	AEMtx33Trans(&translate, -695.f, -280.f);
	AEMtx33Concat(&platformTransform2, &rotate, &scale);
	AEMtx33Concat(&platformTransform2, &translate, &platformTransform2);

	AEMtx33Trans(&translate, -625.f, -280.f);
	AEMtx33Concat(&platformTransform2_1, &rotate, &scale);
	AEMtx33Concat(&platformTransform2_1, &translate, &platformTransform2_1);

	AEMtx33Trans(&translate, -555.f, -280.f); 
	AEMtx33Concat(&platformTransform3, &rotate, &scale);
	AEMtx33Concat(&platformTransform3, &translate, &platformTransform3);

	AEMtx33Trans(&translate, -765.f, -350.f);
	AEMtx33Concat(&platformTransform4, &rotate, &scale); 
	AEMtx33Concat(&platformTransform4, &translate, &platformTransform4);

	AEMtx33Trans(&translate, -695.f, -350.f); 
	AEMtx33Concat(&platformTransform5, &rotate, &scale);
	AEMtx33Concat(&platformTransform5, &translate, &platformTransform5);

	AEMtx33Trans(&translate, -625.f, -350.f);
	AEMtx33Concat(&platformTransform5_1, &rotate, &scale);
	AEMtx33Concat(&platformTransform5_1, &translate, &platformTransform5_1);

	AEMtx33Trans(&translate, -555.f, -350.f);
	AEMtx33Concat(&platformTransform6, &rotate, &scale);
	AEMtx33Concat(&platformTransform6, &translate, &platformTransform6);

	AEMtx33Trans(&translate, -765.f, -420.f);
	AEMtx33Concat(&platformTransform7, &rotate, &scale);
	AEMtx33Concat(&platformTransform7, &translate, &platformTransform7);

	AEMtx33Trans(&translate, -695.f, -420.f);
	AEMtx33Concat(&platformTransform8, &rotate, &scale);
	AEMtx33Concat(&platformTransform8, &translate, &platformTransform8);

	AEMtx33Trans(&translate, -625.f, -420.f);
	AEMtx33Concat(&platformTransform8_1, &rotate, &scale);
	AEMtx33Concat(&platformTransform8_1, &translate, &platformTransform8_1);

	AEMtx33Trans(&translate, -555.f, -420.f);
	AEMtx33Concat(&platformTransform9, &rotate, &scale);
	AEMtx33Concat(&platformTransform9, &translate, &platformTransform9);

	AEMtx33 scaleChar, rotateChar, translateChar; 
	AEMtx33Scale(&scaleChar, 400.f, 400.f); AEMtx33Rot(&rotateChar, 0);
	AEMtx33Trans(&translateChar, -600.f, -200.f);
	AEMtx33Concat(&transformChar, &rotateChar, &scaleChar);
	AEMtx33Concat(&transformChar, &translateChar, &transformChar); 
}

void drawFloors(void) {
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform1.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform2.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform2_1.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base3, 0, 0);
	AEGfxSetTransform(platformTransform3.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform4.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform5.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform5_1.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform6.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform7.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform8.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform8_1.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform9.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
}

void drawCharacter(void) {
	AEGfxTextureSet(characterPic, 0, 0);
	AEGfxSetTransform(transformChar.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
}