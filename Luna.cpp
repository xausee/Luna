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

LRESULT CALLBACK Luna::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			CreateToolbar() ;
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
			bSelection = true ;
			break ;
		case ID_RECTANGLE:
			iShape = 1 ;
			break ;
		case ID_CYCLE:
			iShape = 2 ;
			break ;		
		case ID_LINE:	
			iShape = 3 ;
			break ;
		case ID_TEXT:
			iShape = 4 ;
			break ;
		case ID_LINE_SIZE_ONE:
			iPenSize = 1 ;
			break ;
		case ID_LINE_SIZE_TWO:	
			iPenSize = 2 ;
			break ;
		case ID_LINE_SIZE_THREE:		
			iPenSize = 3 ;
			break ;
		case ID_COLOR_RED:
			iPenColor = 1 ;
			break;
		case ID_COLOR_GREEN:
			iPenColor = 2 ;
			break;
		case ID_COLOR_BLUE:   			
			iPenColor = 3 ;
			break;
		case ID_CLOSE:
			CloseToolbar () ;			
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

int Luna::GetLineSize()
{
	int	size  = 1 ;
	switch (iPenSize)
	{
	case 1:
		size = 1 ;
		break ;
	case 2:
		size = 3 ;
		break ;
	case 3:
		size = 5 ;
		break ;
	default:
		size  = 1 ;
		break ;
	}
	return size ;
}

COLORREF Luna::GetColor()
{
	COLORREF  color = RGB (0, 0, 0) ;
	switch (iPenColor)
	{
	case 1:
		color = RGB (255, 0, 0) ;
		break ;
	case 2:
		color = RGB (0, 255, 0) ;
		break ;
	case 3:
		color = RGB (0, 0, 255) ;
		break ;
	default:
		color = RGB (0, 0, 0) ;
		break ;
	}
	return color ;
}

