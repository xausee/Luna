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
	case WM_SIZE: 
		InitializeHScroll (wParam, lParam) ;
		InitializeVScroll (wParam, lParam) ;
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
	case WM_CTLCOLOREDIT:
		return SetEditBox (wParam, lParam) ;
	case WM_HSCROLL:
		OnHScroll (wParam, lParam) ;
		break ;
	case WM_VSCROLL:
		OnVScroll (wParam, lParam) ;
		break ;
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
			InitializeHScroll (wParam, lParam) ;
			InitializeVScroll (wParam, lParam) ;
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
			iShape = 5 ;
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

LRESULT Luna::SetEditBox(WPARAM wParam, LPARAM lParam)
{
	//SetTextColor ((HDC)wParam,GetSysColor(COLOR_WINDOWTEXT));
	SetTextColor ((HDC)wParam, GetColor());
	SetBkMode ((HDC)wParam,OPAQUE);
	SetBkColor ((HDC)wParam,GetSysColor(COLOR_WINDOW));
	return (LRESULT)CreateSolidBrush(GetSysColor(COLOR_WINDOW));
}

void Luna::ShowPictureInEditModel ()
{	
    BITMAP bm ;		
    HDC hdc = GetDC (m_hwnd);

	// create compatible DC
    HDC hdcCompat = CreateCompatibleDC (hdc); 
    SelectObject (hdcCompat, hBitmap);	

	// get BITMAP object
    GetObject (hBitmap, sizeof (BITMAP), (PSTR) &bm) ;

    COLORREF crBkgnd = GetBkColor (hdc); 
    HBRUSH   hbrBkgnd = CreateSolidBrush (crBkgnd);	

    ReleaseDC (m_hwnd, hdc); 

	PAINTSTRUCT        ps;  
    RECT               clientRect;
    hdc = BeginPaint(m_hwnd, &ps);

	// Fill the client area with a brush
    GetClientRect (m_hwnd, &clientRect) ;   
    HRGN  bgRgn = CreateRectRgnIndirect(&clientRect);
    HBRUSH  hBrush = CreateSolidBrush(RGB(200,200,200));
    FillRgn (hdc, bgRgn, hBrush);    
    
    // draw bolder of the bitmap
	SetRect (&rBitmapRect, 40, 40, bm.bmWidth + 42, bm.bmHeight + 42) ;
    Rectangle (ps.hdc, rBitmapRect.left, rBitmapRect.top, rBitmapRect.right, rBitmapRect.bottom); 

	// draw bitmap
	BitBlt(ps.hdc, rBitmapRect.left + 1, rBitmapRect.top + 1, bm.bmWidth, bm.bmHeight, hdcCompat, 0, 0, SRCCOPY) ;  
    
    // clean up
    DeleteDC (hdcCompat) ;	
    DeleteObject(bgRgn);
    DeleteObject(hBrush);	

	EndPaint (m_hwnd, &ps); 	
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

void Luna::Shape(HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
{
    HDC hdc = GetDC (hwnd) ;
	int oldRop = SetROP2 (hdc, bModel) ;
	HPEN hpen = CreatePen (PS_SOLID, GetLineSize(), GetColor()) ;
	HPEN hpenDot = CreatePen (PS_DOT, 1, RGB(0, 0, 0)) ;

	SelectObject (hdc, hpen) ;	
	SelectObject(hdc, GetStockObject(NULL_BRUSH)) ;	

	if (hBitmap)
	{
	  // restrict the shape in bitmap rectangle
	  if (pBeg.x < rBitmapRect.left)
		  pBeg.x = rBitmapRect.left ;
	  if (pBeg.x > rBitmapRect.right)
		  pBeg.x = rBitmapRect.right ;
	  if (pBeg.y < rBitmapRect.top)
		  pBeg.y = rBitmapRect.top ;
	  if (pBeg.y > rBitmapRect.bottom)
		  pBeg.y = rBitmapRect.bottom ;

	  if (pEnd.x < rBitmapRect.left)
		  pEnd.x = rBitmapRect.left ;
	  if (pEnd.x > rBitmapRect.right)
		  pEnd.x = rBitmapRect.right ;
	  if (pEnd.y < rBitmapRect.top)
		  pEnd.y = rBitmapRect.top ;
	  if (pEnd.y > rBitmapRect.bottom)
		  pEnd.y = rBitmapRect.bottom ;
	}

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
		//use fixed pen when draw text rectangele
		SelectObject (hdc, CreatePen(PS_SOLID, 1,  RGB (0, 0, 0))) ; 
		Rectangle (hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;		
		break ;
	case 5:
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

	// destroy the old edit box
	if (hwndEditBox)
		DestroyWindow (hwndEditBox) ;

	if (hBitmap)
	{
	  // restrict Edit Box in bitmap rectangle
	  if (pBeg.x < rBitmapRect.left)
		  pBeg.x = rBitmapRect.left ;
	  if (pBeg.x > rBitmapRect.right)
		  pBeg.x = rBitmapRect.right ;
	  if (pBeg.y < rBitmapRect.top)
		  pBeg.y = rBitmapRect.top ;
	  if (pBeg.y > rBitmapRect.bottom)
		  pBeg.y = rBitmapRect.bottom ;

	  if (pEnd.x < rBitmapRect.left)
		  pEnd.x = rBitmapRect.left ;
	  if (pEnd.x > rBitmapRect.right)
		  pEnd.x = rBitmapRect.right ;
	  if (pEnd.y < rBitmapRect.top)
		  pEnd.y = rBitmapRect.top ;
	  if (pEnd.y > rBitmapRect.bottom)
		  pEnd.y = rBitmapRect.bottom ;
	}

	pStart.x = pBeg.x < pEnd.x ? pBeg.x : pEnd.x ;
	pStart.y = pBeg.y < pEnd.y ? pBeg.y : pEnd.y ;
	weight = abs (pEnd.x - pBeg.x) ;
	height = abs (pEnd.y - pBeg.y) ;	

	HDC hdc = GetDC (m_hwnd) ;
	int iFontSize = GetLineSize() * 16 ;
	HFONT hFont = CreateFont(iFontSize, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial") ;
	SetTextColor (hdc, GetColor()) ;

	// Creates textbox for input
	hwndEditBox  = CreateWindowEx(NULL, "EDIT", "",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | WS_BORDER | ES_LEFT,
			pStart.x, pStart.y, weight, height,	
			m_hwnd, (HMENU)(101),
			hInstance, NULL) ; 	 

	// Set text font
	SendMessage (hwndEditBox, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0)) ;
	// char array to hold the text from the textbox
	char szInput[MAX_PATH];
	// Obtains input from the textbox and puts it into the char array
	GetWindowText(GetDlgItem(m_hwnd, 101), szInput, MAX_PATH);
	if (hwndEditBox)
		return hwndEditBox;
	else
		return NULL;

	DeleteObject (hFont) ;
}

void Luna::TextOutFromEditBoxToCanvas()
{
	if (hwndEditBox)
	{		
		char szInput[MAX_PATH];		
		GetWindowText(GetDlgItem(m_hwnd, 101), szInput, MAX_PATH);
		HDC hdc = GetDC (m_hwnd) ;	
		int iFontSize = GetLineSize() * 16 ;
		HFONT hFont = CreateFont(iFontSize, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial") ;
		SelectObject (hdc, hFont) ;
		SetTextColor (hdc, GetColor()) ;
		DestroyWindow (hwndEditBox) ;
		hwndEditBox = NULL ;
		Rectangle (hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;		
		TextOut (hdc, pBeg.x + 2, pBeg.y + 2, szInput, strlen(szInput)) ;			
	}	
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
		UpdateWindow (m_hwnd) ;
	}
	
	if (!hBitmap)
		return ;

	if (isEdit)	
		ShowPictureInEditModel () ;
	else
		ShowPictureInViewModel () ;

	//TextOutFromEditBoxToCanvas () ;
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
	
	//TextOutFromEditBoxToCanvas () ;

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
		// drawing shapes: rectangle, Ellipse or line
		Shape (m_hwnd, pBeg, pEnd, R2_COPYPEN) ;	
		// save new bitmap after drawing
		hBitmap = SaveBitmapToMemory () ;
		// select part or full bitmap
		if (iShape == 4)
			CreateEditBox() ;
		if (bSelection)
			bSelection = false ;
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

void Luna::InitializeHScroll (WPARAM wParam, LPARAM lParam)
{	
	BITMAP             bm ;    
    GetObject (hBitmap, sizeof (BITMAP), (PSTR) &bm) ;	

	int   xNewSize = LOWORD (lParam);   
	hScroll.xMaxScroll = max (bm.bmWidth - xNewSize, 0); 
	hScroll.xCurrentScroll = min (hScroll.xCurrentScroll, hScroll.xMaxScroll); 
	
	SCROLLINFO si;
	si.cbSize = sizeof (si) ; 
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS ; 
	si.nMin   = hScroll.xMinScroll ; 
	si.nMax   = bm.bmWidth ; 
	si.nPage  = xNewSize ; 
	si.nPos   = hScroll.xCurrentScroll ; 
	SetScrollInfo (m_hwnd, SB_HORZ, &si, TRUE) ; 	
}

void Luna::InitializeVScroll (WPARAM wParam, LPARAM lParam)
{	
	BITMAP             bm ;    
    GetObject (hBitmap, sizeof (BITMAP), (PSTR) &bm) ;

	int   yNewSize = HIWORD (lParam);
	vScroll.yMaxScroll = max (bm.bmHeight - yNewSize, 0);
	vScroll.yCurrentScroll = min (vScroll.yCurrentScroll, vScroll.yMaxScroll) ; 

	SCROLLINFO si;
	si.cbSize = sizeof(si) ; 
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS ; 
	si.nMin   = vScroll.yMinScroll ; 
	si.nMax   = bm.bmHeight ; 
	si.nPage  = yNewSize ; 
	si.nPos   = vScroll.yCurrentScroll ; 
	SetScrollInfo(m_hwnd, SB_VERT, &si, TRUE); 
}

void Luna::OnHScroll (WPARAM wParam, LPARAM lParam)
{		
}

void Luna::OnVScroll (WPARAM wParam, LPARAM lParam)
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
	isEdit = true;
	// add horizontal and verticle scroll bar from window style
	LONG_PTR style = GetWindowLongPtr (m_hwnd, GWL_STYLE) ;
	SetWindowLongPtr (m_hwnd, GWL_STYLE, style | WS_HSCROLL | WS_VSCROLL) ;

	RECT winRect;
	GetWindowRect (m_hwnd, &winRect) ;

	// Show the new style window immediately 	
	// seems if width and height are the same as old, SetWindowPos not work
	// so just expend width and height by 1 pixel, then change it back
	SetWindowPos (m_hwnd, HWND_TOP, winRect.left, winRect.top,
		          winRect.right - winRect.left + 1, 
		          winRect.bottom - winRect.top + 1, 
				  SWP_SHOWWINDOW/*SWP_NOMOVE*/) ;
	// Change back
	SetWindowPos (m_hwnd, HWND_TOP, winRect.left, winRect.top,
		          winRect.right - winRect.left, 
		          winRect.bottom - winRect.top, 
				  SWP_SHOWWINDOW/*SWP_NOMOVE*/) ;	

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

	InvalidateRect (m_hwnd, NULL, TRUE) ;
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
	isEdit = false;
	DestroyWindow (hWndToolbar) ;	

	// remove horizontal and verticle scroll bar from window style
	LONG_PTR style = GetWindowLongPtr (m_hwnd, GWL_STYLE) ;
	style &= ~(WS_HSCROLL | WS_VSCROLL);
	SetWindowLongPtr (m_hwnd, GWL_STYLE, style) ;

	RECT winRect;
	GetWindowRect (m_hwnd, &winRect) ;

	// Show the new style window immediately 	
	// seems if width and height are the same as old, SetWindowPos not work
	// so just expend width and height by 1 pixel, then change it back
	SetWindowPos (m_hwnd, HWND_TOP, winRect.left, winRect.top,
		          winRect.right - winRect.left + 1, 
		          winRect.bottom - winRect.top + 1, 
				  SWP_SHOWWINDOW/*SWP_NOMOVE*/) ;
	// Change back
	SetWindowPos (m_hwnd, HWND_TOP, winRect.left, winRect.top,
		          winRect.right - winRect.left, 
		          winRect.bottom - winRect.top, 
				  SWP_SHOWWINDOW/*SWP_NOMOVE*/) ;
	
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
		MessageBox (NULL, "±£´æÎÄ¼þÊ§°Ü", "´íÎó", MB_OK) ;
	
	delete bitmap;
	GdiplusShutdown(gdiplusToken);
	return ;
}