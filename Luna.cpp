#include "Luna.h"
#include "MouseHook.h"

extern HBITMAP hEditWindowBitmap ;

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

DWORD WINAPI EditPictureProc(LPVOID lpParameter)
{
	Luna *luna = (Luna*)lpParameter ;
	luna->CreateChildWindow () ;
	return 0 ;
}

LRESULT CALLBACK Luna::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	capture->hwndClient = hwnd ;
	capture->hwndScreen = GetDesktopWindow () ;	
	
	switch (uMsg)
	{
	case WM_CREATE:			
		break ;	
	case WM_SIZE:
		{
			RECT winRect, ToolbarWindowRect;			
			GetClientRect (m_hwnd, &winRect) ;			
			GetWindowRect (hWndToolbar, &ToolbarWindowRect) ;	
			
			int width = winRect.right - winRect.left ;	
			int height = 30;//ToolbarWindowRect.bottom -ToolbarWindowRect.top ;

			if (hWndToolbar)
			{	
				SetWindowPos (hWndToolbar, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW) ;	
				UpdateWindow (hWndToolbar) ;
			}

			if (hwndEditWindow)
			{
				SetWindowPos (hwndEditWindow, HWND_TOP, winRect.left, winRect.top + height,
					winRect.right - winRect.left,
					winRect.bottom - winRect.top - height,
					SWP_SHOWWINDOW) ;
				UpdateWindow (hwndEditWindow) ;				
			}			
		}
		break ;
	case WM_MOVE:
		{
			RECT winRect, ToolbarWindowRect;
			GetWindowRect (hwndEditWindow, &winRect) ;
			RedrawWindow (hwnd, NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE ) ; 
			//RedrawWindow (hwndEditWindow, NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN) ;
			
		}
		break ;
	case WM_PAINT:
		OnPaint () ;
		break ;
	case WM_LBUTTONDOWN:		
		OnLButtonDown (wParam, lParam) ;
		return 0 ;	
	case WM_LBUTTONUP:		
		OnLButtonUP (wParam, lParam) ;
		return 0 ;	
	case WM_MOUSEMOVE:
		OnMouseMove (wParam, lParam) ;
		break;
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
		case ID_EDIT:	
			{
				CreateToolbar() ;
				// more display issues in this way: 
				// (1) can not refesh the window when parent window is moving
				// (2) dosen't displayed after created
				// (3) auto refresh issue...			
				// need to investigate.
				//if (!hwndEditWindow && !EditPictureThread)
				//{
				//	EditPictureThread = CreateThread (NULL,0, EditPictureProc, (void*)this, 0, NULL) ;
				//}

				// using this way
				CreateEditChildWindow () ;
			}
			break ;
		case ID_EXIT: 
			exit (0) ;		
		case ID_HELP_ABOUT:
			{	
				HWND hAbout = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_ABOUT_DIALOG), NULL, (DLGPROC)DialogProc) ; 
				ShowWindow(hAbout, SW_SHOW) ;
			}
			break ;
		case ID_SELECT:	
			SendMessage (hwndEditWindow, WM_COMMAND, ID_SELECT, 0) ;
			break ;
		case ID_RECTANGLE:			
			SendMessage (hwndEditWindow, WM_COMMAND, ID_RECTANGLE, 0) ;
			break ;
		case ID_CYCLE:			
			SendMessage (hwndEditWindow, WM_COMMAND, ID_CYCLE, 0) ;
			break ;		
		case ID_LINE:			
			SendMessage (hwndEditWindow, WM_COMMAND, ID_LINE, 0) ;
			break ;
		case ID_TEXT:			
			SendMessage (hwndEditWindow, WM_COMMAND, ID_TEXT, 0) ;
			break ;
		case ID_LINE_SIZE_ONE:			
			SendMessage (hwndEditWindow, WM_COMMAND, ID_LINE_SIZE_ONE, 0) ;
			break ;
		case ID_LINE_SIZE_TWO:				
			SendMessage (hwndEditWindow, WM_COMMAND, ID_LINE_SIZE_TWO, 0) ;
			break ;
		case ID_LINE_SIZE_THREE:			
			SendMessage (hwndEditWindow, WM_COMMAND, ID_LINE_SIZE_THREE, 0) ;
			break ;
		case ID_COLOR_RED:			
			SendMessage (hwndEditWindow, WM_COMMAND, ID_COLOR_RED, 0) ;
			break;
		case ID_COLOR_GREEN:			
			SendMessage (hwndEditWindow, WM_COMMAND, ID_COLOR_GREEN, 0) ;
			break;
		case ID_COLOR_BLUE:
			SendMessage (hwndEditWindow, WM_COMMAND, ID_COLOR_BLUE, 0) ;
			break;
		case ID_CLOSE:	
			{
				SendMessage (hwndEditWindow, WM_DESTROY, 0, 0) ;
				hwndEditWindow = NULL ;	
				CloseHandle (EditPictureThread) ;
				EditPictureThread = NULL ;
				CloseToolbar () ;
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


void Luna::ShowPictureInViewModel ()
{	
	// destroy the edit box if have
	if (hwndEditBox)
		DestroyWindow (hwndEditBox) ;

    BITMAP             bm ;
    RECT               rcClient;
    PAINTSTRUCT        ps;	
    HDC                hdcClient, hdcMem;
    
    GetClientRect (m_hwnd, &rcClient) ;
    hdcClient = BeginPaint(m_hwnd, &ps);
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
    	SetRect (&rBitmapRect, point.x - 1, point.y - 1, point.x + bm.bmWidth + 1, point.y + bm.bmHeight + 1) ;
    	Rectangle(hdcClient, rBitmapRect.left, rBitmapRect.top, rBitmapRect.right, rBitmapRect.bottom) ; 
    	// draw the bitmap
    	BitBlt(hdcClient, point.x, point.y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY) ;
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
    	SetRect (&rBitmapRect, point.x - 1, point.y - 1, point.x + Width + 1, point.y + Height + 1) ;
    	Rectangle(hdcClient, rBitmapRect.left, rBitmapRect.top, rBitmapRect.right, rBitmapRect.bottom) ;
    	// draw the bitmap
    	StretchBlt (hdcClient, point.x, point.y, Width, Height, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY) ;
    }		
          
    DeleteDC (hdcMem) ;		
    EndPaint (m_hwnd, &ps);		
}

