// this is a window derived form CBaseWindow
// just to show that inheritance is possible

#ifndef _DERIVEDWINDOW_H_
#define _DERIVEDWINDOW_H_
#include "BaseWindow.h"
#include "Capture.h"
#include "resource.h"
#include <windows.h>
#include <stdio.h>

class CDerivedWindow : public CBaseWindow
{
public:	
	CDerivedWindow (HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL) 
		:CBaseWindow(hInst, wcx)
	{ 
		hBitmap = NULL ;  
		capture = new(Capture);
		SetWindowTitle ("Luna") ;
	};	

	Capture * capture;

	void OnCreate();
	void OnPaint();
	void OnLButtonDown (WPARAM wParam, LPARAM lParam) ;
	void OnLButtonUp (WPARAM wParam, LPARAM lParam) ;
	void OnRButtonDown (WPARAM wParam, LPARAM lParam) ;
	void OnRButtonUp (WPARAM wParam, LPARAM lParam) ;
	void OnMouseMove (WPARAM wParam, LPARAM lParam) ;	

protected:	
	HBITMAP	hBitmap;

	LRESULT CALLBACK WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;	
};

#endif 