void Luna::Shape(HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
{
    HDC hdc = GetDC (hwnd) ;
	int oldRop = SetROP2 (hdc, bModel) ;
	HPEN hpen = CreatePen (PS_SOLID, GetLineSize(), GetColor()) ;
	HPEN hpenDot = CreatePen (PS_DOT, 1, RGB(0, 0, 0)) ; 

	SelectObject (hdc, hpen) ;	
	SelectObject(hdc, GetStockObject(NULL_BRUSH)) ;		

	switch (iShape)
	{
	case 1:
		Rectangle (hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;
		break ;
	case 2:
		Ellipse(hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;
		break ;
	case 3:
		MoveToEx (hdc, pBeg.x, pBeg.y, (LPPOINT) NULL) ; 
		LineTo (hdc, pEnd.x, pEnd.y) ;
		break ;
	case 4:		 
		SelectObject (hdc, hpenDot) ;			 
		Rectangle (hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;		
		break ;
	default:
		break ;
	}	
	
	SetROP2 (hdc, oldRop) ;	
	DeleteObject (hpen) ;
	DeleteObject (hpenDot) ;
	ReleaseDC (hwnd, hdc) ;
}

HWND Luna::CreateEditBox()
{
	POINT pStart ;
	int weight, height ;

	pStart.x = pBeg.x < pEnd.x ? pBeg.x : pEnd.x ;
	pStart.y = pBeg.y < pEnd.y ? pBeg.y : pEnd.y ;
	weight = abs (pEnd.x - pBeg.x) ;
	height = abs (pEnd.y - pBeg.y) ;	

	// Creates textbox for input
	HWND hwndEditBox  = CreateWindowEx(NULL, "edit", "",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
			pStart.x, pStart.y, weight, height,	
			m_hwnd, (HMENU)(101),
			hInstance, NULL) ; 
		 
	//MoveWindow(hwnd_ed_1, 400,400,200,200,true);
	// char array to hold the text from the textbox
	char szInput[MAX_PATH];
	// Obtains input from the textbox and puts it into the char array
	GetWindowText(GetDlgItem(m_hwnd, 101), szInput, MAX_PATH);
	if (hwndEditBox)
		return hwndEditBox;
	else
		return NULL;
}

void Luna::DrawRectangle(HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
{
    HDC hdc = GetDC (hwnd) ;
	int oldRop = SetROP2 (hdc, bModel) ;
	HPEN hpen = CreatePen (PS_SOLID, GetLineSize(), GetColor()) ;

	SelectObject (hdc, hpen) ;	
	SelectObject(hdc, GetStockObject(NULL_BRUSH)) ;	
	Rectangle (hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;
	SetROP2 (hdc, oldRop) ;	

	DeleteObject (hpen) ;
	ReleaseDC(hwnd, hdc) ;
}

void Luna::DrawEllipse(HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
{
	 HDC hdc = GetDC (hwnd) ;
	int oldRop = SetROP2 (hdc, bModel) ;
	HPEN hpen = CreatePen (PS_SOLID, GetLineSize(), GetColor()) ;

	SelectObject (hdc, hpen) ;	
	SelectObject(hdc, GetStockObject(NULL_BRUSH)) ;		
	Ellipse(hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;
	SetROP2 (hdc, oldRop) ;	

	DeleteObject (hpen) ;
	ReleaseDC(hwnd, hdc);
}

void Luna::DrawLine(HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
{
    HDC hdc = GetDC (hwnd) ;
	int oldRop = SetROP2 (hdc, bModel) ;
	HPEN hpen = CreatePen (PS_SOLID, GetLineSize(), GetColor()) ;

	SelectObject (hdc, hpen) ;		
	MoveToEx (hdc, pBeg.x, pBeg.y, (LPPOINT) NULL) ; 
	LineTo (hdc, pEnd.x, pEnd.y) ;	
	SetROP2 (hdc, oldRop) ;	

	DeleteObject (hpen) ;
	ReleaseDC(hwnd, hdc) ;
}

void Luna::OnPaint ()
{
	if (hWndToolbar)
	{
		UpdateWindow (m_hwnd) ;
	}
	
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

	if (hWndToolbar)
	{	
		bDrawing = true ;		
		pBeg.x = LOWORD (lParam) ;
		pBeg.y = HIWORD (lParam) ;
		pEnd.x = LOWORD (lParam) ;
		pEnd.y = HIWORD (lParam) ;		
	}	
}

void Luna::OnLButtonUP (WPARAM wParam, LPARAM lParam)
{
	if (bDrawing)
	{	
		bDrawing = false ;			
		pEnd.x = LOWORD (lParam) ;
		pEnd.y = HIWORD (lParam) ;
		Shape (m_hwnd, pBeg, pEnd, R2_COPYPEN) ;	
		if (iShape ==4)
			CreateEditBox() ;
	}	
}

void Luna::OnMouseMove (WPARAM wParam, LPARAM lParam)
{	
	if (bDrawing)
	{
		Shape (m_hwnd, pBeg, pEnd, R2_NOTXORPEN) ;
		pEnd.x = LOWORD (lParam) ;
		pEnd.y = HIWORD (lParam) ;	
		Shape (m_hwnd, pBeg, pEnd, R2_NOTXORPEN) ;
	}
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

	ShowWindow (m_hwnd,SW_SHOW) ;	
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

void Luna::CreateToolbar ()
{
	INITCOMMONCONTROLSEX initctrs;
	initctrs.dwSize = sizeof (INITCOMMONCONTROLSEX);
	initctrs.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&initctrs);

	const int buttonCount = 15 ;

    TBBUTTON button[buttonCount] =
	{
		{0, ID_SELECT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},

		{1, 50000, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0},
		{2, ID_RECTANGLE, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},
		{3, ID_CYCLE, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},		
		{4, ID_LINE, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},		
		{5, ID_TEXT, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},

		{6, 50000, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0},
		{7, ID_LINE_SIZE_ONE, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},
		{8, ID_LINE_SIZE_TWO, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},
		{9, ID_LINE_SIZE_THREE, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},

		{10, 50000, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0},
		{11, ID_COLOR_RED, TBSTATE_ENABLED, BTNS_CHECKGROUP, 0L, 0},		
		{12, ID_COLOR_GREEN, TBSTATE_ENABLED,  BTNS_CHECKGROUP, 0L, 0},
		{13, ID_COLOR_BLUE, TBSTATE_ENABLED,  BTNS_CHECKGROUP, 0L, 0},
		{14, ID_CLOSE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0}
	};

	if (hWndToolbar)
		ShowWindow (hWndToolbar, SW_SHOW) ;
	else
	{ 
		hWndToolbar = CreateToolbarEx (m_hwnd, WS_CHILD | WS_VISIBLE | WS_BORDER, IDR_TOOLBAR,
		1,
		hInstance,
		IDR_TOOLBAR,  // IDB_BIT is the Bitmap resource.
		button,
		buttonCount,
		16,16,16,16,
		sizeof (TBBUTTON)
		);
	}

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
	//CloseHandle (hWndToolbar) ;
	if (hWndToolbar)
	{
		hWndToolbar = NULL ;
	}	
	UpdateWindow (m_hwnd) ;
}

void Luna::SaveFile () 
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