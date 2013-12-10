#define WIN32_LEAN_AND_MEAN
#include "Luna.h"

HINSTANCE g_hInst ;
HHOOK g_hMouse ;
HHOOK g_hKeyb ;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam )
{
	PKBDLLHOOKSTRUCT k = (PKBDLLHOOKSTRUCT)(lParam);	
	int xMetric = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	
	HDC   hdc ;
	RECT  rect ;	
	HPEN  hpen ;
	POINT pNow           = {0,0};	
	HWND  hwndPointNow   = NULL ;

	if (GetCursorPos(&pNow)) 
	{  	
		hwndPointNow = WindowFromPoint(pNow);  
		char title[30];
		GetWindowText (hwndPointNow,  title, 30) ;
		if (hwndPointNow) 
		{
			GetClientRect (hwndPointNow, &rect);
			hdc = GetDC (hwndPointNow) ;
			hpen = CreatePen (PS_SOLID, 5, RGB (255, 78, 111)) ;
			SelectObject (hdc, hpen) ;		

			MoveToEx(hdc, rect.left, rect.top, (LPPOINT) NULL); 
			LineTo(hdc, rect.right, rect.top); 
			MoveToEx(hdc, rect.right, rect.top, (LPPOINT) NULL); 
			LineTo(hdc, rect.right, rect.bottom); 
			MoveToEx(hdc, rect.right, rect.bottom, (LPPOINT) NULL); 
			LineTo(hdc, rect.left, rect.bottom); 
			MoveToEx(hdc, rect.left, rect.bottom, (LPPOINT) NULL); 
			LineTo(hdc, rect.left, rect.top); 
			
			DeleteObject (hpen) ;
			ReleaseDC (hwndPointNow, hdc) ;
		}
	} 
	else 
		MessageBox(NULL, "dd", "info", MB_OK);	

	return CallNextHookEx(NULL,nCode,wParam,lParam);  	
}

LRESULT CALLBACK KeybdProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)  
		return CallNextHookEx(NULL, nCode, wParam, lParam); 
    
	return CallNextHookEx (NULL, nCode, wParam, lParam) ;
}

static void SetHook () 
{	
	g_hMouse = SetWindowsHookEx (WH_MOUSE_LL,MouseProc, g_hInst, 0) ;
	g_hKeyb = SetWindowsHookEx (WH_KEYBOARD_LL, KeybdProc, g_hInst, 0) ;	
}

void UnHook ()
{
	UnhookWindowsHookEx (g_hMouse) ;
	UnhookWindowsHookEx (g_hKeyb) ;
}

int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	g_hInst = hInstance ;
	SetHook () ;

	DWORD dwError = 0 ; 	
	CDerivedWindow mainWnd (hInstance) ;
	
	WNDCLASSEX wcx;
	wcx.cbSize              = sizeof (WNDCLASSEX) ;							        // size of structure 
    wcx.style               = CS_HREDRAW | CS_VREDRAW ;						        // redraw if size changes 
    wcx.lpfnWndProc         = CBaseWindow::stWinMsgHandler ;				        // points to window procedure 
    wcx.cbClsExtra          = 0 ;											        // no extra class memory 
    wcx.cbWndExtra          = 0 ;											        // no extra window memory 
    wcx.hInstance           = hInstance;									        // handle to instance 
    wcx.hIcon               = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_ICON)) ;	// predefined app. icon 
    wcx.hCursor             = LoadCursor (NULL, IDC_ARROW );					    // predefined arrow 
    wcx.hbrBackground       = (HBRUSH)GetStockObject (WHITE_BRUSH) ;	            // white background brush 
    wcx.lpszMenuName        = MAKEINTRESOURCE (IDM_CAPTURER) ;			            // name of menu resource 
    wcx.lpszClassName       = "CapturerWindow" ;						            // name of window class 
    wcx.hIconSm             = LoadIcon(hInstance, MAKEINTRESOURCE (IDI_ICON)) ;	    // small class icon 	
	 
	if (mainWnd.RegisterWindow (&wcx))
	{				
		if (mainWnd.Create ())
		{				
			mainWnd.MsgLoop () ;			
		}
		else
			dwError = GetLastError () ;
	}
	dwError = GetLastError () ;	
	UnHook ();
	return dwError ;
} 