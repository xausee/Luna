#pragma once
#include "BaseWindow.h"

class TrasparentWindow : public CBaseWindow
{
public:
	TrasparentWindow::TrasparentWindow (HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL) : CBaseWindow(hInst, wcx)
	{
		hBitmap = NULL;
		SetWindowTitle ("Luna") ;
	} ;

	~TrasparentWindow(void) ;

	BOOL Create() ;
	void SetWindowTransparent (HWND hwnd) ;
	HBITMAP CreateDesktopBitmap () ;
	void OnCreate (HWND hwnd) ;
	void OnPaint () ;

protected:
	HBITMAP	hBitmap ;
	LRESULT CALLBACK WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;
};

