#include "EditWindow.h"

LRESULT CALLBACK EditWindow::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			LPCREATESTRUCT winInfo ;
			winInfo = (LPCREATESTRUCT)lParam ;
			//SendMessage (hwnd, WM_SIZE, wParam,  winInfo->cx ) ;
			/*InitializeHScroll (wParam, winInfo->cx) ;
			InitializeVScroll (wParam, winInfo->cy) ;*/
			InitializeScroll (hwnd, winInfo->cx, winInfo->cy) ;
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
		bWindowClosed = TRUE;
		break ;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam) ;
	}
	return 0 ;
}

HWND EditWindow::GetHwnd ()
{
	return m_hwnd ;
}

HBITMAP EditWindow::SetBitmap (HBITMAP	srcHBitmap)
{
	hBitmap = srcHBitmap ; 
	return hBitmap ;
}

HBITMAP EditWindow::GetBitmap ()
{	
	return hBitmap ;
}

int EditWindow::GetLineSize()
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

COLORREF EditWindow::GetColor()
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

LRESULT EditWindow::SetEditBox(WPARAM wParam, LPARAM lParam)
{
	//SetTextColor ((HDC)wParam,GetSysColor(COLOR_WINDOWTEXT));
	SetTextColor ((HDC)wParam, GetColor());
	SetBkMode ((HDC)wParam,OPAQUE);
	SetBkColor ((HDC)wParam,GetSysColor(COLOR_WINDOW));
	return (LRESULT)CreateSolidBrush(GetSysColor(COLOR_WINDOW));
}

void EditWindow::ShowPictureInEditModel ()
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
    HBRUSH  hBrush = CreateSolidBrush(RGB(53, 78, 85));
    FillRgn (hdc, bgRgn, hBrush);    
    
    // draw bolder of the bitmap
	SetRect (&rBitmapRect, 0, 0, bm.bmWidth + 2, bm.bmHeight + 2) ;
    Rectangle (ps.hdc, rBitmapRect.left, rBitmapRect.top, rBitmapRect.right, rBitmapRect.bottom); 

	// draw bitmap
	BitBlt(ps.hdc, rBitmapRect.left + 1, rBitmapRect.top + 1, bm.bmWidth, bm.bmHeight, hdcCompat, 0, 0, SRCCOPY) ;      

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
    if (fScroll) 
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
		fScroll = FALSE; 
	} 

    // clean up
    DeleteDC (hdcCompat) ;	
    DeleteObject(bgRgn);
    DeleteObject(hBrush);	

	EndPaint (m_hwnd, &ps); 	
}

void EditWindow::Shape(HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
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

HWND EditWindow::CreateEditBox()
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

void EditWindow::TextOutFromEditBoxToCanvas()
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

void EditWindow::DrawRectangle(HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
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

void EditWindow::DrawEllipse(HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
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

void EditWindow::DrawLine(HWND hwnd, POINT pBeg, POINT pEnd, int bModel)
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

HBITMAP EditWindow::SaveBitmapToMemory ()
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

void EditWindow::OnPaint ()
{		
	if (!hBitmap)
		return ;
	ShowPictureInEditModel () ;
	
	//TextOutFromEditBoxToCanvas () ;
}	

void EditWindow::OnLButtonDown (WPARAM wParam, LPARAM lParam)
{
	//TextOutFromEditBoxToCanvas () ;
	bDrawing = true ;	
	pBeg.x = LOWORD (lParam) ;
	pBeg.y = HIWORD (lParam) ;
	pEnd.x = LOWORD (lParam) ;
	pEnd.y = HIWORD (lParam) ;		
}

void EditWindow::OnLButtonUP (WPARAM wParam, LPARAM lParam)
{
	if (bDrawing)
	{	
		bDrawing = false ;			
		pEnd.x = LOWORD (lParam) ;
		pEnd.y = HIWORD (lParam) ;
		// drawing shapes: rectangle, Ellipse or line
		Shape (m_hwnd, pBeg, pEnd, R2_COPYPEN) ;	
		// save new bitmap after drawing
		//hBitmap = SaveBitmapToMemory () ;
		// select part or full bitmap
		if (iShape == 4)
			CreateEditBox() ;
		if (bSelection)
			bSelection = false ;
	}	
}

void EditWindow::OnMouseMove (WPARAM wParam, LPARAM lParam)
{	
	if (bDrawing)
	{
		Shape (m_hwnd, pBeg, pEnd, R2_NOTXORPEN) ;
		pEnd.x = LOWORD (lParam) ;
		pEnd.y = HIWORD (lParam) ;	
		Shape (m_hwnd, pBeg, pEnd, R2_NOTXORPEN) ;
	}
}

void EditWindow::InitializeScrolls (HWND hwnd, int cx, int cy) 
{
	BITMAP             bm ;    
    GetObject (hBitmap, sizeof (BITMAP), (PSTR) &bm) ;	

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

void EditWindow::InitializeHScroll (WPARAM wParam, LPARAM lParam)
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

void EditWindow::InitializeVScroll (WPARAM wParam, LPARAM lParam)
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

void EditWindow::OnHScroll (WPARAM wParam, LPARAM lParam)
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
    fScroll = true; 
    
    // Determine the amount scrolled (in pixels). 
    xDelta = xNewPos - hScroll.xCurrentScroll; 
    
    // Reset the current scroll position. 
    hScroll.xCurrentScroll = xNewPos; 
    
    // Scroll the window. (The system repaints most of the 
    // client area when ScrollWindowEx is called; however, it is 
    // necessary to call UpdateWindow in order to repaint the 
    // rectangle of pixels that were invalidated.) 
    ScrollWindowEx(m_hwnd, -xDelta, -yDelta, (CONST RECT *) NULL, 
        (CONST RECT *) NULL, (HRGN) NULL, (PRECT) NULL, 
        SW_INVALIDATE); 
    UpdateWindow(m_hwnd); 
    
    // Reset the scroll bar. 
	SCROLLINFO si;
    si.cbSize = sizeof(si); 
    si.fMask  = SIF_POS; 
    si.nPos   = hScroll.xCurrentScroll; 
    SetScrollInfo(m_hwnd, SB_HORZ, &si, TRUE); 
}

void EditWindow::OnVScroll (WPARAM wParam, LPARAM lParam)
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
    fScroll = true; 
    
    // Determine the amount scrolled (in pixels). 
    yDelta = yNewPos - vScroll.yCurrentScroll; 
    
    // Reset the current scroll position. 
    vScroll.yCurrentScroll = yNewPos; 
    
    // Scroll the window. (The system repaints most of the 
    // client area when ScrollWindowEx is called; however, it is 
    // necessary to call UpdateWindow in order to repaint the 
    // rectangle of pixels that were invalidated.) 
    ScrollWindowEx(m_hwnd, -xDelta, -yDelta, (CONST RECT *) NULL, 
        (CONST RECT *) NULL, (HRGN) NULL, (PRECT) NULL, 
        SW_INVALIDATE); 
    UpdateWindow(m_hwnd); 
    
    // Reset the scroll bar. 
	SCROLLINFO si;
    si.cbSize = sizeof(si); 
    si.fMask  = SIF_POS; 
    si.nPos   = vScroll.yCurrentScroll; 
    SetScrollInfo(m_hwnd, SB_VERT, &si, TRUE);
}