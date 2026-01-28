#include "drawWallsLevel1.h" 

AEGfxTexture* characterPic = NULL;
AEGfxTexture* base1 = NULL;
AEGfxTexture* base2 = NULL;
AEGfxTexture* base3 = NULL;
AEGfxTexture* base4 = NULL;
AEGfxTexture* base5 = NULL;
AEGfxTexture* base6 = NULL;
AEGfxTexture* base7 = NULL;
AEGfxTexture* base8 = NULL;
AEGfxTexture* base9 = NULL;
AEGfxTexture* base10 = NULL;
AEGfxTexture* base11 = NULL;
AEGfxTexture* base12 = NULL;

AEMtx33 platformTransform1, platformTransform2, platformTransform3, platformTransform4; 
AEMtx33 platformTransform5, platformTransform6, platformTransform7, platformTransform8;
AEMtx33 platformTransform9, platformTransform10, platformTransform11, platformTransform12;
AEMtx33 platformTransform13, platformTransform14, platformTransform15;
AEMtx33 platformTransform16, platformTransform17, platformTransform18;
AEMtx33 platformTransform19, platformTransform20, platformTransform21, platformTransform22, platformTransform23, platformTransform24, platformTransform25, platformTransform26, platformTransform27, platformTransform28, platformTransform29, platformTransform30, platformTransform31;
AEMtx33 platformTransform32, platformTransform33, platformTransform34, platformTransform35, platformTransform36, platformTransform37, platformTransform38, platformTransform39, platformTransform40, platformTransform41, platformTransform42, platformTransform43, platformTransform44;
AEMtx33 transformChar, transformDoor;
AEMtx33 platformTransform45, platformTransform46, platformTransform47, platformTransform48, platformTransform49, platformTransform50, platformTransform51, platformTransform52, platformTransform53, platformTransform54, platformTransform55;
AEMtx33 platformTransform56, platformTransform57, platformTransform58, platformTransform59, platformTransform60, platformTransform61, platformTransform62, platformTransform63, platformTransform64;

AEGfxVertexList* textureMesh = nullptr;
AEGfxVertexList* exitDoorMesh = nullptr;

void initMesh1(void) {
	AEGfxMeshStart(); 
	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f, 0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, 0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f, -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f); 
	textureMesh = AEGfxMeshEnd(); 

	AEGfxMeshStart();
	AEGfxTriAdd(-0.5f, -0.5f, 0xFFD3D3D3, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFD3D3D3, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFD3D3D3, 0.0f, 0.0f);
	AEGfxTriAdd(0.5f, -0.5f, 0xFFD3D3D3, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFD3D3D3, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFD3D3D3, 0.0f, 0.0f);
	exitDoorMesh = AEGfxMeshEnd();
}

void loadTextures1(void) {

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
	base10 = AEGfxTextureLoad("Assets/Base10.png");
	base11 = AEGfxTextureLoad("Assets/Base11.png");
	base12 = AEGfxTextureLoad("Assets/Base12.png");
}

void unloadTextures1(void) 
{ 
	AEGfxTextureUnload(characterPic); 
	AEGfxTextureUnload(base1);
	AEGfxTextureUnload(base2);
	AEGfxTextureUnload(base3);
	AEGfxTextureUnload(base4); 
	AEGfxTextureUnload(base5);
	AEGfxTextureUnload(base6);
	AEGfxTextureUnload(base7);
	AEGfxTextureUnload(base8);
	AEGfxTextureUnload(base9);
	AEGfxTextureUnload(base10);
	AEGfxTextureUnload(base11);
	AEGfxTextureUnload(base12);
}

