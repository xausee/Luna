#include "MouseHook.h"

MouseHook::MouseHook(void)
{	
	hookData.g_hwndPointNow = NULL ;
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
	LPMOUSEHOOKSTRUCT lpMouse=(MOUSEHOOKSTRUCT FAR*)lParam; 

	HDC   hdc ;
	RECT  rect ;	
	HPEN  hpen ;	
	
	if (hookData.g_hwndPointNow != WindowFromPoint (lpMouse->pt))
	{
		InvalidateRect (hookData.g_hwndPointNow, NULL, TRUE) ;
		hookData.g_hwndPointNow = WindowFromPoint(lpMouse->pt);  
		if (hookData.g_hwndPointNow) 
		{
			GetClientRect (hookData.g_hwndPointNow, &rect);
			hdc = GetDC (hookData.g_hwndPointNow) ;
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
			ReleaseDC (hookData.g_hwndPointNow, hdc) ;
		}
	}	
	return CallNextHookEx(NULL,nCode,wParam,lParam);  
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