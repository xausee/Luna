#define WIN32_LEAN_AND_MEAN
#include "Luna.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
{
	DWORD dwError = 0; 	
	CDerivedWindow mainWnd(hInstance);
	
	WNDCLASSEX wcx;
	wcx.cbSize              = sizeof(WNDCLASSEX);							// size of structure 
    wcx.style               = CS_HREDRAW | CS_VREDRAW;						// redraw if size changes 
    wcx.lpfnWndProc         = CBaseWindow::stWinMsgHandler;				    // points to window procedure 
    wcx.cbClsExtra          = 0;											// no extra class memory 
    wcx.cbWndExtra          = 0;											// no extra window memory 
    wcx.hInstance           = hInstance;									// handle to instance 
    wcx.hIcon               = LoadIcon(NULL, IDI_APPLICATION);				// predefined app. icon 
    wcx.hCursor             = LoadCursor(NULL, IDC_ARROW);					// predefined arrow 
    wcx.hbrBackground       = (HBRUSH)GetStockObject(WHITE_BRUSH);	        // white background brush 
    wcx.lpszMenuName        = MAKEINTRESOURCE(IDM_CAPTURER);			    // name of menu resource 
    wcx.lpszClassName       = "CaptuerWindow";						        // name of window class 
    wcx.hIconSm             = LoadIcon(NULL, IDI_APPLICATION);				// small class icon 
 
	if (mainWnd.RegisterWindow(&wcx))
	{				
		if (mainWnd.Create())
		{				
			mainWnd.MsgLoop();			
		}
		else
			dwError = GetLastError();
	}
	dwError = GetLastError();
	return dwError;
} 