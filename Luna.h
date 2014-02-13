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
		hSelection = false ;
		hPenRectangle = false ;
		hPenCycle = false ;
		hPenText = false ;
		hPenLine = false ;
	    hPenSize = 0 ;
	  	hPenColor = 0 ;

		capture = new(Capture);		
		SetWindowTitle ("Luna") ;
	};		
	Capture * capture ;
	MouseHook *cpMouseHook ;
	
	void OnPaint();
	void OnLButtonDown (WPARAM wParam, LPARAM lParam) ;	
	void OnCaptureAnyArea () ;
	void OnCaptureSpecifiedWindow () ;
	int  GetEncoderClsid (const WCHAR* format, CLSID* pClsid) ;
	void CreateToolbar () ;
	void SaveFile () ;

protected:	
	HBITMAP	hBitmap;
	bool	hSelection;
	bool	hPenRectangle;
	bool	hPenCycle;
	bool	hPenText;
	bool	hPenLine;
	int	    hPenSize;
	int  	hPenColor;
	LRESULT CALLBACK WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;	
};

#endif 