// this is a window derived form CBaseWindow
// just to show that inheritance is possible

#ifndef _LUNA_H_
#define _LUNA_H_
#include "BaseWindow.h"
#include "TrasparentWindow.h"
#include "EditPictureWindow.h"
#include "Capture.h"
#include "Structs.h"
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
		hWndToolbar = NULL ;
		hwndEditBox = NULL ;
		hwndEditWindow = NULL ;
		EditPictureThread = NULL ;

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
	bool CreateEditChildWindow () ;
	void CreateToolbar () ;
	void UpdateToobar () ;
	void CloseToolbar () ;
	HBITMAP SaveBitmapToMemory () ;
	void SaveFile () ;	
	void ShowPictureInViewModel () ;

protected:	
	HWND    hWndToolbar ;
	HWND    hwndEditBox ;
	HWND    hwndEditWindow ;
	HANDLE  EditPictureThread;
	HBITMAP	hBitmap ;	
	POINT   pBeg ;
	POINT   pEnd ;
	RECT    rBitmapRect ;
	LRESULT CALLBACK WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;	
};

#endif 