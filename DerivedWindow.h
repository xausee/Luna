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
		hdcScreen = NULL;
	    hdcCompatible = NULL;
		//hbmScreen = NULL;

		 hdcScreen  = NULL;
     hdcWindow = NULL;
     hdcMemDC = NULL;
     hbmScreen = NULL;
	 fileName=NULL;
	 hBmpFileBitmap=NULL;
     
		SetWindowTitle("Capturer");
	};
	
	LPSTR fileName;
	bool captuered;
	int	 nWidth,nHeight;
	HDC	 hBmpFileDC;
	HDC  hDesktopDC;
	//HDC  hdcScreen; 
	HDC  hdcCompatible;
	//HBITMAP hbmScreen;


	HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC;
    HBITMAP hbmScreen;
    BITMAP bmpScreen;
	HBITMAP	hBmpFileBitmap;

	int CaptureAnImage();

	void OnCreate();
	void OnPaint();
	void OnCaptureFullScreen();
	void SaveBitmap(char *szFilename,HBITMAP hBitmap);

protected:
	// the message handler for this window
	LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	
};

#endif 