#include "EditPictureWindow.h"

LPCSTR     szWindowClass = "EditPictureChildWindow" ;
HINSTANCE  hInstance ;
HWND       hEditPictureChildWindow ;
HBITMAP	   hEditWindowBitmap ;
int        iToolbarHeight = 30 ;
HWND       hwndEditWindowEditBox ;
bool	   bEditWindowDrawing ;
bool       fEditWindowScroll ;
HScroll    hEditWindowScroll ;
VScroll    vEditWindowScroll ;	
bool	   bEditWindowSelection ;
int 	   iEditWindowShape ;	
bool	   bEditWindowPenLine ;
int	       iEditWindowPenSize ;
int  	   iEditWindowPenColor ;
POINT      pEditWindowBeg ;
POINT      pEditWindowEnd ;
RECT       rEditWindowBitmapRect ;

bool RegisterEditPictureChildWindowClass(HINSTANCE hInstance)
{
	DWORD dwError = 0 ; 

	WNDCLASSEX wcx;
	wcx.cbSize              = sizeof (WNDCLASSEX) ;							        // size of structure 
    wcx.style               = CS_HREDRAW | CS_VREDRAW ;						        // redraw if size changes 
    wcx.lpfnWndProc         = EditPictureChildWindowWndProc ;				        // points to window procedure 
    wcx.cbClsExtra          = 0 ;											        // no extra class memory 
    wcx.cbWndExtra          = 0 ;											        // no extra window memory 
    wcx.hInstance           = hInstance;									        // handle to instance 
    wcx.hIcon               = NULL ;	                                            // predefined app. icon 
    wcx.hCursor             = LoadCursor (NULL, IDC_ARROW );					    // predefined arrow 
    wcx.hbrBackground       = (HBRUSH)GetStockObject (WHITE_BRUSH) ;	            // white background brush 
    wcx.lpszMenuName        = NULL ;			                                    // name of menu resource 
    wcx.lpszClassName       = szWindowClass ;						                // name of window class 
    wcx.hIconSm             = NULL ;	                                            // small class icon 

	RegisterClassEx(&wcx) ;
	dwError = GetLastError () ;

	// already registered
	if (dwError == 1410)
	{
		return false ;
	}
	else
	{
		return true ;
	}

	//return RegisterClassEx(&wcx);  	
}

HWND CreateEditPictureChildWindow(HINSTANCE g_hInstance, HWND hwndParent, int nCmdShow)
{
   HINSTANCE hInstance = g_hInstance; 

   RECT clientRect;
   GetClientRect (hwndParent, &clientRect) ;
   // put edit window under tool bar
   clientRect.top += iToolbarHeight ; 

   DWORD dwError = 0 ; 	


   hEditPictureChildWindow = CreateWindow(szWindowClass, "TestWindow", WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
	   clientRect.left, clientRect.top, clientRect.right-clientRect.left, clientRect.bottom-clientRect.top, hwndParent, NULL, g_hInstance, NULL);
   
   if (!hEditPictureChildWindow)
   {
	   dwError =GetLastError () ;
	   return FALSE;
   }  

   ShowWindow(hEditPictureChildWindow, nCmdShow);
   UpdateWindow(hEditPictureChildWindow);

   return hEditPictureChildWindow;
}

