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
		iShape = 0 ;
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
	void UpdateToobar () ;
	void CloseToolbar () ;
	void SaveFile () ;
	int  GetLineSize () ;
	COLORREF GetColor () ;
	void Shape (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
	void DrawRectangle (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
	void DrawEllipse (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
	void DrawLine (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
	HWND CreateEditBox() ;

protected:	
	HWND    hWndToolbar ;
	HBITMAP	hBitmap ;
	bool	bSelection ;
	bool	bDrawing ;
	int 	iShape ;	
	bool	bPenLine ;
	int	    iPenSize ;
	int  	iPenColor ;
	POINT   pBeg ;
	POINT   pEnd ;
	LRESULT CALLBACK WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;	
};

#endif 