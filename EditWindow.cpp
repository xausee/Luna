#include "EditWindow.h"

LPCSTR     szWindowClass = "EditPictureChildWindow" ;
HINSTANCE  hInstance ;
HWND       hEditWindow ;
HBITMAP	   hEditWindowBitmap ;
int        iToolbarHeight = 30 ;
HWND       hwndEditBox ;
bool	   bDrawing ;
bool       fScroll ;
HScroll    hScroll ;
VScroll    vScroll ;	
bool	   bSelection ;
int 	   iShape ;	
bool	   bPenLine ;
int	       iPenSize ;
int  	   iPenColor ;
POINT      pBeg ;
POINT      pEnd ;
RECT       rBitmapRect ;
const int  BITMAP_COUNT = 10 ;
int        iBitmapIndex ;
HBITMAP    hBitmaps[BITMAP_COUNT] ;

bool RegisterEditWindowClass(HINSTANCE hInstance)
{
	DWORD dwError = 0 ; 

	WNDCLASSEX wcx;
	wcx.cbSize              = sizeof (WNDCLASSEX) ;							        // size of structure 
    wcx.style               = CS_HREDRAW | CS_VREDRAW ;						        // redraw if size changes 
    wcx.lpfnWndProc         = EditWindowWndProc ;				                    // points to window procedure 
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

HWND CreateEditWindow (HINSTANCE g_hInstance, HWND hwndParent, int nCmdShow)
{
   HINSTANCE hInstance = g_hInstance; 

   RECT clientRect;
   GetClientRect (hwndParent, &clientRect) ;
   // put edit window under tool bar
   clientRect.top += iToolbarHeight ; 

   DWORD dwError = 0 ; 	
   hBitmaps[0] = hEditWindowBitmap ;

   hEditWindow = CreateWindow(szWindowClass, "TestWindow", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
	   clientRect.left, clientRect.top, clientRect.right-clientRect.left, clientRect.bottom-clientRect.top, hwndParent, NULL, g_hInstance, NULL);
   
   if (!hEditWindow)
   {
	   dwError =GetLastError () ;
	   return FALSE;
   }  

   ShowWindow(hEditWindow, nCmdShow);
   UpdateWindow(hEditWindow);

   return hEditWindow;
}

LRESULT CALLBACK EditWindowWndProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			LPCREATESTRUCT winInfo ;
			winInfo = (LPCREATESTRUCT)lParam ;			
			InitializeScrolls (hwnd, winInfo->cx, winInfo->cy) ;
		}
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
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN: 
		OnKeyDown (uMsg, wParam, lParam) ;
		break ;
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
		default: 
			break ; 
		} 
		return 0 ; 
	case WM_DESTROY:
		DestroyWindow (hEditWindow) ;
		break ;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam) ;
	}
	return 0 ;
}

HWND GetHwnd ()
{
	return hEditWindow ;
}

HBITMAP SetBitmap (HBITMAP	srcHBitmap)
{
	hEditWindowBitmap = srcHBitmap ; 
	return hEditWindowBitmap ;
}

HBITMAP GetBitmap ()
{	
	return hEditWindowBitmap ;
}

int GetLineSize ()
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

COLORREF GetColor ()
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

LRESULT SetEditBox (WPARAM wParam, LPARAM lParam)
{
	//SetTextColor ((HDC)wParam,GetSysColor(COLOR_WINDOWTEXT));
	SetTextColor ((HDC)wParam, GetColor());
	SetBkMode ((HDC)wParam,OPAQUE);
	SetBkColor ((HDC)wParam,GetSysColor(COLOR_WINDOW));
	return (LRESULT)CreateSolidBrush(GetSysColor(COLOR_WINDOW));
}

