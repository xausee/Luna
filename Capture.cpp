#include "Capture.h"

Capture::Capture(void)
{
	bCapturing = false ;
	bBlocking  = false ;
	bSpecifiedWindow = false ;	
	hBitmap    = NULL ;
	ptBeg.x    = 0;
	ptBeg.y    = 0;
	ptEnd.x    = 0;
	ptEnd.y    = 0;
}

Capture::~Capture(void)
{
}

HBITMAP Capture::CaptureFullScreen ()
{		
	ShowWindow (hwndClient, SW_HIDE) ;
	/*
	TODO: wait for Luna disapear gracefully
	*/
	Sleep (100) ;
    SetCursor (LoadCursor (NULL,IDC_WAIT)) ;	
    int nWidth = GetSystemMetrics (SM_CXSCREEN) ;
    int nHeight = GetSystemMetrics (SM_CYSCREEN) ;

	HWND hDesktopWnd = hwndScreen ;
    HDC hDesktopDC = GetDC (hDesktopWnd) ;    
	HDC hBmpFileDC = CreateCompatibleDC (hDesktopDC) ;

    hBitmap = CreateCompatibleBitmap (hDesktopDC, nWidth, nHeight) ;
    HBITMAP hOldBitmap = (HBITMAP) SelectObject (hBmpFileDC, hBitmap) ;
    BitBlt (hBmpFileDC, 0, 0, nWidth, nHeight, hDesktopDC, 0, 0, SRCCOPY|CAPTUREBLT) ;
    SelectObject (hBmpFileDC,hOldBitmap) ; 
	 
	DeleteDC (hDesktopDC) ;
	DeleteObject (hDesktopWnd) ;
	DeleteObject (hOldBitmap) ;
	DeleteDC (hBmpFileDC) ;
	
    SetCursor (LoadCursor(NULL,IDC_ARROW)) ;
	ShowWindow (hwndClient,SW_SHOW) ;	
		
	return hBitmap ;
}

void Capture::InvertBlock ()
{		
	HDC hdc = GetDCEx (hwndScreen, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE) ;
	POINT ptScreenBeg, ptScreenEnd ;
	ptScreenBeg = ptBeg ;
	ptScreenEnd = ptEnd ;
	ClientToScreen (hwndClient, &ptScreenBeg) ;
	ClientToScreen (hwndClient, &ptScreenEnd) ;	
	PatBlt (hdc, ptScreenBeg.x, ptScreenBeg.y, ptScreenEnd.x - ptScreenBeg.x, ptScreenEnd.y - ptScreenBeg.y, DSTINVERT) ;
    ReleaseDC (hwndScreen, hdc) ;
}

void Capture::InitCaptureAnyArea()
{	
	if (!bCapturing)    
	{		
		if (LockWindowUpdate (hwndScreen))
		{
			bCapturing = true ;
			//move to window creating
			//SetCapture (hwndClient) ;
			//SetCursor (LoadCursor (NULL, IDC_CROSS)) ;
		}
		else
			MessageBeep (0) ;
	}	
}

void Capture::StartCaptureAnyArea (POINT pBeg)
{
	if (bCapturing)
	{
		bBlocking = true ;
		ptBeg.x = pBeg.x ;
		ptBeg.y = pBeg.y ;
		ptEnd = ptBeg ;
		InvertBlock () ;		
	}  
}

void Capture::MarkCaptureArea (POINT pEnd) 
{
	if (bBlocking)
	{
		InvertBlock () ;		
		ptEnd.x = pEnd.x ;
		ptEnd.y = pEnd.y ;
		InvertBlock () ;				
	}   
}

HBITMAP Capture::EndCaptureAnyArea (POINT pEnd) 
{
	if (bBlocking)
	{
		InvertBlock () ;		
		ptEnd.x = pEnd.x ;
		ptEnd.y = pEnd.y ;
		
		if (hBitmap)
		{
			DeleteObject (hBitmap) ;
			hBitmap = NULL ;
		}
		
		HDC hdc = GetDC (hwndClient) ;
		HDC hdcMem = CreateCompatibleDC (hdc) ;
		hBitmap = CreateCompatibleBitmap (hdc, abs (ptEnd.x - ptBeg.x), abs (ptEnd.y - ptBeg.y)) ;
		SelectObject (hdcMem, hBitmap) ;
		StretchBlt (hdcMem, 0, 0, abs (ptEnd.x - ptBeg.x), abs (ptEnd.y - ptBeg.y), hdc, ptBeg.x, ptBeg.y, ptEnd.x - ptBeg.x, ptEnd.y - ptBeg.y, SRCCOPY) ;
		
		DeleteDC (hdcMem) ;
		ReleaseDC (hwndClient, hdc) ;
		InvalidateRect (hwndClient, NULL, TRUE) ;
	}

	if (bBlocking || bCapturing)
	{
		bBlocking = bCapturing = false ;
		SetCursor (LoadCursor (NULL, IDC_ARROW)) ;
		ReleaseCapture () ;
		LockWindowUpdate (NULL) ;
	}

	return hBitmap ;
}