LRESULT CALLBACK EditPictureChildWindowWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			LPCREATESTRUCT winInfo ;
			winInfo = (LPCREATESTRUCT)lParam ;			
			InitializeScrollsEditWindow (hwnd, winInfo->cx, winInfo->cy) ;
		}
		break ;	
	case WM_SIZE: 		
		InitializeHScrollEditWindow (wParam, lParam) ;
		InitializeVScrollEditWindow (wParam, lParam) ;
		break ;
	case WM_PAINT:
		OnPaintEditWindow () ;
		break ;
	case WM_LBUTTONDOWN:		
		OnLButtonDownEditWindow (wParam, lParam) ;
		return 0 ;	
	case WM_LBUTTONUP:		
		OnLButtonUPEditWindow (wParam, lParam) ;
		return 0 ;	
	case WM_MOUSEMOVE:
		OnMouseMoveEditWindow (wParam, lParam) ;
		break;
	case WM_CTLCOLOREDIT:
		return SetEditBoxEditWindow (wParam, lParam) ;
	case WM_HSCROLL:
		OnHScrollEditWindow (wParam, lParam) ;
		break ;
	case WM_VSCROLL:
		OnVScrollEditWindow (wParam, lParam) ;
		break ;
	case WM_COMMAND:
		switch (LOWORD (wParam)) 
        {
		case ID_SELECT:			
			bEditWindowSelection = true ;
			iEditWindowShape = 5 ;
			break ;
		case ID_RECTANGLE:
			iEditWindowShape = 1 ;
			break ;
		case ID_CYCLE:
			iEditWindowShape = 2 ;
			break ;		
		case ID_LINE:	
			iEditWindowShape = 3 ;
			break ;
		case ID_TEXT:
			iEditWindowShape = 4 ;
			break ;
		case ID_LINE_SIZE_ONE:
			iEditWindowPenSize = 1 ;
			break ;
		case ID_LINE_SIZE_TWO:	
			iEditWindowPenSize = 2 ;
			break ;
		case ID_LINE_SIZE_THREE:		
			iEditWindowPenSize = 3 ;
			break ;
		case ID_COLOR_RED:
			iEditWindowPenColor = 1 ;
			break;
		case ID_COLOR_GREEN:
			iEditWindowPenColor = 2 ;
			break;
		case ID_COLOR_BLUE:   			
			iEditWindowPenColor = 3 ;
			break;		
		default: 
			break ; 
		} 
		return 0 ; 
	case WM_DESTROY:
		DestroyWindow (hEditPictureChildWindow) ;
		break ;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam) ;
	}
	return 0 ;
}

HWND GetHwndEditWindow ()
{
	return hEditPictureChildWindow ;
}

HBITMAP SetBitmapEditWindow (HBITMAP	srcHBitmap)
{
	hEditWindowBitmap = srcHBitmap ; 
	return hEditWindowBitmap ;
}

HBITMAP GetBitmapEditWindow ()
{	
	return hEditWindowBitmap ;
}