void initTransform1(void) 
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
	AEMtx33Concat(&platformTransform3, &rotate, &scale);
	AEMtx33Concat(&platformTransform3, &translate, &platformTransform3);
	AEMtx33Trans(&translate, -555.f, -280.f); 
	AEMtx33Concat(&platformTransform4, &rotate, &scale);
	AEMtx33Concat(&platformTransform4, &translate, &platformTransform4);

	AEMtx33Trans(&translate, -765.f, -350.f);
	AEMtx33Concat(&platformTransform5, &rotate, &scale); 
	AEMtx33Concat(&platformTransform5, &translate, &platformTransform5);
	AEMtx33Trans(&translate, -695.f, -350.f); 
	AEMtx33Concat(&platformTransform6, &rotate, &scale);
	AEMtx33Concat(&platformTransform6, &translate, &platformTransform6);
	AEMtx33Trans(&translate, -625.f, -350.f);
	AEMtx33Concat(&platformTransform7, &rotate, &scale);
	AEMtx33Concat(&platformTransform7, &translate, &platformTransform7);
	AEMtx33Trans(&translate, -555.f, -350.f);
	AEMtx33Concat(&platformTransform8, &rotate, &scale);
	AEMtx33Concat(&platformTransform8, &translate, &platformTransform8);

	AEMtx33Trans(&translate, -765.f, -420.f);
	AEMtx33Concat(&platformTransform9, &rotate, &scale);
	AEMtx33Concat(&platformTransform9, &translate, &platformTransform9);
	AEMtx33Trans(&translate, -695.f, -420.f);
	AEMtx33Concat(&platformTransform10, &rotate, &scale);
	AEMtx33Concat(&platformTransform10, &translate, &platformTransform10);
	AEMtx33Trans(&translate, -625.f, -420.f);
	AEMtx33Concat(&platformTransform11, &rotate, &scale);
	AEMtx33Concat(&platformTransform11, &translate, &platformTransform11);
	AEMtx33Trans(&translate, -555.f, -420.f);
	AEMtx33Concat(&platformTransform12, &rotate, &scale);
	AEMtx33Concat(&platformTransform12, &translate, &platformTransform12);

	AEMtx33Trans(&translate, -485.f, -350.f);
	AEMtx33Concat(&platformTransform13, &rotate, &scale);
	AEMtx33Concat(&platformTransform13, &translate, &platformTransform13);
	AEMtx33Trans(&translate, -415.f, -350.f);
	AEMtx33Concat(&platformTransform14, &rotate, &scale);
	AEMtx33Concat(&platformTransform14, &translate, &platformTransform14);
	AEMtx33Trans(&translate, -345.f, -350.f);
	AEMtx33Concat(&platformTransform15, &rotate, &scale);
	AEMtx33Concat(&platformTransform15, &translate, &platformTransform15);

	AEMtx33Trans(&translate, -485.f, -420.f);
	AEMtx33Concat(&platformTransform16, &rotate, &scale);
	AEMtx33Concat(&platformTransform16, &translate, &platformTransform16);
	AEMtx33Trans(&translate, -415.f, -420.f);
	AEMtx33Concat(&platformTransform17, &rotate, &scale);
	AEMtx33Concat(&platformTransform17, &translate, &platformTransform17);
	AEMtx33Trans(&translate, -345.f, -420.f);
	AEMtx33Concat(&platformTransform18, &rotate, &scale);
	AEMtx33Concat(&platformTransform18, &translate, &platformTransform18);

	AEMtx33Trans(&translate, 765.f, -70.f);
	AEMtx33Concat(&platformTransform19, &rotate, &scale);
	AEMtx33Concat(&platformTransform19, &translate, &platformTransform19);
	AEMtx33Trans(&translate, 695.f, -70.f);
	AEMtx33Concat(&platformTransform20, &rotate, &scale);
	AEMtx33Concat(&platformTransform20, &translate, &platformTransform20);
	AEMtx33Trans(&translate, 625.f, -70.f);
	AEMtx33Concat(&platformTransform21, &rotate, &scale);
	AEMtx33Concat(&platformTransform21, &translate, &platformTransform21);
	AEMtx33Trans(&translate, 555.f, -70.f);
	AEMtx33Concat(&platformTransform22, &rotate, &scale);
	AEMtx33Concat(&platformTransform22, &translate, &platformTransform22);
	AEMtx33Trans(&translate, 485.f, -70.f);
	AEMtx33Concat(&platformTransform23, &rotate, &scale);
	AEMtx33Concat(&platformTransform23, &translate, &platformTransform23);
	AEMtx33Trans(&translate, 415.f, -70.f);
	AEMtx33Concat(&platformTransform24, &rotate, &scale);
	AEMtx33Concat(&platformTransform24, &translate, &platformTransform24);
	AEMtx33Trans(&translate, 345.f, -70.f);
	AEMtx33Concat(&platformTransform25, &rotate, &scale);
	AEMtx33Concat(&platformTransform25, &translate, &platformTransform25);
	AEMtx33Trans(&translate, 275.f, -70.f);
	AEMtx33Concat(&platformTransform26, &rotate, &scale);
	AEMtx33Concat(&platformTransform26, &translate, &platformTransform26);
	AEMtx33Trans(&translate, 205.f, -70.f);
	AEMtx33Concat(&platformTransform27, &rotate, &scale);
	AEMtx33Concat(&platformTransform27, &translate, &platformTransform27);
	AEMtx33Trans(&translate, 135.f, -70.f);
	AEMtx33Concat(&platformTransform28, &rotate, &scale);
	AEMtx33Concat(&platformTransform28, &translate, &platformTransform28);
	AEMtx33Trans(&translate, 65.f, -70.f);
	AEMtx33Concat(&platformTransform29, &rotate, &scale);
	AEMtx33Concat(&platformTransform29, &translate, &platformTransform29);
	AEMtx33Trans(&translate, -5.f, -70.f);
	AEMtx33Concat(&platformTransform30, &rotate, &scale);
	AEMtx33Concat(&platformTransform30, &translate, &platformTransform30);
	AEMtx33Trans(&translate, -75.f, -70.f);
	AEMtx33Concat(&platformTransform31, &rotate, &scale);
	AEMtx33Concat(&platformTransform31, &translate, &platformTransform31);

	AEMtx33Trans(&translate, 765.f, -140.f);
	AEMtx33Concat(&platformTransform32, &rotate, &scale);
	AEMtx33Concat(&platformTransform32, &translate, &platformTransform32);
	AEMtx33Trans(&translate, 695.f, -140.f);
	AEMtx33Concat(&platformTransform33, &rotate, &scale);
	AEMtx33Concat(&platformTransform33, &translate, &platformTransform33);
	AEMtx33Trans(&translate, 625.f, -140.f);
	AEMtx33Concat(&platformTransform34, &rotate, &scale);
	AEMtx33Concat(&platformTransform34, &translate, &platformTransform34);
	AEMtx33Trans(&translate, 555.f, -140.f);
	AEMtx33Concat(&platformTransform35, &rotate, &scale);
	AEMtx33Concat(&platformTransform35, &translate, &platformTransform35);
	AEMtx33Trans(&translate, 485.f, -140.f);
	AEMtx33Concat(&platformTransform36, &rotate, &scale);
	AEMtx33Concat(&platformTransform36, &translate, &platformTransform36);
	AEMtx33Trans(&translate, 415.f, -140.f);
	AEMtx33Concat(&platformTransform37, &rotate, &scale);
	AEMtx33Concat(&platformTransform37, &translate, &platformTransform37);
	AEMtx33Trans(&translate, 345.f, -140.f);
	AEMtx33Concat(&platformTransform38, &rotate, &scale);
	AEMtx33Concat(&platformTransform38, &translate, &platformTransform38);
	AEMtx33Trans(&translate, 275.f, -140.f);
	AEMtx33Concat(&platformTransform39, &rotate, &scale);
	AEMtx33Concat(&platformTransform39, &translate, &platformTransform39);
	AEMtx33Trans(&translate, 205.f, -140.f);
	AEMtx33Concat(&platformTransform40, &rotate, &scale);
	AEMtx33Concat(&platformTransform40, &translate, &platformTransform40);
	AEMtx33Trans(&translate, 135.f, -140.f);
	AEMtx33Concat(&platformTransform41, &rotate, &scale);
	AEMtx33Concat(&platformTransform41, &translate, &platformTransform41);
	AEMtx33Trans(&translate, 65.f, -140.f);
	AEMtx33Concat(&platformTransform42, &rotate, &scale);
	AEMtx33Concat(&platformTransform42, &translate, &platformTransform42);
	AEMtx33Trans(&translate, -5.f, -140.f);
	AEMtx33Concat(&platformTransform43, &rotate, &scale);
	AEMtx33Concat(&platformTransform43, &translate, &platformTransform43);
	AEMtx33Trans(&translate, -75.f, -140.f);
	AEMtx33Concat(&platformTransform44, &rotate, &scale);
	AEMtx33Concat(&platformTransform44, &translate, &platformTransform44);

	AEMtx33 scaleChar, rotateChar, translateChar; 
	AEMtx33Scale(&scaleChar, 250.f, 250.f); AEMtx33Rot(&rotateChar, 0);
	AEMtx33Trans(&translateChar, 200.f, -380.f);
	AEMtx33Concat(&transformChar, &rotateChar, &scaleChar);
	AEMtx33Concat(&transformChar, &translateChar, &transformChar);

	AEMtx33 scaleDoor, rotateDoor, translateDoor;
	AEMtx33Scale(&scaleDoor, 30.f, 200.f); AEMtx33Rot(&rotateDoor, 0);
	AEMtx33Trans(&translateDoor, -790.f, 300.f);
	AEMtx33Concat(&transformDoor, &rotateDoor, &scaleDoor);
	AEMtx33Concat(&transformDoor, &translateDoor, &transformDoor);

	AEMtx33Trans(&translate, -765.f, 210.f);
	AEMtx33Concat(&platformTransform45, &rotate, &scale);
	AEMtx33Concat(&platformTransform45, &translate, &platformTransform45);
	AEMtx33Trans(&translate, -695.f, 210.f);
	AEMtx33Concat(&platformTransform46, &rotate, &scale);
	AEMtx33Concat(&platformTransform46, &translate, &platformTransform46);
	AEMtx33Trans(&translate, -625.f, 210.f);
	AEMtx33Concat(&platformTransform47, &rotate, &scale);
	AEMtx33Concat(&platformTransform47, &translate, &platformTransform47);
	AEMtx33Trans(&translate, -555.f, 210.f);
	AEMtx33Concat(&platformTransform48, &rotate, &scale);
	AEMtx33Concat(&platformTransform48, &translate, &platformTransform48);
	AEMtx33Trans(&translate, -485.f, 210.f);
	AEMtx33Concat(&platformTransform49, &rotate, &scale);
	AEMtx33Concat(&platformTransform49, &translate, &platformTransform49);
	AEMtx33Trans(&translate, -415.f, 210.f);
	AEMtx33Concat(&platformTransform50, &rotate, &scale);
	AEMtx33Concat(&platformTransform50, &translate, &platformTransform50);
	AEMtx33Trans(&translate, -345.f, 210.f);
	AEMtx33Concat(&platformTransform51, &rotate, &scale);
	AEMtx33Concat(&platformTransform51, &translate, &platformTransform51);
	AEMtx33Trans(&translate, -275.f, 210.f);
	AEMtx33Concat(&platformTransform52, &rotate, &scale);
	AEMtx33Concat(&platformTransform52, &translate, &platformTransform52);
	AEMtx33Trans(&translate, -205.f, 210.f);
	AEMtx33Concat(&platformTransform53, &rotate, &scale);
	AEMtx33Concat(&platformTransform53, &translate, &platformTransform53);
	AEMtx33Trans(&translate, -135.f, 210.f);
	AEMtx33Concat(&platformTransform54, &rotate, &scale);
	AEMtx33Concat(&platformTransform54, &translate, &platformTransform54);
	AEMtx33Trans(&translate, -65.f, 210.f);
	AEMtx33Concat(&platformTransform55, &rotate, &scale);
	AEMtx33Concat(&platformTransform55, &translate, &platformTransform55);

	AEMtx33Trans(&translate, -765.f, 140.f);
	AEMtx33Concat(&platformTransform56, &rotate, &scale);
	AEMtx33Concat(&platformTransform56, &translate, &platformTransform56);
	AEMtx33Trans(&translate, -695.f, 140.f);
	AEMtx33Concat(&platformTransform57, &rotate, &scale);
	AEMtx33Concat(&platformTransform57, &translate, &platformTransform57);
	AEMtx33Trans(&translate, -625.f, 140.f);
	AEMtx33Concat(&platformTransform58, &rotate, &scale);
	AEMtx33Concat(&platformTransform58, &translate, &platformTransform58);
	AEMtx33Trans(&translate, -555.f, 140.f);
	AEMtx33Concat(&platformTransform59, &rotate, &scale);
	AEMtx33Concat(&platformTransform59, &translate, &platformTransform59);
	AEMtx33Trans(&translate, -485.f, 140.f);
	AEMtx33Concat(&platformTransform60, &rotate, &scale);
	AEMtx33Concat(&platformTransform60, &translate, &platformTransform60);
	AEMtx33Trans(&translate, -415.f, 140.f);
	AEMtx33Concat(&platformTransform61, &rotate, &scale);
	AEMtx33Concat(&platformTransform61, &translate, &platformTransform61);
	AEMtx33Trans(&translate, -345.f, 140.f);
	AEMtx33Concat(&platformTransform62, &rotate, &scale);
	AEMtx33Concat(&platformTransform62, &translate, &platformTransform62);
	AEMtx33Trans(&translate, -275.f, 140.f);
	AEMtx33Concat(&platformTransform63, &rotate, &scale);
	AEMtx33Concat(&platformTransform63, &translate, &platformTransform63);
	AEMtx33Trans(&translate, -205.f, 140.f);
	AEMtx33Concat(&platformTransform64, &rotate, &scale);
	AEMtx33Concat(&platformTransform64, &translate, &platformTransform64);
}

