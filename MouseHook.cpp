#include "MouseHook.h"

MouseHook::MouseHook(void)
{	
	hookData.g_hwndPointNow = NULL ;
	hookData.hBitmap = NULL ;
	hookData.captured = false ;
}

MouseHook::MouseHook(HINSTANCE hInstance)
{	
	hookData.g_hInst =  hInstance ;
	hookData.g_hwndPointNow = NULL ;
}

MouseHook::~MouseHook(void)
{
}

LRESULT CALLBACK MouseHook::MouseMsgHandler(int nCode, WPARAM wParam, LPARAM lParam )
{
	if (nCode < 0) 
		return CallNextHookEx(NULL, nCode, wParam, lParam) ; 

	HDC   hdc ;
	RECT  rect ;	
	HPEN  hpen ;
    MOUSEHOOKSTRUCT *msg = (MOUSEHOOKSTRUCT *)lParam;

    switch (nCode) 
    { 
    case HC_ACTION: 
        {    
			if (hookData.g_hwndPointNow != WindowFromPoint (msg->pt))
			{
				InvalidateRect (hookData.g_hwndPointNow, NULL, TRUE) ;
				hookData.g_hwndPointNow = WindowFromPoint(msg->pt); 
				if (hookData.g_hwndPointNow) 
				{		
					GetClientRect (hookData.g_hwndPointNow, &rect) ;
					hdc = GetDC (hookData.g_hwndPointNow) ;
					hpen = CreatePen (PS_SOLID, 5, RGB (255, 78, 111)) ;
					SelectObject (hdc, hpen) ;	
			
			        MoveToEx (hdc, rect.left, rect.top, (LPPOINT) NULL); 
					LineTo (hdc, rect.right, rect.top) ; 
					MoveToEx (hdc, rect.right, rect.top, (LPPOINT) NULL) ; 
					LineTo (hdc, rect.right, rect.bottom) ; 
					MoveToEx (hdc, rect.right, rect.bottom, (LPPOINT) NULL) ; 
					LineTo (hdc, rect.left, rect.bottom) ; 
					MoveToEx (hdc, rect.left, rect.bottom, (LPPOINT) NULL) ; 
					LineTo (hdc, rect.left, rect.top) ; 	
					
					DeleteObject (hpen) ;
					ReleaseDC (hookData.g_hwndPointNow, hdc) ;
				}
			}

			switch(wParam)
			{
			case WM_LBUTTONDOWN:				
				{					
					HDC hdc = GetDC (hookData.g_hwndPointNow) ;
					HDC hdcMem = CreateCompatibleDC (hdc) ;
					RECT rcClient ;					
					HWND hwnd = FindWindow("CapturerWindow", "Luna") ;
					GetClientRect (hwnd, &rcClient) ;
					hookData.hBitmap = CreateCompatibleBitmap (hdc, abs (rcClient.right - rcClient.left), abs (rcClient.bottom - rcClient.top)) ;
					SelectObject (hdcMem, hookData.hBitmap) ;
				    StretchBlt (hdcMem, 0, 0, abs (rcClient.right - rcClient.left), abs (rcClient.bottom - rcClient.top), hdc, 0, 0, abs (rcClient.right - rcClient.left), abs (rcClient.bottom - rcClient.top), SRCCOPY) ;
		
					if (hookData.hBitmap)
						hookData.captured = true ;

					DeleteDC (hdc) ;	
                    DeleteDC (hdcMem) ;
				}
				break ;
			case WM_RBUTTONDOWN: 
				break ;
			case WM_LBUTTONDBLCLK:  				
				break ;
			}
		} 
		break ;
      default :
        break ;
    }
	return CallNextHookEx(NULL, nCode, wParam, lParam) ; 
}

LRESULT CALLBACK MouseHook::MouseProc(int nCode, WPARAM wParam, LPARAM lParam )
{
	MouseHook hook;
	return hook.MouseMsgHandler(nCode, wParam, lParam) ;	
}

LRESULT CALLBACK MouseHook::KeybdProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)  
		return CallNextHookEx(NULL, nCode, wParam, lParam); 
    
	return CallNextHookEx (NULL, nCode, wParam, lParam) ;
}

void MouseHook::SetHook () 
{	
	hookData.g_hMouse = SetWindowsHookEx (WH_MOUSE_LL, MouseHook::MouseProc, hookData.g_hInst, 0) ;
	hookData.g_hKeyb = SetWindowsHookEx (WH_KEYBOARD_LL, MouseHook::KeybdProc, hookData.g_hInst, 0) ;	
}

void MouseHook::UnHook ()
{
	UnhookWindowsHookEx (hookData.g_hMouse) ;
	UnhookWindowsHookEx (hookData.g_hKeyb) ;
}

DWORD WINAPI MouseHook::MouseHookThreadFunction( LPVOID lpParam ) 
{
	MessageBox(NULL, "dd", "info", MB_OK);
	SetHook () ;
	return 0;
}