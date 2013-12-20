#include "MouseHook.h"

MouseHook::MouseHook(void)
{		
}

MouseHook::MouseHook(HINSTANCE hInstance)
{	
	hookData.hInst =  hInstance ;	
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
	HWND hwndPointNow = NULL ;
	static HWND hwndPointOld = NULL ;
	HWND hwndLuna = FindWindow("CapturerWindow", "Luna") ;
    MOUSEHOOKSTRUCT *msg = (MOUSEHOOKSTRUCT *)lParam ;	

    switch (nCode) 
    { 
    case HC_ACTION: 
        {    
			hwndPointNow = WindowFromPoint (msg->pt) ;			
			if (hwndPointOld != hwndPointNow)
			{
				InvalidateRect (hwndPointOld, NULL, TRUE) ;
				hwndPointOld = hwndPointNow ; 
				if (hwndPointOld) 
				{		
					GetClientRect (hwndPointOld, &rect) ;
					hdc = GetDC (hwndPointOld) ;
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
					ReleaseDC (hwndPointOld, hdc) ;
				}
			}

			switch(wParam)
			{
			case WM_LBUTTONDOWN:
				SendMessage (hwndLuna, WM_LBUTTONDOWN, NULL, NULL) ;				
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
	hookData.hMouse = SetWindowsHookEx (WH_MOUSE_LL, MouseHook::MouseProc, hookData.hInst, 0) ;
	hookData.hKeyb = SetWindowsHookEx (WH_KEYBOARD_LL, MouseHook::KeybdProc, hookData.hInst, 0) ;	
}

void MouseHook::UnHook ()
{
	UnhookWindowsHookEx (hookData.hMouse) ;
	UnhookWindowsHookEx (hookData.hKeyb) ;
}