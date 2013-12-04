#include "Luna.h"

LRESULT CALLBACK CDerivedWindow::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		//OnCreate();
		break;		
	case WM_PAINT:
		OnPaint();
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(wParam, lParam);
		return 0;
	case WM_RBUTTONDOWN:
		OnRButtonDown(wParam, lParam);
		return 0;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		OnRButtonUp(wParam, lParam);
		 return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, lParam);
		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam)) 
        {
		case ID_FILE_SAVEAS:
			capture->SaveBitmap(hBitmap);		
			break;
		case ID_CAPTURER_FULLSCREEN:
			hBitmap = capture->CaptureFullScreen () ;		
			break;			
	    case ID_CAPTURER_SINGLEWINDOW: 
			exit(0);  
			break;
		case ID_CAPTURER_ANYAREA:
			exit(0);   
            break; 
		case ID_CAPTURER_EXIT: 
			exit(0);
		case ID_HELP_ABOUT:
			SendMessage(hwnd,WM_SYSCOMMAND,SC_MINIMIZE,0);			
			break;
		default: 
			break; 
		} 
		return 0; 
	case WM_DESTROY:
		bWindowClosed = TRUE;
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void CDerivedWindow::OnCreate()
{
	#define         BITSPERPIXEL 32;
	LPVOID		    pBits=NULL;
	static HBITMAP	hDesktopCompatibleBitmap=NULL;
	static HDC		hDesktopCompatibleDC=NULL;
	static HWND		hDesktopWnd=NULL;
	static NOTIFYICONDATA	nid;

	BITMAPINFO	bmpInfo;	
	ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biBitCount=BITSPERPIXEL;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biWidth=GetSystemMetrics(SM_CXSCREEN);
	bmpInfo.bmiHeader.biHeight=GetSystemMetrics(SM_CYSCREEN);
	bmpInfo.bmiHeader.biPlanes=1;
	bmpInfo.bmiHeader.biSizeImage=abs(bmpInfo.bmiHeader.biHeight)*bmpInfo.bmiHeader.biWidth*bmpInfo.bmiHeader.biBitCount/8;
	hDesktopWnd = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktopWnd);
	hDesktopCompatibleDC = CreateCompatibleDC(hDesktopDC);
	hDesktopCompatibleBitmap = CreateDIBSection(hDesktopDC,&bmpInfo,DIB_RGB_COLORS,&pBits,NULL,0);
	if(hDesktopCompatibleDC == NULL || hDesktopCompatibleBitmap == NULL)
	{
		MessageBox(m_hwnd, "Unable to Create Desktop Compatible DC/Bitmap", "error",MB_OK);	
		return ;
	}
	SelectObject(hDesktopCompatibleDC,hDesktopCompatibleBitmap);
	ZeroMemory(&nid,sizeof(nid));
	nid.cbSize=sizeof(nid);
	nid.uID=1000;
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_DEFAULT));
	nid.hWnd=m_hwnd;
	strcpy(nid.szTip,"Screen Capture Application - Double Click to Start Capturing");
	nid.uCallbackMessage=NULL;//WM_NOTIFYICON_MESSAGE;
	//if(!Shell_NotifyIcon(NIM_ADD,&nid))	MessageBox(NULL,"Unable to Set Notification Icon","Error",MB_ICONINFORMATION|MB_OK);
	//if((ghMenu=LoadMenu(hInst,MAKEINTRESOURCE(IDC_SCREENCAPTURE)))==NULL)
	//{
	//	ErrorMessage("Unable to Load Menu");	return -1;
	//}
	//nTimerId=SetTimer(hWnd,12345,500,NULL);	//Timer set to 500 ms.
}

void CDerivedWindow::OnPaint()
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
		EndPaint (m_hwnd, &ps);
	}			
}	

void CDerivedWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (!capture->bCapturing)    
	{
		capture->hwndScreen = GetDesktopWindow ();
		if (LockWindowUpdate (capture->hwndScreen))
		{
			capture->bCapturing = true ;
			SetCapture (m_hwnd) ;
			SetCursor (LoadCursor (NULL, IDC_CROSS)) ;
		}
		else
			MessageBeep (0) ;
	}	
}

void CDerivedWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
}

void CDerivedWindow::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (capture->bCapturing)
	{
		capture->bBlocking = true ;
		capture->ptBeg.x = LOWORD (lParam) ;
		capture->ptBeg.y = HIWORD (lParam) ;
		capture->ptEnd = capture->ptBeg ;
		//capture->InvertBlock (m_hwnd);
		InvertBlock (capture->hwndScreen, m_hwnd, capture->ptBeg, capture->ptEnd) ;
	}       
}

void CDerivedWindow::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
	if (capture->bBlocking)
	{
		//capture->InvertBlock (m_hwnd);
		InvertBlock (capture->hwndScreen, m_hwnd, capture->ptBeg, capture->ptEnd) ;
		capture->ptEnd.x = LOWORD (lParam) ;
		capture->ptEnd.y = HIWORD (lParam) ;
		
		if (hBitmap)
		{
			DeleteObject (hBitmap) ;
			hBitmap = NULL ;
		}
		
		HDC hdc = GetDC (m_hwnd) ;
		HDC hdcMem = CreateCompatibleDC (hdc) ;
		hBitmap = CreateCompatibleBitmap (hdc, abs (capture->ptEnd.x - capture->ptBeg.x), abs (capture->ptEnd.y - capture->ptBeg.y)) ;
		SelectObject (hdcMem, hBitmap) ;
		StretchBlt (hdcMem, 0, 0, abs (capture->ptEnd.x - capture->ptBeg.x),
                                  abs (capture->ptEnd.y - capture->ptBeg.y), 
                    hdc, capture->ptBeg.x, capture->ptBeg.y, capture->ptEnd.x - capture->ptBeg.x, 
                    capture->ptEnd.y - capture->ptBeg.y, SRCCOPY) ;
		
		DeleteDC (hdcMem) ;
		ReleaseDC (m_hwnd, hdc) ;
		InvalidateRect (m_hwnd, NULL, TRUE) ;
	}

	if (capture->bBlocking || capture->bCapturing)
	{
		capture->bBlocking = capture->bCapturing = false ;
		SetCursor (LoadCursor (NULL, IDC_ARROW)) ;
		ReleaseCapture () ;
		LockWindowUpdate (NULL) ;
	}      
}

void CDerivedWindow::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	if (capture->bBlocking)
	{
		//capture->InvertBlock (m_hwnd);
		InvertBlock (capture->hwndScreen, m_hwnd, capture->ptBeg, capture->ptEnd) ;
		capture->ptEnd.x = LOWORD (lParam) ;
		capture->ptEnd.y = HIWORD (lParam) ;
		//capture->InvertBlock (m_hwnd);
		InvertBlock (capture->hwndScreen, m_hwnd, capture->ptBeg, capture->ptEnd) ;		
	}        
}

void CDerivedWindow::InvertBlock (HWND hwndScreen, HWND hwnd, POINT ptBeg, POINT ptEnd)
{	
     HDC hdc = GetDCEx (hwndScreen, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE) ;	 
     ClientToScreen (hwnd, &ptBeg) ;
     ClientToScreen (hwnd, &ptEnd) ;
     PatBlt (hdc, capture->ptBeg.x, capture->ptBeg.y, capture->ptEnd.x - capture->ptBeg.x, capture->ptEnd.y - capture->ptBeg.y, DSTINVERT) ;	 
     ReleaseDC (hwndScreen, hdc) ;
}