HBITMAP Luna::SaveBitmapToMemory ()
{	
   BITMAP bm ;		
   HDC hdc = GetDC (m_hwnd);

   // create compatible DC
   HDC hdcCompat = CreateCompatibleDC (hdc); 
   SelectObject (hdcCompat, hBitmap);	

   // get BITMAP object
   GetObject (hBitmap, sizeof (BITMAP), (PSTR) &bm) ;

   // get new bitmap
   HDC hdcMem = CreateCompatibleDC (hdc) ;
   hBitmap = CreateCompatibleBitmap (hdc, bm.bmWidth, bm.bmHeight) ;
   SelectObject (hdcMem, hBitmap) ;
   StretchBlt (hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdc, 41, 41, bm.bmWidth, bm.bmHeight, SRCCOPY) ;
		
   DeleteDC (hdcMem) ;
   ReleaseDC (m_hwnd, hdc) ;  

   return hBitmap ;
}

void Luna::OnPaint ()
{
	if (hWndToolbar)
	{			
		return ;
	}
	
	if (!hBitmap)
		return ;	
	
	ShowPictureInViewModel () ;	
}	

void Luna::OnLButtonDown (WPARAM wParam, LPARAM lParam)
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

void Luna::OnLButtonUP (WPARAM wParam, LPARAM lParam)
{	
}

void Luna::OnMouseMove (WPARAM wParam, LPARAM lParam)
{		
}

