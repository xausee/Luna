// this is a window derived form CBaseWindow
// just to show that inheritance is possible

#ifndef _LUNA_H_
#define _LUNA_H_
#include "BaseWindow.h"
#include "TrasparentWindow.h"
#include "Capture.h"
#include "resource.h"
#include <windows.h>
#include <Commctrl.h>
#include <stdio.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"Comctl32.lib")

class Luna : public CBaseWindow
{
public:	
	Luna (HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL) : CBaseWindow(hInst, wcx)
	{ 
		hBitmap = NULL ;  
		bSelection = false ;
		bPenRectangle = false ;
		bPenCycle = false ;
		bPenText = false ;
		bPenLine = false ;
	    iPenSize = 0 ;
	  	iPenColor = 0 ;
		bDrawing = false ;
		hWndToolbar = NULL ;

		capture = new(Capture);		
		SetWindowTitle ("Luna") ;
	};		
	Capture * capture ;
	MouseHook *cpMouseHook ;
	
	void OnPaint();
	void OnLButtonDown (WPARAM wParam, LPARAM lParam) ;	
	void OnLButtonUP (WPARAM wParam, LPARAM lParam) ;
	void OnMouseMove (WPARAM wParam, LPARAM lParam) ;
	void OnCaptureAnyArea () ;
	void OnCaptureSpecifiedWindow () ;
	int  GetEncoderClsid (const WCHAR* format, CLSID* pClsid) ;
	void CreateToolbar () ;
	void CloseToolbar () ;
	void SaveFile () ;
	void DrawRectangle (HWND hwnd, POINT pBeg, POINT pEnd) ;

protected:	
	HWND    hWndToolbar ;
	HBITMAP	hBitmap ;
	bool	bSelection ;
	bool	bDrawing ;
	bool	bPenRectangle ;	
	bool	bPenCycle ;
	bool	bPenText ;
	bool	bPenLine ;
	int	    iPenSize ;
	int  	iPenColor ;
	POINT   pBeg ;
	POINT   pEnd ;
	LRESULT CALLBACK WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;	
};

#endif 