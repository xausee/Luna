#include "Luna.h"
#include "MouseHook.h"

LRESULT CALLBACK CDerivedWindow::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	capture->hwndClient = hwnd ;
	capture->hwndScreen = GetDesktopWindow () ;	
	
	switch (uMsg)
	{
	case WM_CREATE:
		//OnCreate();
		break ;		
	case WM_PAINT:
		OnPaint () ;
		break ;
	case WM_LBUTTONDOWN:		
		OnLButtonDown (wParam, lParam) ;
		return 0 ;	
	case WM_COMMAND:
		switch (LOWORD (wParam)) 
        {
		case ID_FILE_SAVEAS:
			SaveFile () ;				
			break ;
		case ID_CAPTURER_FULLSCREEN:
			hBitmap = capture->CaptureFullScreen () ;		
			break ;			
	    case ID_CAPTURER_SPECIFIED_WINDOW: 
			OnCaptureSpecifiedWindow () ;  
			break ;
		case ID_CAPTURER_ANYAREA:
			 OnCaptureAnyArea () ;
            break ; 
		case ID_EXIT: 
			exit (0) ;
		case ID_HELP_ABOUT:
			MessageBox (hwnd, "Copyright@Phiso Hu 2013", "About Luna", MB_OK) ;			
			break ;
		default: 
			break ; 
		} 
		return 0 ; 
	case WM_DESTROY:
		bWindowClosed = TRUE;
		break ;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam) ;
	}
	return 0 ;
}

void CDerivedWindow::OnCreate()
{
	#define         BITSPERPIXEL 32 ;
	LPVOID		    pBits = NULL ;
	static HBITMAP	hDesktopCompatibleBitmap = NULL ;
	static HDC		hDesktopCompatibleDC = NULL ;
	static HWND		hDesktopWnd = NULL ;
	static NOTIFYICONDATA	nid ;

	BITMAPINFO	bmpInfo ;	
	ZeroMemory (&bmpInfo,sizeof (BITMAPINFO)) ;
	bmpInfo.bmiHeader.biSize = sizeof (BITMAPINFOHEADER) ;
	bmpInfo.bmiHeader.biBitCount = BITSPERPIXEL ;
	bmpInfo.bmiHeader.biCompression = BI_RGB ;
	bmpInfo.bmiHeader.biWidth = GetSystemMetrics (SM_CXSCREEN) ;
	bmpInfo.bmiHeader.biHeight = GetSystemMetrics (SM_CYSCREEN) ;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biSizeImage = abs (bmpInfo.bmiHeader.biHeight) * bmpInfo.bmiHeader.biWidth*bmpInfo.bmiHeader.biBitCount/8 ;
	hDesktopWnd = GetDesktopWindow () ;
    HDC hDesktopDC = GetDC (hDesktopWnd) ;
	hDesktopCompatibleDC = CreateCompatibleDC (hDesktopDC) ;
	hDesktopCompatibleBitmap = CreateDIBSection (hDesktopDC,&bmpInfo,DIB_RGB_COLORS,&pBits,NULL,0) ;
	if(hDesktopCompatibleDC == NULL || hDesktopCompatibleBitmap == NULL)
	{
		MessageBox (m_hwnd, "Unable to Create Desktop Compatible DC/Bitmap", "error",MB_OK) ;	
		return ;
	}
	SelectObject (hDesktopCompatibleDC,hDesktopCompatibleBitmap) ;
	ZeroMemory (&nid,sizeof(nid)) ;
	nid.cbSize =sizeof (nid) ;
	nid.uID = 1000;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE (IDI_ICON)) ;
	nid.hWnd = m_hwnd;
	strcpy (nid.szTip,"Screen Capture Application - Double Click to Start Capturing") ;
	nid.uCallbackMessage=NULL;//WM_NOTIFYICON_MESSAGE;
	//if(!Shell_NotifyIcon(NIM_ADD,&nid))	MessageBox(NULL,"Unable to Set Notification Icon","Error",MB_ICONINFORMATION|MB_OK);
	//if((ghMenu=LoadMenu(hInst,MAKEINTRESOURCE(IDC_SCREENCAPTURE)))==NULL)
	//{
	//	ErrorMessage("Unable to Load Menu");	return -1;
	//}
	//nTimerId=SetTimer(hWnd,12345,500,NULL);	//Timer set to 500 ms.
}

void CDerivedWindow::OnPaint ()
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