int GetLineSizeEditWindow ()
{
	int	size  = 1 ;
	switch (iEditWindowPenSize)
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

COLORREF GetColorEditWindow ()
{
	COLORREF  color = RGB (0, 0, 0) ;
	switch (iEditWindowPenColor)
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

LRESULT SetEditBoxEditWindow (WPARAM wParam, LPARAM lParam)
{
	//SetTextColor ((HDC)wParam,GetSysColor(COLOR_WINDOWTEXT));
	SetTextColor ((HDC)wParam, GetColorEditWindow());
	SetBkMode ((HDC)wParam,OPAQUE);
	SetBkColor ((HDC)wParam,GetSysColor(COLOR_WINDOW));
	return (LRESULT)CreateSolidBrush(GetSysColor(COLOR_WINDOW));
}

void ShowPictureInEditModelEditWindow ()
{	
    BITMAP bm ;		
    HDC hdc = GetDC (hEditPictureChildWindow);

	// create compatible DC
    HDC hdcCompat = CreateCompatibleDC (hdc); 
    SelectObject (hdcCompat, hEditWindowBitmap);	

	// get BITMAP object
    GetObject (hEditWindowBitmap, sizeof (BITMAP), (PSTR) &bm) ;

    COLORREF crBkgnd = GetBkColor (hdc); 
    HBRUSH   hbrBkgnd = CreateSolidBrush (crBkgnd);	

    ReleaseDC (hEditPictureChildWindow, hdc); 

	PAINTSTRUCT        ps;  
    RECT               clientRect;
    hdc = BeginPaint(hEditPictureChildWindow, &ps);

	// Fill the client area with a brush
    GetClientRect (hEditPictureChildWindow, &clientRect) ;   
    HRGN  bgRgn = CreateRectRgnIndirect(&clientRect);
    HBRUSH  hBrush = CreateSolidBrush(RGB(53, 78, 85));
    FillRgn (hdc, bgRgn, hBrush);    
        
	SetRect (&rEditWindowBitmapRect, 0, 0, bm.bmWidth, bm.bmHeight) ;
	// draw bolder of the bitmap
    //Rectangle (ps.hdc, rEditWindowBitmapRect.left, rEditWindowBitmapRect.top, rEditWindowBitmapRect.right, rEditWindowBitmapRect.bottom); 

	// draw bitmap
	BitBlt(ps.hdc, rEditWindowBitmapRect.left, rEditWindowBitmapRect.top, bm.bmWidth, bm.bmHeight, hdcCompat, 0, 0, SRCCOPY) ;      

	// If scrolling has occurred, use the following call to
    // BitBlt to paint the invalid rectangle. 
    // 
    // The coordinates of this rectangle are specified in the 
    // RECT structure to which prect points. 
    // 
    // Note that it is necessary to increment the seventh 
    // argument (prect->left) by xCurrentScroll and the 
    // eighth argument (prect->top) by yCurrentScroll in 
    // order to map the correct pixels from the source bitmap. 
	PRECT prect;
    if (bEditWindowDrawing) 
    { 
		prect = &ps.rcPaint;
		BitBlt(ps.hdc, 
			prect->left, prect->top, 
			(prect->right - prect->left), 
			(prect->bottom - prect->top), 
			hdcCompat, 
			prect->left + hEditWindowScroll.xCurrentScroll, 
			prect->top + vEditWindowScroll.yCurrentScroll, 
			SRCCOPY); 
		bEditWindowDrawing = FALSE; 
	} 

    // clean up
    DeleteDC (hdcCompat) ;	
    DeleteObject(bgRgn);
    DeleteObject(hBrush);	

	EndPaint (hEditPictureChildWindow, &ps); 	
}

void ShapeEditWindow (HWND hwnd, POINT pEditWindowBeg, POINT pEditWindowEnd, int bModel)
{
    HDC hdc = GetDC (hwnd) ;
	int oldRop = SetROP2 (hdc, bModel) ;
	HPEN hpen = CreatePen (PS_SOLID, GetLineSizeEditWindow(), GetColorEditWindow()) ;
	HPEN hpenDot = CreatePen (PS_DOT, 1, RGB(0, 0, 0)) ;

	SelectObject (hdc, hpen) ;	
	SelectObject(hdc, GetStockObject(NULL_BRUSH)) ;	

	if (hEditWindowBitmap)
	{
	  // restrict the shape in bitmap rectangle
	  if (pEditWindowBeg.x < rEditWindowBitmapRect.left)
		  pEditWindowBeg.x = rEditWindowBitmapRect.left ;
	  if (pEditWindowBeg.x > rEditWindowBitmapRect.right)
		  pEditWindowBeg.x = rEditWindowBitmapRect.right ;
	  if (pEditWindowBeg.y < rEditWindowBitmapRect.top)
		  pEditWindowBeg.y = rEditWindowBitmapRect.top ;
	  if (pEditWindowBeg.y > rEditWindowBitmapRect.bottom)
		  pEditWindowBeg.y = rEditWindowBitmapRect.bottom ;

	  if (pEditWindowEnd.x < rEditWindowBitmapRect.left)
		  pEditWindowEnd.x = rEditWindowBitmapRect.left ;
	  if (pEditWindowEnd.x > rEditWindowBitmapRect.right)
		  pEditWindowEnd.x = rEditWindowBitmapRect.right ;
	  if (pEditWindowEnd.y < rEditWindowBitmapRect.top)
		  pEditWindowEnd.y = rEditWindowBitmapRect.top ;
	  if (pEditWindowEnd.y > rEditWindowBitmapRect.bottom)
		  pEditWindowEnd.y = rEditWindowBitmapRect.bottom ;
	}

	switch (iEditWindowShape)
	{
	case 1:
		Rectangle (hdc, pEditWindowBeg.x, pEditWindowBeg.y, pEditWindowEnd.x, pEditWindowEnd.y) ;
		break ;
	case 2:
		Ellipse(hdc, pEditWindowBeg.x, pEditWindowBeg.y, pEditWindowEnd.x, pEditWindowEnd.y) ;
		break ;
	case 3:
		MoveToEx (hdc, pEditWindowBeg.x, pEditWindowBeg.y, (LPPOINT) NULL) ; 
		LineTo (hdc, pEditWindowEnd.x, pEditWindowEnd.y) ;
		break ;
	case 4:	
		//use fixed pen when draw text rectangele		
		SelectObject (hdc, hpenDot) ;
		Rectangle (hdc, pEditWindowBeg.x, pEditWindowBeg.y, pEditWindowEnd.x, pEditWindowEnd.y) ;		
		break ;
	case 5:
		SelectObject (hdc, hpenDot) ;	
		Rectangle (hdc, pEditWindowBeg.x, pEditWindowBeg.y, pEditWindowEnd.x, pEditWindowEnd.y) ;
		break ;
	default:
		break ;
	}	
	
	SetROP2 (hdc, oldRop) ;	
	DeleteObject (hpen) ;
	DeleteObject (hpenDot) ;
	ReleaseDC (hwnd, hdc) ;	
}

HWND CreateEditBoxEditWindow ()
{
	POINT pStart ;
	int weight, height ;

	// destroy the old edit box
	if (hwndEditWindowEditBox)
		DestroyWindow (hwndEditWindowEditBox) ;

	if (hEditWindowBitmap)
	{
	  // restrict Edit Box in bitmap rectangle
	  if (pEditWindowBeg.x < rEditWindowBitmapRect.left)
		  pEditWindowBeg.x = rEditWindowBitmapRect.left ;
	  if (pEditWindowBeg.x > rEditWindowBitmapRect.right)
		  pEditWindowBeg.x = rEditWindowBitmapRect.right ;
	  if (pEditWindowBeg.y < rEditWindowBitmapRect.top)
		  pEditWindowBeg.y = rEditWindowBitmapRect.top ;
	  if (pEditWindowBeg.y > rEditWindowBitmapRect.bottom)
		  pEditWindowBeg.y = rEditWindowBitmapRect.bottom ;

	  if (pEditWindowEnd.x < rEditWindowBitmapRect.left)
		  pEditWindowEnd.x = rEditWindowBitmapRect.left ;
	  if (pEditWindowEnd.x > rEditWindowBitmapRect.right)
		  pEditWindowEnd.x = rEditWindowBitmapRect.right ;
	  if (pEditWindowEnd.y < rEditWindowBitmapRect.top)
		  pEditWindowEnd.y = rEditWindowBitmapRect.top ;
	  if (pEditWindowEnd.y > rEditWindowBitmapRect.bottom)
		  pEditWindowEnd.y = rEditWindowBitmapRect.bottom ;
	}

	pStart.x = pEditWindowBeg.x < pEditWindowEnd.x ? pEditWindowBeg.x : pEditWindowEnd.x ;
	pStart.y = pEditWindowBeg.y < pEditWindowEnd.y ? pEditWindowBeg.y : pEditWindowEnd.y ;
	weight = abs (pEditWindowEnd.x - pEditWindowBeg.x) ;
	height = abs (pEditWindowEnd.y - pEditWindowBeg.y) ;	

	HDC hdc = GetDC (hEditPictureChildWindow) ;
	int iFontSize = GetLineSizeEditWindow() * 16 ;
	HFONT hFont = CreateFont(iFontSize, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial") ;
	SetTextColor (hdc, GetColorEditWindow()) ;

	// Creates textbox for input
	hwndEditWindowEditBox  = CreateWindowEx(NULL, "EDIT", "",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_LEFT,
			// restrict editbox in the dot rectangle drawed when before
			pStart.x+1, pStart.y+1, weight-2, height-2,	
			hEditPictureChildWindow, (HMENU)(101),
			hInstance, NULL) ; 	 

	// Set text font
	SendMessage (hwndEditWindowEditBox, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0)) ;
	// char array to hold the text from the textbox
	char szInput[MAX_PATH];
	// Obtains input from the textbox and puts it into the char array
	GetWindowText(GetDlgItem(hEditPictureChildWindow, 101), szInput, MAX_PATH);
	if (hwndEditWindowEditBox)
		return hwndEditWindowEditBox;
	else
		return NULL;

	DeleteObject (hFont) ;
}

void TextOutFromEditBoxToCanvasEditWindow ()
{
	if (hwndEditWindowEditBox)
	{		
		char szInput[MAX_PATH];		
		GetWindowText (GetDlgItem (hEditPictureChildWindow, 101), szInput, MAX_PATH);
		HDC hdc = GetDC (hEditPictureChildWindow) ;	
		int iFontSize = GetLineSizeEditWindow () * 16 ;
		HFONT hFont = CreateFont(iFontSize, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial") ;
		SelectObject (hdc, hFont) ;
		SetTextColor (hdc, GetColorEditWindow()) ;
		bool e = DestroyWindow (hwndEditWindowEditBox) ;
		hwndEditWindowEditBox = NULL ;
		
		RECT rec;
		rec.left = pEditWindowBeg.x;
		rec.top = pEditWindowBeg.y; 
		rec.right = pEditWindowEnd.x;
		rec.bottom = pEditWindowEnd.y;	

		//Rectangle (hdc, pEditWindowBeg.x, pEditWindowBeg.y, pEditWindowEnd.x, pEditWindowEnd.y) ;		
		TextOut (hdc, pEditWindowBeg.x -30, pEditWindowBeg.y, szInput, strlen(szInput)) ;
		//ExtTextOut (hdc, pEditWindowBeg.x -30, pEditWindowBeg.y -30, ETO_CLIPPED, &rec, szInput, strlen(szInput), NULL );
	  	
		// save new bitmap after input text
        hEditWindowBitmap = SaveBitmapToMemoryEditWindow () ;
	}	
}

void DrawRectangleEditWindow (HWND hwnd, POINT pEditWindowBeg, POINT pEditWindowEnd, int bModel)
{
    HDC hdc = GetDC (hwnd) ;
	int oldRop = SetROP2 (hdc, bModel) ;
	HPEN hpen = CreatePen (PS_SOLID, GetLineSizeEditWindow(), GetColorEditWindow()) ;

	SelectObject (hdc, hpen) ;	
	SelectObject(hdc, GetStockObject(NULL_BRUSH)) ;	
	Rectangle (hdc, pEditWindowBeg.x, pEditWindowBeg.y, pEditWindowEnd.x, pEditWindowEnd.y) ;
	SetROP2 (hdc, oldRop) ;	

	DeleteObject (hpen) ;
	ReleaseDC(hwnd, hdc) ;
}

void DrawEllipseEditWindow (HWND hwnd, POINT pEditWindowBeg, POINT pEditWindowEnd, int bModel)
{
	 HDC hdc = GetDC (hwnd) ;
	int oldRop = SetROP2 (hdc, bModel) ;
	HPEN hpen = CreatePen (PS_SOLID, GetLineSizeEditWindow(), GetColorEditWindow()) ;

	SelectObject (hdc, hpen) ;	
	SelectObject(hdc, GetStockObject(NULL_BRUSH)) ;		
	Ellipse(hdc, pEditWindowBeg.x, pEditWindowBeg.y, pEditWindowEnd.x, pEditWindowEnd.y) ;
	SetROP2 (hdc, oldRop) ;	

	DeleteObject (hpen) ;
	ReleaseDC(hwnd, hdc);
}

void DrawLineEditWindow (HWND hwnd, POINT pEditWindowBeg, POINT pEditWindowEnd, int bModel)
{
    HDC hdc = GetDC (hwnd) ;
	int oldRop = SetROP2 (hdc, bModel) ;
	HPEN hpen = CreatePen (PS_SOLID, GetLineSizeEditWindow(), GetColorEditWindow()) ;

	SelectObject (hdc, hpen) ;		
	MoveToEx (hdc, pEditWindowBeg.x, pEditWindowBeg.y, (LPPOINT) NULL) ; 
	LineTo (hdc, pEditWindowEnd.x, pEditWindowEnd.y) ;	
	SetROP2 (hdc, oldRop) ;	

	DeleteObject (hpen) ;
	ReleaseDC(hwnd, hdc) ;
}

HBITMAP SaveBitmapToMemoryEditWindow ()
{	
   // update window for avoid some window on top of it, such as input window
   UpdateWindow (hEditPictureChildWindow) ;

   BITMAP bm ;		
   HDC hdc = GetDC (hEditPictureChildWindow);

   // create compatible DC
   HDC hdcCompat = CreateCompatibleDC (hdc); 
   SelectObject (hdcCompat, hEditWindowBitmap);	

   // get BITMAP object
   GetObject (hEditWindowBitmap, sizeof (BITMAP), (PSTR) &bm) ;

   // get new bitmap
   HDC hdcMem = CreateCompatibleDC (hdc) ;
   hEditWindowBitmap = CreateCompatibleBitmap (hdc, bm.bmWidth, bm.bmHeight) ;
   SelectObject (hdcMem, hEditWindowBitmap) ;
   StretchBlt (hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY) ;
		
   DeleteDC (hdcMem) ;
   ReleaseDC (hEditPictureChildWindow, hdc) ;     

   return hEditWindowBitmap ;
}

void OnPaintEditWindow ()
{		
	if (!hEditWindowBitmap)
		return ;
	ShowPictureInEditModelEditWindow () ;
}	

void OnLButtonDownEditWindow (WPARAM wParam, LPARAM lParam)
{
	TextOutFromEditBoxToCanvasEditWindow () ;
	
	bEditWindowDrawing = true ;	
	pEditWindowBeg.x = LOWORD (lParam) ;
	pEditWindowBeg.y = HIWORD (lParam) ;
	pEditWindowEnd.x = LOWORD (lParam) ;
	pEditWindowEnd.y = HIWORD (lParam) ;		
}

void OnLButtonUPEditWindow (WPARAM wParam, LPARAM lParam)
{
	if (bEditWindowDrawing)
	{	
		bEditWindowDrawing = false ;			
		pEditWindowEnd.x = LOWORD (lParam) ;
		pEditWindowEnd.y = HIWORD (lParam) ;
		// drawing shapes: rectangle, Ellipse or line
		ShapeEditWindow (hEditPictureChildWindow, pEditWindowBeg, pEditWindowEnd, R2_COPYPEN) ;	
		// save new bitmap after drawing
		//hEditWindowBitmap = SaveBitmapToMemoryEditWindow () ;
		// update window immediatly after saved new bitmap
		UpdateWindow (hEditPictureChildWindow) ;
		// select part or full bitmap
		if (iEditWindowShape == 4)
			CreateEditBoxEditWindow() ;
		if (bEditWindowSelection)
			bEditWindowSelection = false ;
	}	
}

void OnMouseMoveEditWindow (WPARAM wParam, LPARAM lParam)
{	
	if (bEditWindowDrawing)
	{
		ShapeEditWindow (hEditPictureChildWindow, pEditWindowBeg, pEditWindowEnd, R2_NOTXORPEN) ;
		pEditWindowEnd.x = LOWORD (lParam) ;
		pEditWindowEnd.y = HIWORD (lParam) ;	
		ShapeEditWindow (hEditPictureChildWindow, pEditWindowBeg, pEditWindowEnd, R2_NOTXORPEN) ;
	}
}

void InitializeScrollsEditWindow (HWND hwnd, int cx, int cy) 
{
	BITMAP             bm ;    
    GetObject (hEditWindowBitmap, sizeof (BITMAP), (PSTR) &bm) ;	

	// initialize horizonal scroll bar
	int   xNewSize = cx;   
	hEditWindowScroll.xMaxScroll = max (bm.bmWidth - xNewSize, 0); 
	hEditWindowScroll.xCurrentScroll = min (hEditWindowScroll.xCurrentScroll, hEditWindowScroll.xMaxScroll); 
	
	SCROLLINFO si;
	si.cbSize = sizeof (si) ; 
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS ; 
	si.nMin   = hEditWindowScroll.xMinScroll ; 
	si.nMax   = bm.bmWidth ; 
	si.nPage  = xNewSize ; 
	si.nPos   = hEditWindowScroll.xCurrentScroll ; 
	SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ; 


	// initialize verticle scroll bar
	int   yNewSize = cy;
	vEditWindowScroll.yMaxScroll = max (bm.bmHeight - yNewSize, 0);
	vEditWindowScroll.yCurrentScroll = min (vEditWindowScroll.yCurrentScroll, vEditWindowScroll.yMaxScroll) ;
	
	si.cbSize = sizeof(si) ; 
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS ; 
	si.nMin   = vEditWindowScroll.yMinScroll ; 
	si.nMax   = bm.bmHeight ; 
	si.nPage  = yNewSize ; 
	si.nPos   = vEditWindowScroll.yCurrentScroll ; 
	SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
}

void InitializeHScrollEditWindow (WPARAM wParam, LPARAM lParam)
{	
	BITMAP             bm ;    
    GetObject (hEditWindowBitmap, sizeof (BITMAP), (PSTR) &bm) ;	

	int   xNewSize = LOWORD (lParam);   
	hEditWindowScroll.xMaxScroll = max (bm.bmWidth - xNewSize, 0); 
	hEditWindowScroll.xCurrentScroll = min (hEditWindowScroll.xCurrentScroll, hEditWindowScroll.xMaxScroll); 
	
	SCROLLINFO si;
	si.cbSize = sizeof (si) ; 
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS ; 
	si.nMin   = hEditWindowScroll.xMinScroll ; 
	si.nMax   = bm.bmWidth ; 
	si.nPage  = xNewSize ; 
	si.nPos   = hEditWindowScroll.xCurrentScroll ; 
	SetScrollInfo (hEditPictureChildWindow, SB_HORZ, &si, TRUE) ; 	
}

void InitializeVScrollEditWindow (WPARAM wParam, LPARAM lParam)
{	
	BITMAP             bm ;    
    GetObject (hEditWindowBitmap, sizeof (BITMAP), (PSTR) &bm) ;

	int   yNewSize = HIWORD (lParam);
	vEditWindowScroll.yMaxScroll = max (bm.bmHeight - yNewSize, 0);
	vEditWindowScroll.yCurrentScroll = min (vEditWindowScroll.yCurrentScroll, vEditWindowScroll.yMaxScroll) ; 

	SCROLLINFO si;
	si.cbSize = sizeof(si) ; 
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS ; 
	si.nMin   = vEditWindowScroll.yMinScroll ; 
	si.nMax   = bm.bmHeight ; 
	si.nPage  = yNewSize ; 
	si.nPos   = vEditWindowScroll.yCurrentScroll ; 
	SetScrollInfo(hEditPictureChildWindow, SB_VERT, &si, TRUE); 
}

void OnHScrollEditWindow (WPARAM wParam, LPARAM lParam)
{
	int xDelta;     // xDelta = new_pos - current_pos  
    int xNewPos;    // new position 
    int yDelta = 0; 
    
    switch (LOWORD(wParam)) 
    { 
        // User clicked the scroll bar shaft left of the scroll box. 
        case SB_PAGEUP: 
            xNewPos = hEditWindowScroll.xCurrentScroll - 50; 
            break; 
    
        // User clicked the scroll bar shaft right of the scroll box. 
        case SB_PAGEDOWN: 
            xNewPos = hEditWindowScroll.xCurrentScroll + 50; 
            break; 
    
        // User clicked the left arrow. 
        case SB_LINEUP: 
            xNewPos = hEditWindowScroll.xCurrentScroll - 5; 
            break; 
    
        // User clicked the right arrow. 
        case SB_LINEDOWN: 
            xNewPos = hEditWindowScroll.xCurrentScroll + 5; 
            break; 
    
        // User dragged the scroll box. 
        case SB_THUMBPOSITION: 
            xNewPos = HIWORD(wParam); 
            break; 
    
        default: 
            xNewPos = hEditWindowScroll.xCurrentScroll; 
    } 
    
    // New position must be between 0 and the screen width. 
    xNewPos = max(0, xNewPos); 
    xNewPos = min(hEditWindowScroll.xMaxScroll, xNewPos); 
    
    // If the current position does not change, do not scroll.
    if (xNewPos == hEditWindowScroll.xCurrentScroll) 
        return ;
    
    // Set the scroll flag to TRUE. 
    bEditWindowDrawing = true; 
    
    // Determine the amount scrolled (in pixels). 
    xDelta = xNewPos - hEditWindowScroll.xCurrentScroll; 
    
    // Reset the current scroll position. 
    hEditWindowScroll.xCurrentScroll = xNewPos; 
    
    // Scroll the window. (The system repaints most of the 
    // client area when ScrollWindowEx is called; however, it is 
    // necessary to call UpdateWindow in order to repaint the 
    // rectangle of pixels that were invalidated.) 
    ScrollWindowEx(hEditPictureChildWindow, -xDelta, -yDelta, (CONST RECT *) NULL, 
        (CONST RECT *) NULL, (HRGN) NULL, (PRECT) NULL, 
        SW_INVALIDATE); 
    UpdateWindow(hEditPictureChildWindow); 
    
    // Reset the scroll bar. 
	SCROLLINFO si;
    si.cbSize = sizeof(si); 
    si.fMask  = SIF_POS; 
    si.nPos   = hEditWindowScroll.xCurrentScroll; 
    SetScrollInfo(hEditPictureChildWindow, SB_HORZ, &si, TRUE); 
}

void OnVScrollEditWindow (WPARAM wParam, LPARAM lParam)
{	
    int xDelta = 0; 
    int yDelta;     // yDelta = new_pos - current_pos 
    int yNewPos;    // new position 
    
    switch (LOWORD(wParam)) 
    { 
        // User clicked the scroll bar shaft above the scroll box. 
        case SB_PAGEUP: 
            yNewPos = vEditWindowScroll.yCurrentScroll - 50; 
            break; 
    
        // User clicked the scroll bar shaft below the scroll box. 
        case SB_PAGEDOWN: 
            yNewPos = vEditWindowScroll.yCurrentScroll + 50; 
            break; 
    
        // User clicked the top arrow. 
        case SB_LINEUP: 
            yNewPos = vEditWindowScroll.yCurrentScroll - 5; 
            break; 
    
        // User clicked the bottom arrow. 
        case SB_LINEDOWN: 
            yNewPos = vEditWindowScroll.yCurrentScroll + 5; 
            break; 
    
        // User dragged the scroll box. 
        case SB_THUMBPOSITION: 
            yNewPos = HIWORD(wParam); 
            break; 
    
        default: 
            yNewPos = vEditWindowScroll.yCurrentScroll; 
    } 
    
    // New position must be between 0 and the screen height. 
    yNewPos = max(0, yNewPos); 
    yNewPos = min(vEditWindowScroll.yMaxScroll, yNewPos); 
    
    // If the current position does not change, do not scroll.
    if (yNewPos == vEditWindowScroll.yCurrentScroll) 
        return ;
    
    // Set the scroll flag to TRUE. 
    bEditWindowDrawing = true; 
    
    // Determine the amount scrolled (in pixels). 
    yDelta = yNewPos - vEditWindowScroll.yCurrentScroll; 
    
    // Reset the current scroll position. 
    vEditWindowScroll.yCurrentScroll = yNewPos; 
    
    // Scroll the window. (The system repaints most of the 
    // client area when ScrollWindowEx is called; however, it is 
    // necessary to call UpdateWindow in order to repaint the 
    // rectangle of pixels that were invalidated.) 
    ScrollWindowEx (hEditPictureChildWindow, -xDelta, -yDelta, (CONST RECT *) NULL, 
        (CONST RECT *) NULL, (HRGN) NULL, (PRECT) NULL, 
        SW_INVALIDATE); 
    UpdateWindow (hEditPictureChildWindow); 
    
    // Reset the scroll bar. 
	SCROLLINFO si;
    si.cbSize = sizeof(si); 
    si.fMask  = SIF_POS; 
    si.nPos   = vEditWindowScroll.yCurrentScroll; 
    SetScrollInfo(hEditPictureChildWindow, SB_VERT, &si, TRUE);
}