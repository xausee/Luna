#include "MouseHook.h"

MouseHook::MouseHook(void)
{	
	hookData.hwndPointNow = NULL ;		
}

MouseHook::MouseHook(HINSTANCE hInstance)
{	
	hookData.hInst =  hInstance ;
	hookData.hwndPointNow = NULL ;
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
    MOUSEHOOKSTRUCT *msg = (MOUSEHOOKSTRUCT *)lParam ;

    switch (nCode) 
    { 
    case HC_ACTION: 
        {    
			if (hookData.hwndPointNow != WindowFromPoint (msg->pt))
			{
				InvalidateRect (hookData.hwndPointNow, NULL, TRUE) ;
				hookData.hwndPointNow = WindowFromPoint(msg->pt) ; 
				if (hookData.hwndPointNow) 
				{		
					GetClientRect (hookData.hwndPointNow, &rect) ;
					hdc = GetDC (hookData.hwndPointNow) ;
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
					ReleaseDC (hookData.hwndPointNow, hdc) ;
				}
			}

			switch(wParam)
			{
			case WM_LBUTTONDOWN:				
				{	
					HWND hwnd = FindWindow("CapturerWindow", "Luna") ;					
					SendMessage (hwnd, WM_LBUTTONDOWN, NULL, NULL) ;
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
	hookData.hMouse = SetWindowsHookEx (WH_MOUSE_LL, MouseHook::MouseProc, hookData.hInst, 0) ;
	hookData.hKeyb = SetWindowsHookEx (WH_KEYBOARD_LL, MouseHook::KeybdProc, hookData.hInst, 0) ;	
}

void MouseHook::UnHook ()
{
	UnhookWindowsHookEx (hookData.hMouse) ;
	UnhookWindowsHookEx (hookData.hKeyb) ;
}