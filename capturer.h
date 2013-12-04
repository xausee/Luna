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
	CDerivedWindow (HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL) 
		:CBaseWindow(hInst, wcx)
	{ 
		bCapturing = false ;
		bBlocking = false ;
		
		hBitmap = NULL ;     
		SetWindowTitle ("Capturer") ;
	};	

	bool bCapturing ;
	bool bBlocking ;
	POINT ptBeg, ptEnd ;
	HWND hwndScr;


	void OnCreate();
	void OnPaint();
	void OnLButtonDown (WPARAM wParam, LPARAM lParam) ;
	void OnLButtonUp (WPARAM wParam, LPARAM lParam) ;
	void OnRButtonDown (WPARAM wParam, LPARAM lParam) ;
	void OnRButtonUp (WPARAM wParam, LPARAM lParam) ;
	void OnMouseMove (WPARAM wParam, LPARAM lParam) ;
	void InvertBlock (HWND hwndScr, HWND hwnd, POINT ptBeg, POINT ptEnd) ;

	HBITMAP CaptureFullScreen () ;
	void SaveBitmap (HBITMAP hBitmap) ;

protected:	
	HBITMAP	hBitmap;

	LRESULT CALLBACK WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;	
};

#endif 