void ShowPictureInEditModel ()
{	
    BITMAP bm ;		
    HDC hdc = GetDC (hEditWindow);

	// create compatible DC
    HDC hdcCompat = CreateCompatibleDC (hdc); 
    SelectObject (hdcCompat, hEditWindowBitmap);	

	// get BITMAP object
    GetObject (hEditWindowBitmap, sizeof (BITMAP), (PSTR) &bm) ;

    COLORREF crBkgnd = GetBkColor (hdc); 
    HBRUSH   hbrBkgnd = CreateSolidBrush (crBkgnd);	

    ReleaseDC (hEditWindow, hdc); 

	PAINTSTRUCT        ps;  
    RECT               clientRect;
    hdc = BeginPaint(hEditWindow, &ps);

	// Fill the client area with a brush
    GetClientRect (hEditWindow, &clientRect) ;   
    HRGN  bgRgn = CreateRectRgnIndirect(&clientRect);
    HBRUSH  hBrush = CreateSolidBrush(RGB(53, 78, 85));
    FillRgn (hdc, bgRgn, hBrush);    
        
	SetRect (&rBitmapRect, 0, 0, bm.bmWidth, bm.bmHeight) ;
	// draw bolder of the bitmap
    //Rectangle (ps.hdc, rBitmapRect.left, rBitmapRect.top, rBitmapRect.right, rBitmapRect.bottom); 

	// draw bitmap
	BitBlt(ps.hdc, rBitmapRect.left, rBitmapRect.top, bm.bmWidth, bm.bmHeight, hdcCompat, 0, 0, SRCCOPY) ;      

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
    if (bDrawing) 
    { 
		prect = &ps.rcPaint;
		BitBlt(ps.hdc, 
			prect->left, prect->top, 
			(prect->right - prect->left), 
			(prect->bottom - prect->top), 
			hdcCompat, 
			prect->left + hScroll.xCurrentScroll, 
			prect->top + vScroll.yCurrentScroll, 
			SRCCOPY); 
		bDrawing = FALSE; 
	} 

    // clean up
    DeleteDC (hdcCompat) ;	
    DeleteObject(bgRgn);
    DeleteObject(hBrush);	

	EndPaint (hEditWindow, &ps); 	
}