void drawWalls1(void) {
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform1.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform2.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform3.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base3, 0, 0);
	AEGfxSetTransform(platformTransform4.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform5.m);
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
	AEGfxSetTransform(platformTransform9.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform10.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform11.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform12.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform13.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform14.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base3, 0, 0);
	AEGfxSetTransform(platformTransform15.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform16.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base5, 0, 0);
	AEGfxSetTransform(platformTransform17.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base6, 0, 0);
	AEGfxSetTransform(platformTransform18.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(characterPic, 0, 0);
	AEGfxSetTransform(transformChar.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(transformDoor.m);
	AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 0.5f);
	AEGfxMeshDraw(exitDoorMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform19.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform20.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform21.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform22.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform23.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform24.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform25.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform26.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform27.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform28.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform29.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform30.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base1, 0, 0);
	AEGfxSetTransform(platformTransform31.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform32.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform33.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform34.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform35.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform36.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform37.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform38.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform39.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform40.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform41.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform42.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform43.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base7, 0, 0);
	AEGfxSetTransform(platformTransform44.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform45.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform46.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform47.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform48.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform49.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform50.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform51.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform52.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base2, 0, 0);
	AEGfxSetTransform(platformTransform53.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base11, 0, 0);
	AEGfxSetTransform(platformTransform54.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base12, 0, 0);
	AEGfxSetTransform(platformTransform55.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform56.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform57.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform58.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform59.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform60.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform61.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform62.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base8, 0, 0);
	AEGfxSetTransform(platformTransform63.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(base9, 0, 0);
	AEGfxSetTransform(platformTransform64.m);
	AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
}