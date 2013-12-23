#pragma once
#include "BaseWindow.h"
#include "Capture.h"

class TrasparentWindow : public CBaseWindow
{
public:
	TrasparentWindow::TrasparentWindow (HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL) : CBaseWindow(hInst, wcx)
	{
		hBitmap = NULL;
		capture = new(Capture) ;	
		SetWindowTitle ("Luna") ;
	} ;

	~TrasparentWindow(void) ;	

	Capture * capture ;	
	HBITMAP	hBitmap ;

	BOOL Create() ;
	void SetWindowTransparent (HWND hwnd) ;
	HBITMAP CreateDesktopBitmap () ;
	void OnCreate (HWND hwnd) ;
	void OnPaint () ;
	void OnLButtonDown (WPARAM wParam, LPARAM lParam) ;
	void OnLButtonUp (WPARAM wParam, LPARAM lParam) ;
	void OnMouseMove (WPARAM wParam, LPARAM lParam) ;

protected:	
	LRESULT CALLBACK WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;
};

