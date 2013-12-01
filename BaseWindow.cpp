#include "BaseWindow.h"

BOOL CBaseWindow::RegisterWindow()
{
    WNDCLASSEX wcx; 
 
    // Fill in the window class structure with default parameters 
 
    wcx.cbSize = sizeof(WNDCLASSEX);							// size of structure 
    wcx.style = CS_HREDRAW | CS_VREDRAW;						// redraw if size changes 
    wcx.lpfnWndProc = CBaseWindow::stWinMsgHandler;				// points to window procedure 
    wcx.cbClsExtra = 0;											// no extra class memory 
    wcx.cbWndExtra = 0;											// no extra window memory 
    wcx.hInstance = hInstance;									// handle to instance 
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// predefined app. icon 
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);					// predefined arrow 
    wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// white background brush 
    wcx.lpszMenuName = NULL;									// name of menu resource 
    wcx.lpszClassName = "BaseWindow";							// name of window class 
    wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);				// small class icon 
 
    // Register the window class. 
    return RegisterWindow(&wcx); 

}

BOOL CBaseWindow::RegisterWindow(UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, 
									LPCTSTR lpszMenuName, LPCTSTR lpszClassName, HICON hIconSm)
{
    WNDCLASSEX wcx; 
 
    // Fill in the window class structure with default parameters 
 
    wcx.cbSize = sizeof(WNDCLASSEX);				// size of structure 
    wcx.style = style;								// redraw if size changes 
    wcx.lpfnWndProc = CBaseWindow::stWinMsgHandler;	// points to window procedure 
    wcx.cbClsExtra = 0;								// no extra class memory 
    wcx.cbWndExtra = 0;								// no extra window memory 
    wcx.hInstance = hInstance;						// handle to instance 
    wcx.hIcon = hIcon;								// predefined app. icon 
    wcx.hCursor = hCursor;							// predefined arrow 
    wcx.hbrBackground = hbrBackground;				// white background brush 
    wcx.lpszMenuName = lpszMenuName;				// name of menu resource 
    wcx.lpszClassName = lpszClassName;				// name of window class 
    wcx.hIconSm = hIconSm;							// small class icon 
 
    // Register the window class. 
    return RegisterWindow(&wcx); 
}

BOOL CBaseWindow::RegisterWindow(CONST WNDCLASSEX* wcx)
{
	// Register the window class. 
	ZeroMemory(szClassName, sizeof(char)*256);
	strcpy(szClassName, wcx->lpszClassName);

	if (RegisterClassEx(wcx) == 0)
		return FALSE;
	else
		return TRUE;
}

/*
	You can not initialize the window class with a class method as the window 
	procedure unless it is a static method, so the class also needs a static 
	message handler that determines which instance of the class is the recipient 
	of the message and calls that instance's window procedure.

	See "http://www.gamedev.net/reference/articles/article1810.asp" for more info.
*/
LRESULT CALLBACK CBaseWindow::stWinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CBaseWindow* pWnd;

	if (uMsg == WM_NCCREATE)
	{
		// get the pointer to the window from lpCreateParams which was set in CreateWindow
		SetWindowLong(hwnd, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
	}

	// get the pointer to the window
	pWnd = GetObjectFromWindow(hwnd);

	// if we have the pointer, go to the message handler of the window
	// else, use DefWindowProc
	if (pWnd)
		return pWnd->WinMsgHandler(hwnd, uMsg, wParam, lParam);
	else
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CBaseWindow::Create()
{ 
	// Create the window
	RECT rect;

	rect.top = 0;
	rect.left = 0;
	rect.right = 600;
	rect.bottom = 400;

	return Create(WS_OVERLAPPEDWINDOW | WS_VISIBLE, &rect);
}

BOOL CBaseWindow::Create(DWORD dwStyles, RECT* rect)
{ 
	// Create the window
	
	// send the this pointer as the window creation parameter
	m_hwnd = CreateWindow(szClassName, szWindowTitle, dwStyles, rect->left, rect->top, 
		rect->right - rect->left, rect->bottom - rect->top, NULL, NULL, hInstance, 
		(void *)this);

	return (m_hwnd != NULL);
} 

void CBaseWindow::MsgLoop()
{
  MSG msg;
  BOOL bRet;
  while((bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
  { 
	  if (bRet == -1)
	  {
		  // handle the error and possibly exit
		  break;
	  }
	  else
	  {
		  TranslateMessage(&msg); 
		  DispatchMessage(&msg); 
		  if (IsWindowClosed()) 
			  break;
	  }
  }
}