void CDerivedWindow::OnLButtonDown (WPARAM wParam, LPARAM lParam)
{
	if (capture->bSpecifiedWindow)
	{
		POINT pNow ;
		GetCursorPos (&pNow) ;
		HWND hwndPointNow = WindowFromPoint(pNow);
		hBitmap = capture->CaptureSpecifiedWindow (hwndPointNow) ;
		capture->bSpecifiedWindow = false ;
		cpMouseHook->UnHook () ;			
	}	
}

void CDerivedWindow::OnCaptureAnyArea ()
{
	ShowWindow (m_hwnd, SW_HIDE) ;
	/*
	TODO: wait for Luna disapear gracefully
	*/
	Sleep (100) ;
	TrasparentWindow pTrasparentWindow (hInstance) ;
	pTrasparentWindow.RegisterWindow () ;
	pTrasparentWindow.Create () ;
	pTrasparentWindow.MsgLoop () ;	
	hBitmap = pTrasparentWindow.hBitmap ;

	ShowWindow (m_hwnd,SW_SHOW) ;	
}

void CDerivedWindow::OnCaptureSpecifiedWindow ()
{	
	capture->bSpecifiedWindow = true ;

	HINSTANCE hInstance = (HINSTANCE)GetWindowLong (m_hwnd, GWL_HINSTANCE) ;
	cpMouseHook = new MouseHook (hInstance) ;		
	cpMouseHook->SetHook ();	
}

int CDerivedWindow::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

void CDerivedWindow::SaveFile () 
{
	char szFileName[512] ;
	strcpy (szFileName,"ScreenShot") ;

	/*char  filesFilter[250];
	char * pBitmapStr  = "Bitmap Files (*.bmp)\0*.bmp\0" ;
	char * pPngStr     = "PNG Files (*.png)\0*.png\0" ;
	char * pJpgStr     = "JPG Files (*.jpg)\0*.jpg\0" ;
	char * pGifStr     = "GIF Files (*.gif)\0*.gif\0" ;
	char * pTiffStr    = "TIFF Files (*.tif)\0*.tif\0" ;
	strcat (filesFilter, pBitmapStr) ;
	strcat (filesFilter, pPngStr) ;
	strcat (filesFilter, pJpgStr) ;
	strcat (filesFilter, pGifStr) ;
	strcat (filesFilter, pTiffStr) ;*/

	OPENFILENAME	    ofn ;	
    ZeroMemory (&ofn,sizeof (ofn)) ;
    ofn.lStructSize = sizeof (OPENFILENAME) ;
    ofn.Flags = OFN_HIDEREADONLY|OFN_PATHMUSTEXIST ;
    ofn.lpstrFilter = "JPG Files (*.jpg)\0*.jpg\0Bitmap Files (*.bmp)\0*.bmp\0PNG Files (*.png)\0*.png\0GIF Files (*.gif)\0*.gif\0TIFF Files (*.tif)\0*.tif\0" ;
    ofn.lpstrDefExt = "jpg" ;
    ofn.lpstrFile = szFileName ;
    ofn.nMaxFile = 512 ;	
    ofn.hwndOwner = m_hwnd ; 
    if (!GetSaveFileName(&ofn))	
		return ;
	char *fileName = ofn.lpstrFile ;

	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	CLSID   encoderClsid;
	Status  stat;	
	Bitmap* bitmap = Bitmap::FromHBITMAP(hBitmap, NULL) ;
	
	// Get the CLSID of the PNG encoder. 
	if (strstr (fileName, "bmp") != NULL)
		GetEncoderClsid(L"image/bmp", &encoderClsid) ;
	else if (strstr (fileName, "png") != NULL)
		GetEncoderClsid(L"image/png", &encoderClsid) ;	
	else if (strstr (fileName, "jpg") != NULL)
		GetEncoderClsid(L"image/jpeg", &encoderClsid) ;
	else if (strstr (fileName, "gif") != NULL)
		GetEncoderClsid(L"image/gif", &encoderClsid) ;
	else if (strstr (fileName, "tif") != NULL)
		GetEncoderClsid(L"image/tiff", &encoderClsid) ;	
	// change filename string type
	DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, fileName, -1, NULL, 0);
	WCHAR * psText = new WCHAR[dwNum];
	if(!psText)
	{
		delete []psText;
	} 
	MultiByteToWideChar (CP_ACP, 0, fileName, -1, psText, dwNum);
	stat = bitmap->Save(psText, &encoderClsid, NULL);
	
	if(stat != Ok)
		MessageBox (NULL, "±£´æÎÄ¼þÊ§°Ü", "´íÎó", MB_OK) ;
	
	delete bitmap;
	GdiplusShutdown(gdiplusToken);
	return ;
}