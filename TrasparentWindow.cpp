#include "TrasparentWindow.h"

TrasparentWindow::~TrasparentWindow(void)
{
}

BOOL TrasparentWindow::Create()
{ 
	RECT rect;
	int cx = GetSystemMetrics(SM_CXSCREEN) ;
	int cy = GetSystemMetrics(SM_CYSCREEN) ;
	rect.top     = 0 ;
	rect.left    = 0 ;
	rect.right   = cx ;
	rect.bottom  = cy ;     

	// No border window 
	return CBaseWindow::Create(~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU | WS_HSCROLL | WS_VSCROLL), &rect) ;
}

LRESULT CALLBACK TrasparentWindow::WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		 {
			 // Set WS_EX_LAYERED on this window 
			 SetWindowLong (hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED /*| WS_EX_TRANSPARENT*/);
			 // Make this window 70% alpha
			 SetLayeredWindowAttributes (hwnd, 0, (255 * 1) / 100, LWA_ALPHA) ;
		 }			
		//OnCreate();
		break ;		
	case WM_PAINT:
		{
			RECT               rcClient;
			PAINTSTRUCT        ps;	
			HDC                hdcClient, hdcMem;	
			
			hdcClient = BeginPaint(hwnd, &ps);
			GetClientRect(hwnd, &rcClient) ;
			hdcMem = CreateCompatibleDC (hdcClient) ;	
			TextOut(hdcClient, 500, 500, TEXT("No Border window"), 30);
			EndPaint (hwnd, &ps);
		}
		//OnPaint () ;
		break ;
	case WM_LBUTTONDOWN:		
		//OnLButtonDown (wParam, lParam) ;
		return 0 ;
	case WM_LBUTTONUP:
		//OnLButtonUp (wParam, lParam) ;
		return 0 ;
	case WM_RBUTTONDOWN:
		//OnRButtonDown (wParam, lParam) ;
		return 0 ;	
	case WM_RBUTTONUP:
		//OnRButtonUp (wParam, lParam) ;
		 return 0 ;
	case WM_MOUSEMOVE:
		//OnMouseMove (wParam, lParam);
		return 0 ;	
	case WM_DESTROY:
		bWindowClosed = TRUE;
		break ;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam) ;
	}
	return 0 ;
}