#include "Luna.h"

LRESULT CALLBACK CDerivedWindow::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	capture->hwndClient = hwnd;
	capture->hwndScreen = GetDesktopWindow () ;
	
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
	case WM_LBUTTONUP:
		OnLButtonUp(wParam, lParam);
		return 0;
	case WM_RBUTTONDOWN:
		//OnRButtonDown(wParam, lParam);
		return 0;	
	case WM_RBUTTONUP:
		//OnRButtonUp(wParam, lParam);
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
			capture->InitCaptureAnyArea() ;   
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
	nid.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON));
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
	POINT pBeg;
	pBeg.x = LOWORD (lParam) ;
	pBeg.y = HIWORD (lParam) ;
	capture->StartCaptureAnyArea(pBeg) ;
}

void CDerivedWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pEnd;
	pEnd.x = LOWORD (lParam) ;
	pEnd.y = HIWORD (lParam) ;
	capture->EndCaptureAnyArea (pEnd) ;
}

void CDerivedWindow::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{	    
}

void CDerivedWindow::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{	  
}

void CDerivedWindow::OnMouseMove(WPARAM wParam, LPARAM lParam)
{	
	POINT pEnd;
    pEnd.x = LOWORD (lParam) ;
	pEnd.y = HIWORD (lParam) ;
	capture->MarkCaptureArea (pEnd);
}