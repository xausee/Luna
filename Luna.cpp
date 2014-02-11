#include "Luna.h"
#include "MouseHook.h"

LRESULT CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{	
	case WM_COMMAND:
		switch (LOWORD (wParam)) 
        {		
		case IDOK: 
			EndDialog(hwndDlg, wParam) ; 
			break ;
		default: 
			break ; 
		} 
		return 0 ; 	
	case WM_DESTROY:
		EndDialog(hwndDlg, wParam) ; 
		break ;
	default:
		return DefWindowProc(hwndDlg, uMsg, wParam, lParam) ;
	}
	return DefWindowProc(hwndDlg, uMsg, wParam, lParam) ;
}

LRESULT CALLBACK CDerivedWindow::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	capture->hwndClient = hwnd ;
	capture->hwndScreen = GetDesktopWindow () ;	
	
	switch (uMsg)
	{
	case WM_CREATE:		
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
		case ID_EDIT:
			CreateToolbar() ;
			break ;
		case ID_HELP_ABOUT:
			{	
				HWND hAbout = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_ABOUT_DIALOG), NULL, (DLGPROC)DialogProc) ; 
				ShowWindow(hAbout, SW_SHOW) ;
			}
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

		POINT point  ;
		float rate ;
		int clientWidth, clientHeitht , Width, Height;
		clientWidth = rcClient.right - rcClient.left ;
		clientHeitht = rcClient.bottom - rcClient.top ;

		if (bm.bmWidth + 20 < clientWidth &&  bm.bmHeight + 20 < clientHeitht)
		{
			point.x = (clientWidth - bm.bmWidth - 20) / 2 ;
			point.y = (clientHeitht - bm.bmHeight - 20) / 2 ;
			// draw the border of the picture first
			Rectangle(hdcClient, point.x - 1, point.y - 1, point.x + bm.bmWidth + 1, point.y + bm.bmHeight + 1) ; 
			// draw the bitmap
			BitBlt(hdcClient, point.x, point.y, bm.bmWidth, bm.bmHeight,  hdcMem, 0, 0, SRCCOPY) ;
		}
		else
		{			
			if ((float)bm.bmWidth / (float)clientWidth < (float)bm.bmHeight / (float)clientHeitht)
			{
				Height = clientHeitht - 20 ;
				rate = (float)Height / (float)bm.bmHeight ;
				Width = (int)(bm.bmWidth * rate) ;
				point.x = (clientWidth - Width - 10) / 2 ;
				point.y = 10 ;
			}
			else
			{
				Width = clientWidth - 20 ;
				rate = (float)Width / (float)bm.bmWidth ;
				Height = (int)(bm.bmHeight * rate) ;
				point.x = 10 ;
				point.y = (clientHeitht - Height - 10) / 2 ;
			}
			// draw the border of the picture first
			Rectangle(hdcClient, point.x - 1, point.y - 1, point.x + Width + 1, point.y + Height + 1) ; 
			// draw the bitmap
			StretchBlt (hdcClient, point.x, point.y, Width, Height, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY) ;
		}		

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
	Sleep (500) ;
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

void CDerivedWindow::CreateToolbar ()
{
	//// Load toolbar bitmap. (48 pixel wide, 16 high)
	//HBITMAP hbmTool = (HBITMAP)LoadImage(hInstance, "toolbar.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
 // 
 //   // blah blah, setup buttons..

 //   HWND hWnd = CreateWindowEx (m_hwnd, WS_CHILD | WS_VISIBLE, IDTB_MAIN,  3, NULL, (UINT)hbmTool, tbButtons,			  
 //                               3, 16, 16, 16, 16, sizeof(TBBUTTON)); 


	HIMAGELIST g_hImageList = NULL;
	// Declare and initialize local constants.
    const int ImageListID    = 0;
    const int numButtons     = 3;
    const int bitmapSize     = 16;
    
    const DWORD buttonStyles = BTNS_AUTOSIZE;

    // Create the toolbar.
    HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, 
                                      WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0, 
                                      m_hwnd, NULL, hInstance, NULL);
        
    if (hWndToolbar == NULL)
        return ;

    // Create the image list.
    g_hImageList = ImageList_Create(bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
                                    ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
                                    numButtons, 0);

    // Set the image list.
    SendMessage(hWndToolbar, TB_SETIMAGELIST, (WPARAM)ImageListID, (LPARAM)g_hImageList);

    // Load the button images.
    SendMessage(hWndToolbar, TB_LOADIMAGES, (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);

    // Initialize button info.
    // IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.
    
    TBBUTTON tbButtons[numButtons] = 
    {
        { /*MAKEINTRESOURCEW(IDR_TOOLBAR)*/MAKELONG(STD_FILENEW,  ImageListID), ID_EDIT,  TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"New" },
        { MAKELONG(IDR_TOOLBAR, ImageListID), ID_EDIT, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"Open"},
        { MAKELONG(STD_FILESAVE, ImageListID), ID_EDIT, 0,               buttonStyles, {0}, 0, (INT_PTR)L"Save"}
    };

    // Add buttons.
    SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hWndToolbar, TB_ADDBUTTONS,       (WPARAM)numButtons,       (LPARAM)&tbButtons);

    // Resize the toolbar, and then show it.
    SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0); 
    ShowWindow(hWndToolbar,  TRUE);
    
    return /*hWndToolbar*/;
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