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
		hBmpFileBitmap = NULL;     
		SetWindowTitle("Capturer");
	};		

	void OnCreate();
	void OnPaint();
	HBITMAP OnCaptureFullScreen();
	void SaveBitmap(HBITMAP hBitmap);

protected:	
	HBITMAP	hBmpFileBitmap;

	LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	
};

#endif 