void Shape (HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
{
    HDC hdc = GetDC (hwnd) ;
	int oldRop = SetROP2 (hdc, bModel) ;	
	HPEN hpen = CreatePen (PS_SOLID, GetLineSize(), GetColor()) ;
	HPEN hpenDot = CreatePen (PS_DOT, 1, RGB(0, 0, 0)) ;

	SelectObject (hdc, hpen) ;	
	SelectObject(hdc, GetStockObject(NULL_BRUSH)) ;	

	if (hEditWindowBitmap)
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
		{
			//SelectObject (hdc, hpenDot) ;
			if (bModel == R2_COPYPEN)
			{
				SetROP2(hdc, R2_NOTXORPEN);
				Rectangle (hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;				
			}
			else
			{
				Rectangle (hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;
			}
						
		}
		break ;
	case 5:
		if (bSelection)
		{
			SelectObject (hdc, hpenDot) ;	
			Rectangle (hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;
		}
		break ;
	default:
		break ;
	}	
	
	SetROP2 (hdc, oldRop) ;		
	DeleteObject (hpen) ;
	DeleteObject (hpenDot) ;
	ReleaseDC (hwnd, hdc) ;	
}

HWND CreateEditBox ()
{
	POINT pStart ;
	int weight, height ;

	// destroy the old edit box
	if (hwndEditBox)
		DestroyWindow (hwndEditBox) ;

	if (hEditWindowBitmap)
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

	HDC hdc = GetDC (hEditWindow) ;
	int iFontSize = GetLineSize() * 16 ;
	HFONT hFont = CreateFont(iFontSize, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial") ;
	SetTextColor (hdc, GetColor()) ;

	// Creates textbox for input
	hwndEditBox  = CreateWindowEx(NULL, "EDIT", "",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_LEFT,
			// restrict editbox in the dot rectangle drawed when before
			pStart.x+1, pStart.y+1, weight-2, height-2,	
			hEditWindow, (HMENU)(101),
			hInstance, NULL) ; 	 

	// Set text font
	SendMessage (hwndEditBox, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0)) ;
	// char array to hold the text from the textbox
	char szInput[MAX_PATH];
	// Obtains input from the textbox and puts it into the char array
	GetWindowText(GetDlgItem(hEditWindow, 101), szInput, MAX_PATH);
	if (hwndEditBox)
		return hwndEditBox;
	else
		return NULL;

	DeleteObject (hFont) ;
}

void TextOutFromEditBoxToCanvas ()
{
	if (hwndEditBox)
	{		
		char szInput[MAX_PATH];		
		GetWindowText (GetDlgItem (hEditWindow, 101), szInput, MAX_PATH);
		HDC hdc = GetDC (hEditWindow) ;	
		int iFontSize = GetLineSize () * 16 ;
		HFONT hFont = CreateFont(iFontSize, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial") ;
		SelectObject (hdc, hFont) ;
		SetTextColor (hdc, GetColor()) ;

		// destroy edit window
		DestroyWindow (hwndEditBox) ;
		hwndEditBox = NULL ;
		UpdateWindow (hEditWindow) ;
		// erase the rectangle by draw double rectangle	
		HPEN hpen = CreatePen (PS_SOLID, GetLineSize(), GetColor()) ;
	    HPEN hpenDot = CreatePen (PS_DOT, 1, RGB(0, 0, 0)) ;		
		SelectObject (hdc, hpen) ;

	    int oldRop = SetROP2 (hdc, R2_NOTXORPEN) ;
		Rectangle (hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;
		// draw the rectangle again 			
		Rectangle (hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;
	    SetROP2 (hdc, oldRop) ;

		//Rectangle (hdc, pBeg.x, pBeg.y, pEnd.x, pEnd.y) ;			
		//TextOut (hdc, pBeg.x + 4, pBeg.y + 2, szInput, strlen(szInput)) ;			
		RECT TextRect;
		TextRect.left    = pBeg.x + 4;
		TextRect.top     = pBeg.y + 1; 
		TextRect.right   = pEnd.x;
		TextRect.bottom  = pEnd.y;
		DrawText (hdc, szInput, strlen(szInput), &TextRect, DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK) ;

		// save new bitmap after input text
        hEditWindowBitmap = SaveBitmapToMemory () ;

		// delete objects
		DeleteObject (hpen) ;
		DeleteObject (hpenDot) ;
		DeleteObject (hFont) ;
		ReleaseDC (hEditWindow, hdc) ;	
	}	
}

void DrawRectangle (HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
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

void DrawEllipse (HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
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

void DrawLine (HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
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

void InvalideAndUpdateClient ()
{
	HDC hdc = GetDC (hEditWindow) ;
	RECT clientRect ;
	POINT pLeftTop, pRightBottom  ;

	GetWindowRect (hEditWindow, &clientRect) ;
	pLeftTop.x = clientRect.left ;
	pLeftTop.y = clientRect.top ;
	pRightBottom.x = clientRect.right ;
	pRightBottom.y = clientRect.bottom ;
	
	ScreenToClient (hEditWindow, &pLeftTop) ;
	ScreenToClient (hEditWindow, &pRightBottom) ;

	clientRect.left = pLeftTop.x ;
	clientRect.top = pLeftTop.y ;
	clientRect.right = pRightBottom.x ;
	clientRect.bottom = pRightBottom.y ;

	InvalidateRect (hEditWindow, &clientRect, FALSE) ;
	UpdateWindow (hEditWindow) ;
}

HBITMAP SelectBitmap ()
{	
   // update window for avoid some window on top of it, such as input window
   UpdateWindow (hEditWindow) ;

   BITMAP bm ;		
   HDC hdc = GetDC (hEditWindow);

   // create compatible DC
   HDC hdcCompat = CreateCompatibleDC (hdc); 
   SelectObject (hdcCompat, hEditWindowBitmap);	

   // get BITMAP object
   GetObject (hEditWindowBitmap, sizeof (BITMAP), (PSTR) &bm) ;

   RECT selectRect;
   selectRect.left = pBeg.x ;
   selectRect.top = pBeg.y ;
   selectRect.right = pEnd.x ;
   selectRect.bottom = pEnd.y ; 
   int bmWidth = abs (pEnd.x - pBeg.x) ;
   int bmHeight = abs (pEnd.y - pBeg.y) ;

   // get new bitmap
   HDC hdcMem = CreateCompatibleDC (hdc) ;
   hEditWindowBitmap = CreateCompatibleBitmap (hdc, bmWidth, bmHeight) ;
   SelectObject (hdcMem, hEditWindowBitmap) ;
   StretchBlt (hdcMem, 0, 0, bmWidth, bmHeight, hdc, selectRect.left, selectRect.top, bmWidth, bmHeight, SRCCOPY) ;
		
   DeleteDC (hdcMem) ;
   ReleaseDC (hEditWindow, hdc) ;     

   return hEditWindowBitmap ;
}

HBITMAP SaveBitmapToMemory ()
{	
   // update window for avoid some window on top of it, such as input window
   UpdateWindow (hEditWindow) ;

   BITMAP bm ;		
   HDC hdc = GetDC (hEditWindow);

   // create compatible DC
   HDC hdcCompat = CreateCompatibleDC (hdc); 
   SelectObject (hdcCompat, hEditWindowBitmap);	

   // get BITMAP object
   GetObject (hEditWindowBitmap, sizeof (BITMAP), (PSTR) &bm) ;

   // get new bitmap
   HDC hdcMem = CreateCompatibleDC (hdc) ;
   HBITMAP bitmap ;
   bitmap = CreateCompatibleBitmap (hdc, bm.bmWidth, bm.bmHeight) ;
   SelectObject (hdcMem, bitmap) ;
   StretchBlt (hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY) ;

  /* hEditWindowBitmap = CreateCompatibleBitmap (hdc, bm.bmWidth, bm.bmHeight) ;
   SelectObject (hdcMem, hEditWindowBitmap) ;
   StretchBlt (hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY) ;*/
		
   DeleteDC (hdcMem) ;
   ReleaseDC (hEditWindow, hdc) ;     

   return bitmap ;
   //return hEditWindowBitmap ;
}

void OnPaint ()
{		
	if (!hEditWindowBitmap)
		return ;
	ShowPictureInEditModel () ;
}	

void OnLButtonDown (WPARAM wParam, LPARAM lParam)
{
	TextOutFromEditBoxToCanvas () ;
	
	bDrawing = true ;	
	pBeg.x = LOWORD (lParam) ;
	pBeg.y = HIWORD (lParam) ;
	pEnd.x = LOWORD (lParam) ;
	pEnd.y = HIWORD (lParam) ;		
}

void OnLButtonUP (WPARAM wParam, LPARAM lParam)
{
	if (bDrawing)
	{	
		bDrawing = false ;			
		pEnd.x = LOWORD (lParam) ;
		pEnd.y = HIWORD (lParam) ;
		// drawing shapes: rectangle, Ellipse or line
		Shape (hEditWindow, pBeg, pEnd, R2_COPYPEN) ;
		
		// select part or full bitmap
		if (iShape == 4)
			CreateEditBox() ;
		else
		{
			// save new bitmap after drawing
			iBitmapIndex ++ ;
			if (iBitmapIndex > BITMAP_COUNT)
			{
				iBitmapIndex == BITMAP_COUNT ;
				for (int i = 0; i < BITMAP_COUNT-1; i ++)
				{
					hBitmaps[i] = hBitmaps[i+1] ;
				}				
			}	
			hBitmaps[iBitmapIndex] = SaveBitmapToMemory () ;
			hEditWindowBitmap = hBitmaps[iBitmapIndex] ;
		}

		if (iShape == 5 && bSelection)
		{
			InvalideAndUpdateClient () ;
			hEditWindowBitmap = SelectBitmap () ;
			InvalideAndUpdateClient () ;	
			bSelection = false ;
		}		
	}	
}

void OnMouseMove (WPARAM wParam, LPARAM lParam)
{	
	if (bDrawing)
	{
		Shape (hEditWindow, pBeg, pEnd, R2_NOTXORPEN) ;
		pEnd.x = LOWORD (lParam) ;
		pEnd.y = HIWORD (lParam) ;	
		Shape (hEditWindow, pBeg, pEnd, R2_NOTXORPEN) ;
	}
}

void OnKeyDown (UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	// Ctrl + Z keys
	if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(0x5A))
	{
		if (iBitmapIndex != 0)
			iBitmapIndex -- ;
		hEditWindowBitmap = hBitmaps[iBitmapIndex] ;
	}

	wchar_t msg[32];
    switch (uMsg)
    {
    case WM_SYSKEYDOWN:
        swprintf_s(msg, L"WM_SYSKEYDOWN: 0x%x\n", wParam);
        OutputDebugString(LPCSTR(msg));
        break;

    case WM_SYSCHAR:
        swprintf_s(msg, L"WM_SYSCHAR: %c\n", (wchar_t)wParam);
        OutputDebugString(LPCSTR(msg));
        break;

    case WM_SYSKEYUP:
        swprintf_s(msg, L"WM_SYSKEYUP: 0x%x\n", wParam);
        OutputDebugString(LPCSTR(msg));
        break;

    case WM_KEYDOWN:
        swprintf_s(msg, L"WM_KEYDOWN: 0x%x\n", wParam);
        OutputDebugString(LPCSTR(msg));
        break;

    case WM_KEYUP:
        swprintf_s(msg, L"WM_KEYUP: 0x%x\n", wParam);
        OutputDebugString(LPCSTR(msg));
        break;

    case WM_CHAR:
        swprintf_s(msg, L"WM_CHAR: %c\n", (wchar_t)wParam);
        OutputDebugString(LPCSTR(msg));
        break;   

    }
}

void InitializeScrolls (HWND hwnd, int cx, int cy) 
{
	BITMAP             bm ;    
    GetObject (hEditWindowBitmap, sizeof (BITMAP), (PSTR) &bm) ;	

	// initialize horizonal scroll bar
	int   xNewSize = cx;   
	hScroll.xMaxScroll = max (bm.bmWidth - xNewSize, 0); 
	hScroll.xCurrentScroll = min (hScroll.xCurrentScroll, hScroll.xMaxScroll); 
	
	SCROLLINFO si;
	si.cbSize = sizeof (si) ; 
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS ; 
	si.nMin   = hScroll.xMinScroll ; 
	si.nMax   = bm.bmWidth ; 
	si.nPage  = xNewSize ; 
	si.nPos   = hScroll.xCurrentScroll ; 
	SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ; 


	// initialize verticle scroll bar
	int   yNewSize = cy;
	vScroll.yMaxScroll = max (bm.bmHeight - yNewSize, 0);
	vScroll.yCurrentScroll = min (vScroll.yCurrentScroll, vScroll.yMaxScroll) ;
	
	si.cbSize = sizeof(si) ; 
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS ; 
	si.nMin   = vScroll.yMinScroll ; 
	si.nMax   = bm.bmHeight ; 
	si.nPage  = yNewSize ; 
	si.nPos   = vScroll.yCurrentScroll ; 
	SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
}

void InitializeHScroll (WPARAM wParam, LPARAM lParam)
{	
	BITMAP             bm ;    
    GetObject (hEditWindowBitmap, sizeof (BITMAP), (PSTR) &bm) ;	

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
	SetScrollInfo (hEditWindow, SB_HORZ, &si, TRUE) ; 	
}

void InitializeVScroll (WPARAM wParam, LPARAM lParam)
{	
	BITMAP             bm ;    
    GetObject (hEditWindowBitmap, sizeof (BITMAP), (PSTR) &bm) ;

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
	SetScrollInfo(hEditWindow, SB_VERT, &si, TRUE); 
}

void OnHScroll (WPARAM wParam, LPARAM lParam)
{
	int xDelta;     // xDelta = new_pos - current_pos  
    int xNewPos;    // new position 
    int yDelta = 0; 
    
    switch (LOWORD(wParam)) 
    { 
        // User clicked the scroll bar shaft left of the scroll box. 
        case SB_PAGEUP: 
            xNewPos = hScroll.xCurrentScroll - 50; 
            break; 
    
        // User clicked the scroll bar shaft right of the scroll box. 
        case SB_PAGEDOWN: 
            xNewPos = hScroll.xCurrentScroll + 50; 
            break; 
    
        // User clicked the left arrow. 
        case SB_LINEUP: 
            xNewPos = hScroll.xCurrentScroll - 5; 
            break; 
    
        // User clicked the right arrow. 
        case SB_LINEDOWN: 
            xNewPos = hScroll.xCurrentScroll + 5; 
            break; 
    
        // User dragged the scroll box. 
        case SB_THUMBPOSITION: 
            xNewPos = HIWORD(wParam); 
            break; 
    
        default: 
            xNewPos = hScroll.xCurrentScroll; 
    } 
    
    // New position must be between 0 and the screen width. 
    xNewPos = max(0, xNewPos); 
    xNewPos = min(hScroll.xMaxScroll, xNewPos); 
    
    // If the current position does not change, do not scroll.
    if (xNewPos == hScroll.xCurrentScroll) 
        return ;
    
    // Set the scroll flag to TRUE. 
    bDrawing = true; 
    
    // Determine the amount scrolled (in pixels). 
    xDelta = xNewPos - hScroll.xCurrentScroll; 
    
    // Reset the current scroll position. 
    hScroll.xCurrentScroll = xNewPos; 
    
    // Scroll the window. (The system repaints most of the 
    // client area when ScrollWindowEx is called; however, it is 
    // necessary to call UpdateWindow in order to repaint the 
    // rectangle of pixels that were invalidated.) 
    ScrollWindowEx(hEditWindow, -xDelta, -yDelta, (CONST RECT *) NULL, 
        (CONST RECT *) NULL, (HRGN) NULL, (PRECT) NULL, 
        SW_INVALIDATE); 
    UpdateWindow(hEditWindow); 
    
    // Reset the scroll bar. 
	SCROLLINFO si;
    si.cbSize = sizeof(si); 
    si.fMask  = SIF_POS; 
    si.nPos   = hScroll.xCurrentScroll; 
    SetScrollInfo(hEditWindow, SB_HORZ, &si, TRUE); 
}

void OnVScroll (WPARAM wParam, LPARAM lParam)
{	
    int xDelta = 0; 
    int yDelta;     // yDelta = new_pos - current_pos 
    int yNewPos;    // new position 
    
    switch (LOWORD(wParam)) 
    { 
        // User clicked the scroll bar shaft above the scroll box. 
        case SB_PAGEUP: 
            yNewPos = vScroll.yCurrentScroll - 50; 
            break; 
    
        // User clicked the scroll bar shaft below the scroll box. 
        case SB_PAGEDOWN: 
            yNewPos = vScroll.yCurrentScroll + 50; 
            break; 
    
        // User clicked the top arrow. 
        case SB_LINEUP: 
            yNewPos = vScroll.yCurrentScroll - 5; 
            break; 
    
        // User clicked the bottom arrow. 
        case SB_LINEDOWN: 
            yNewPos = vScroll.yCurrentScroll + 5; 
            break; 
    
        // User dragged the scroll box. 
        case SB_THUMBPOSITION: 
            yNewPos = HIWORD(wParam); 
            break; 
    
        default: 
            yNewPos = vScroll.yCurrentScroll; 
    } 
    
    // New position must be between 0 and the screen height. 
    yNewPos = max(0, yNewPos); 
    yNewPos = min(vScroll.yMaxScroll, yNewPos); 
    
    // If the current position does not change, do not scroll.
    if (yNewPos == vScroll.yCurrentScroll) 
        return ;
    
    // Set the scroll flag to TRUE. 
    bDrawing = true; 
    
    // Determine the amount scrolled (in pixels). 
    yDelta = yNewPos - vScroll.yCurrentScroll; 
    
    // Reset the current scroll position. 
    vScroll.yCurrentScroll = yNewPos; 
    
    // Scroll the window. (The system repaints most of the 
    // client area when ScrollWindowEx is called; however, it is 
    // necessary to call UpdateWindow in order to repaint the 
    // rectangle of pixels that were invalidated.) 
    ScrollWindowEx (hEditWindow, -xDelta, -yDelta, (CONST RECT *) NULL, 
        (CONST RECT *) NULL, (HRGN) NULL, (PRECT) NULL, 
        SW_INVALIDATE); 
    UpdateWindow (hEditWindow); 
    
    // Reset the scroll bar. 
	SCROLLINFO si;
    si.cbSize = sizeof(si); 
    si.fMask  = SIF_POS; 
    si.nPos   = vScroll.yCurrentScroll; 
    SetScrollInfo(hEditWindow, SB_VERT, &si, TRUE);
}