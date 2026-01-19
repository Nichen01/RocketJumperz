// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "collision.h"
#include "Input.h"
#include "GameStateManager.h"
#include "GameStateList.h"



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



	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	GSM_Initialize(GS_TEST);
	while (current != GS_QUIT) {
        if (current != GS_RESTART) {
            GSM_Update();
            fpLoad();
        }
        else {  
            current = previous;
            next = previous;
        }
        fpInitialize();
        while(next==current)
        {
			AESysFrameStart();
            Input_Handle();
            fpUpdate();
            fpDraw();
			AESysFrameEnd();
        }

        fpFree();

        if (next != GS_RESTART) {
            fpUnload();
        }
        previous = current;
        current = next;
    }
	AESysExit();
}