HBITMAP Capture::CaptureSpecifiedWindow (HWND hwnd)
{	
	InvalidateRect (hwnd, NULL, TRUE) ;
	ShowWindow (hwndClient, SW_HIDE) ;
	Sleep (100) ;
	RECT rcClient ;
	HDC hdc = GetDC (hwnd) ;
	HDC hdcMem = CreateCompatibleDC (hdc) ;
	GetClientRect (hwnd, &rcClient) ;
	hBitmap = CreateCompatibleBitmap (hdc, abs (rcClient.right - rcClient.left), abs (rcClient.bottom - rcClient.top)) ;
	SelectObject (hdcMem, hBitmap) ;
	StretchBlt (hdcMem, 0, 0,
		        rcClient.right - rcClient.left, 
				rcClient.bottom - rcClient.top, 
				hdc, rcClient.left, rcClient.top, 
				rcClient.right - rcClient.left, 
				rcClient.bottom - rcClient.top, SRCCOPY) ;
	
	DeleteDC (hdcMem) ;
	ReleaseDC (hwnd, hdc) ;
	ShowWindow (hwndClient, SW_SHOW) ;
	return hBitmap ;	
}

void Capture::SaveBitmap(HBITMAP hBitmap)
{
	if (!hBitmap)	
		return ;

	HDC					hdc = NULL ;
	FILE*				fp = NULL ;
	LPVOID				pBuf = NULL ;
	BITMAPINFO			bmpInfo ;
	BITMAPFILEHEADER	bmpFileHeader ;
	char	            szFileName[512] ;

	strcpy (szFileName,"ScreenShot.bmp") ;

	OPENFILENAME	    ofn ;	
    ZeroMemory (&ofn,sizeof (ofn)) ;
    ofn.lStructSize = sizeof (OPENFILENAME) ;
    ofn.Flags = OFN_HIDEREADONLY|OFN_PATHMUSTEXIST ;
    ofn.lpstrFilter = "Bitmap Files (*.bmp)\0*.bmp\0" ;
    ofn.lpstrDefExt = "bmp" ;
    ofn.lpstrFile = szFileName ;
    ofn.nMaxFile = 512 ;	
    ofn.hwndOwner = hwndClient ; 
    if (!GetSaveFileName(&ofn))
		return ;	
	char *fileName = ofn.lpstrFile ;

	do{
		hdc = GetDC (NULL) ;
		ZeroMemory (&bmpInfo,sizeof (BITMAPINFO)) ;
		bmpInfo.bmiHeader.biSize = sizeof (BITMAPINFOHEADER) ;
		GetDIBits (hdc,hBitmap,0,0,NULL,&bmpInfo,DIB_RGB_COLORS) ;

		if (bmpInfo.bmiHeader.biSizeImage <= 0)
			bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth*abs (bmpInfo.bmiHeader.biHeight)* (bmpInfo.bmiHeader.biBitCount+7)/8 ;

		if ((pBuf=malloc(bmpInfo.bmiHeader.biSizeImage)) == NULL)
		{
			MessageBox (NULL,"Unable to Allocate Bitmap Memory","Error",MB_OK|MB_ICONERROR) ;
			break ;
		}
		
		bmpInfo.bmiHeader.biCompression = BI_RGB;
		GetDIBits (hdc,hBitmap,0,bmpInfo.bmiHeader.biHeight,pBuf,&bmpInfo,DIB_RGB_COLORS) ;	

		if ((fp = fopen (fileName,"wb")) == NULL)
		{
			MessageBox (NULL,"Unable to Create Bitmap File","Error",MB_OK|MB_ICONERROR) ;
			break ;
		}

		bmpFileHeader.bfReserved1 = 0 ;
		bmpFileHeader.bfReserved2 = 0 ;
		bmpFileHeader.bfSize = sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER) + bmpInfo.bmiHeader.biSizeImage ;
		bmpFileHeader.bfType ='MB' ;
		bmpFileHeader.bfOffBits = sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER) ;

		fwrite (&bmpFileHeader, sizeof (BITMAPFILEHEADER), 1, fp) ;
		fwrite (&bmpInfo.bmiHeader, sizeof (BITMAPINFOHEADER), 1, fp) ;
		fwrite (pBuf, bmpInfo.bmiHeader.biSizeImage, 1, fp) ;

	}while (false) ;
	if (hdc)
		ReleaseDC (NULL,hdc) ;
	if (pBuf)
		free (pBuf) ;
	if (fp)
		fclose (fp) ;
}