void Luna::OnCaptureAnyArea ()
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

	ShowWindow (m_hwnd, SW_SHOW) ;	
}

void Luna::OnCaptureSpecifiedWindow ()
{	
	capture->bSpecifiedWindow = true ;

	HINSTANCE hInstance = (HINSTANCE)GetWindowLong (m_hwnd, GWL_HINSTANCE) ;
	cpMouseHook = new MouseHook (hInstance) ;		
	cpMouseHook->SetHook ();	
}

int Luna::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
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

int Luna::CreateChildWindow ()
{
	DWORD dwError = 0 ; 	
	EditWindow window (hInstance) ;
	
	WNDCLASSEX wcx;
	wcx.cbSize              = sizeof (WNDCLASSEX) ;							        // size of structure 
    wcx.style               = CS_HREDRAW | CS_VREDRAW ;						        // redraw if size changes 
    wcx.lpfnWndProc         = CBaseWindow::stWinMsgHandler ;				        // points to window procedure 
    wcx.cbClsExtra          = 0 ;											        // no extra class memory 
    wcx.cbWndExtra          = 0 ;											        // no extra window memory 
    wcx.hInstance           = hInstance;									        // handle to instance 
    wcx.hIcon               = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_ICON)) ;	// predefined app. icon 
    wcx.hCursor             = LoadCursor (NULL, IDC_ARROW );					    // predefined arrow 
    wcx.hbrBackground       = (HBRUSH)GetStockObject (WHITE_BRUSH) ;	            // white background brush 
    wcx.lpszMenuName        = MAKEINTRESOURCE (IDM_CAPTURER) ;			            // name of menu resource 
    wcx.lpszClassName       = "EditWindow" ;						                // name of window class 
    wcx.hIconSm             = LoadIcon(hInstance, MAKEINTRESOURCE (IDI_ICON)) ;	    // small class icon 	
	 
	RECT clientRect;
	GetClientRect (m_hwnd, &clientRect) ;
	
	// put edit window under tool bar
	clientRect.top += 30; 

	if (window.RegisterWindow (&wcx))
	{		
		if (hBitmap)
			window.SetBitmap (hBitmap) ;
		//window.bDrawing = true ;

		if (window.Create (WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL, &clientRect, m_hwnd))
		{				
			hwndEditWindow = window.GetHwnd () ;
			window.MsgLoop () ;			
		}
		else
			dwError = GetLastError () ;
	}
	dwError = GetLastError () ;

	// already registered
	if (dwError == 1410)
	{
		if (hBitmap)
			window.SetBitmap (hBitmap) ;

		if (window.Create (WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL, &clientRect, m_hwnd))
		{				
			hwndEditWindow = window.GetHwnd () ;
			window.MsgLoop () ;			
		}
		else
			dwError = GetLastError () ;
	}

	return dwError ;	
}

bool Luna::CreateEditChildWindow ()
{	
	hEditWindowBitmap = hBitmap ;
	RegisterEditPictureChildWindowClass (hInstance);
	hwndEditWindow = CreateEditPictureChildWindow (hInstance, m_hwnd, SW_SHOW);
	
	if (hwndEditWindow)
		return true ;
	else
		return false ;
}

