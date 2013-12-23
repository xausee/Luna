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
	//return CBaseWindow::Create( WS_OVERLAPPEDWINDOW | WS_VISIBLE, &rect) ;
}

void TrasparentWindow::SetWindowTransparent (HWND hwnd) 
{
	// Set WS_EX_LAYERED on this window 
	SetWindowLong (hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED /*| WS_EX_TRANSPARENT*/);
	// Make this window 10% alpha
	SetLayeredWindowAttributes (hwnd, 0, (255 * 10) / 100, LWA_ALPHA) ;
}

HBITMAP TrasparentWindow::CreateDesktopBitmap ()
{		
    int nWidth = GetSystemMetrics (SM_CXSCREEN) ;
    int nHeight = GetSystemMetrics (SM_CYSCREEN) ;

	HWND hDesktopWnd =  GetDesktopWindow () ;
    HDC hDesktopDC = GetDC (hDesktopWnd) ;    
	HDC hBmpFileDC = CreateCompatibleDC (hDesktopDC) ;

    hBitmap = CreateCompatibleBitmap (hDesktopDC,nWidth,nHeight) ;
    HBITMAP hOldBitmap = (HBITMAP) SelectObject (hBmpFileDC, hBitmap) ;
    BitBlt (hBmpFileDC, 0, 0, nWidth, nHeight, hDesktopDC, 0, 0, SRCCOPY|CAPTUREBLT) ;
    SelectObject (hBmpFileDC,hOldBitmap) ; 
	 
	DeleteDC (hDesktopDC) ;
	DeleteObject (hDesktopWnd) ;
	DeleteObject (hOldBitmap) ;
	DeleteDC (hBmpFileDC) ;
	return hBitmap ;
}

void TrasparentWindow::OnCreate (HWND hwnd)
{	
	CreateDesktopBitmap ();	
    //Todo: m_hwnd != hwnd?
	//SetWindowTransparent (hwnd) ;
}

void TrasparentWindow::OnPaint ()
{
	if (hBitmap)
	{
		BITMAP             bm ;
		RECT               rcClient;
		PAINTSTRUCT        ps;	
		HDC                hdcClient, hdcMem;
		
		GetClientRect (m_hwnd, &rcClient) ;
		hdcClient = BeginPaint(m_hwnd, &ps);
		GetClientRect(m_hwnd, &rcClient) ;
		hdcMem = CreateCompatibleDC (hdcClient) ;
		SelectObject (hdcMem, hBitmap) ;
		GetObject (hBitmap, sizeof (BITMAP), (PSTR) &bm) ;
		SetStretchBltMode (hdcClient, COLORONCOLOR) ;
		StretchBlt (hdcClient, 0, 0, rcClient.right, rcClient.bottom,
                   hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY) ;	
		
		DeleteDC (hdcMem) ;
		EndPaint (m_hwnd, &ps) ;
	}	
}

LRESULT CALLBACK TrasparentWindow::WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HBITMAP hBitmap = NULL ;

	switch (uMsg)
	{
	case WM_CREATE:			
		OnCreate(hwnd) ;
		break ;		
	case WM_PAINT:		
		OnPaint () ;
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