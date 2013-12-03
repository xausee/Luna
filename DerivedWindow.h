// this is a window derived form CBaseWindow
// just to show that inheritance is possible

#ifndef _DERIVEDWINDOW_H_
#define _DERIVEDWINDOW_H_
#include "BaseWindow.h"
#include "resource.h"
#include <windows.h>
#include <stdio.h>

class CDerivedWindow : public CBaseWindow
{
public:	
	CDerivedWindow(HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL) 
		:CBaseWindow(hInst, wcx)
	{ 
		captuered = false;
		hBmpFileDC = NULL;
		hDesktopDC = NULL;	
		hdcScreen  = NULL;
	    hdcCompatible = NULL;
		hdcWindow = NULL;
		hdcMemDC = NULL;
		hBmpFileBitmap=NULL;
     
		SetWindowTitle("Capturer");
	};	
	
	bool captuered;
	int	 nWidth,nHeight;
	HDC	 hBmpFileDC;
	HDC  hDesktopDC;
	HDC  hdcCompatible;

	HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC;
    HBITMAP hbmScreen;
    BITMAP  bmpScreen;
	HBITMAP	hBmpFileBitmap;

	void OnCreate();
	void OnPaint();
	void OnCaptureFullScreen();
	void SaveBitmap(HBITMAP hBitmap);

protected:
	// the message handler for this window
	LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	
};

#endif 