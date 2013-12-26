#include "TrasparentWindow.h"

TrasparentWindow::~TrasparentWindow(void)
{ 
	delete (capture) ; 
	capture = NULL ;
}

BOOL TrasparentWindow::Create()
{ 
	RECT rect;
	int cx = GetSystemMetrics (SM_CXSCREEN) ;
	int cy = GetSystemMetrics (SM_CYSCREEN) ;
	int cf = GetSystemMetrics (SM_CXFRAME) ;
	int cc = GetSystemMetrics (SM_CYCAPTION) ;
	rect.top     = 0 ;
	rect.left    = 0 ;
	rect.right   = cx ;
	rect.bottom  = cy ; 

	if (!CBaseWindow::Create (0, &rect))
		return false;
	SetWindowLong (m_hwnd, GWL_STYLE, 0);	
	SetWindowPos (m_hwnd, HWND_TOPMOST, 0, 0, cx, cy + cc - 20, SWP_SHOWWINDOW) ;	
	ShowWindow (m_hwnd, SW_SHOW) ;

	return true ;
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
	SetCapture (hwnd) ;
	SetCursor (LoadCursor (NULL, IDC_CROSS)) ;
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

		//Invert the solor of the all client area
		//HDC hdc = GetDCEx (m_hwnd, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE) ;
		//PatBlt (hdcClient, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, DSTINVERT) ;

		DeleteDC (hdcMem) ;
		DeleteDC (hdcClient) ;
		EndPaint (m_hwnd, &ps) ;
	}	
}

void TrasparentWindow::OnLButtonDown (WPARAM wParam, LPARAM lParam)
{	
	POINT pBeg;
	pBeg.x = LOWORD (lParam) ;
	pBeg.y = HIWORD (lParam) ;
	capture->InitCaptureAnyArea () ; 
	capture->StartCaptureAnyArea(pBeg) ;
}

void TrasparentWindow::OnLButtonUp (WPARAM wParam, LPARAM lParam)
{
	POINT pEnd;
	pEnd.x = LOWORD (lParam) ;
	pEnd.y = HIWORD (lParam) ;
	hBitmap = capture->EndCaptureAnyArea (pEnd) ;	
	DestroyWindow (m_hwnd) ;
}

void TrasparentWindow::OnMouseMove (WPARAM wParam, LPARAM lParam)
{	
	POINT pEnd;
    pEnd.x = LOWORD (lParam) ;
	pEnd.y = HIWORD (lParam) ;
	capture->MarkCaptureArea (pEnd) ;	
}

LRESULT CALLBACK TrasparentWindow::WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HBITMAP hBitmap = NULL ;
	capture->hwndClient = hwnd ;
	capture->hwndScreen = GetDesktopWindow () ;	

	switch (uMsg)
	{
	case WM_CREATE:			
		OnCreate(hwnd) ;
		break ;		
	case WM_PAINT:		
		OnPaint () ;
		break ;
	case WM_LBUTTONDOWN:		
		OnLButtonDown (wParam, lParam) ;
		return 0 ;
	case WM_LBUTTONUP:
		OnLButtonUp (wParam, lParam) ;
		return 0 ;
	case WM_MOUSEMOVE:
		OnMouseMove (wParam, lParam);
		return 0 ;	
	case WM_DESTROY:
		bWindowClosed = TRUE;
		break ;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam) ;
	}
	return 0 ;
}