void Luna::CreateToolbar ()
{	
	INITCOMMONCONTROLSEX initctrs;
	initctrs.dwSize = sizeof (INITCOMMONCONTROLSEX);
	initctrs.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&initctrs);

	const int buttonCount = 14 ;

    TBBUTTON button[buttonCount] =
	{
		{0, ID_SELECT, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},		
		{1, ID_RECTANGLE, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},
		{2, ID_CYCLE, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},		
		{3, ID_LINE, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},		
		{4, ID_TEXT, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},

		{5, 50000, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0},
		{6, ID_LINE_SIZE_ONE, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},
		{7, ID_LINE_SIZE_TWO, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},
		{8, ID_LINE_SIZE_THREE, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},

		{9, 50000, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0},
		{10, ID_COLOR_RED, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},		
		{11, ID_COLOR_GREEN, TBSTATE_ENABLED,  BTNS_CHECKGROUP, 0L, 0},
		{12, ID_COLOR_BLUE, TBSTATE_ENABLED,  BTNS_CHECKGROUP, 0L, 0},
		{13, ID_CLOSE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0}
	};

	if (hWndToolbar)
		ShowWindow (hWndToolbar, SW_SHOW) ;
	else
	{ 
		hWndToolbar = CreateToolbarEx (m_hwnd, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DLGFRAME|CCS_TOP,
		IDR_TOOLBAR,
		1,
		hInstance,
		IDR_TOOLBAR,  // IDB_BIT is the Bitmap resource.
		button,
		buttonCount,
		16,16,16,16,
		sizeof (TBBUTTON)
		);	
	}

	//InvalidateRect (m_hwnd, NULL, TRUE) ;
	UpdateWindow (m_hwnd) ;
}

void Luna::UpdateToobar()
{
	TBBUTTONINFO tbInfo; 
    
    tbInfo.cbSize  = sizeof(TBBUTTONINFO);
    tbInfo.dwMask  = TBIF_STATE;
   
	LRESULT s = SendMessage(hWndToolbar, TB_GETBUTTONINFO, 2, (LPARAM)&tbInfo); 
	LRESULT ss = SendMessage(m_hwnd, TB_GETBUTTONINFO, 2, (LPARAM)&tbInfo);

    tbInfo.pszText = "color";
	tbInfo.fsState = TBSTATE_PRESSED ;

    
   LRESULT r = SendMessage (m_hwnd, TB_SETBUTTONINFO, 2, (LPARAM)&tbInfo);
	//UpdateWindow (hWndToolbar) ;
   int y = 9;
}

void Luna::CloseToolbar ()
{
	DestroyWindow (hWndToolbar) ;
	
	if (hWndToolbar)
	{
		hWndToolbar = NULL ;
	}	
	InvalidateRect (m_hwnd, NULL, TRUE) ;
	UpdateWindow (m_hwnd) ;
}

void Luna::SaveFile () 
{
	char szFileName[512] ;
	strcpy (szFileName,"ScreenShot") ;

	char  filesFilter[250] = "";	
	char * pJpgStr     = "JPG Files (*.jpg)|*.jpg|" ;
	char * pBitmapStr  = "Bitmap Files (*.bmp)|*.bmp|" ;
	char * pPngStr     = "PNG Files (*.png)|*.png|" ;	
	char * pGifStr     = "GIF Files (*.gif)|*.gif|" ;
	char * pTiffStr    = "TIFF Files (*.tif)|*.tif|" ;
	strcat (filesFilter, pJpgStr) ;
	strcat (filesFilter, pBitmapStr) ;
	strcat (filesFilter, pPngStr) ;	
	strcat (filesFilter, pGifStr) ;
	strcat (filesFilter, pTiffStr) ;

	char *str = filesFilter ;
	while (*str != '\0') {
        if (*str == '|') {
            *str++ = '\0';			
        }
		*str++;
    }
	// "JPG Files (*.jpg)\0*.jpg\0Bitmap Files (*.bmp)\0*.bmp\0PNG Files (*.png)\0*.png\0GIF Files (*.gif)\0*.gif\0TIFF Files (*.tif)\0*.tif\0" ;

	OPENFILENAME	    ofn ;	
    ZeroMemory (&ofn,sizeof (ofn)) ;
    ofn.lStructSize = sizeof (OPENFILENAME) ;
    ofn.Flags = OFN_HIDEREADONLY|OFN_PATHMUSTEXIST ;
    ofn.lpstrFilter = filesFilter;
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
		MessageBox (NULL, "保存文件失败", "错误", MB_OK) ;
	
	delete bitmap;
	GdiplusShutdown(gdiplusToken);
	return ;
}