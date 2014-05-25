// this is a window derived form CBaseWindow
// just to show that inheritance is possible

#ifndef _EDITWINDOW_H_
#define _EDITWINDOW_H_
#include "BaseWindow.h"
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

class EditWindow : public CBaseWindow
{
public:	
	EditWindow (HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL) : CBaseWindow(hInst, wcx)
	{ 
		hBitmap = NULL ;  
		isEdit = false ;
		bSelection = false ;		
		iShape = 0 ;
	    iPenSize = 0 ;
	  	iPenColor = 0 ;
		bDrawing = false ;		
		hwndEditBox = NULL ;
		fScroll = false ;
		hScroll.xCurrentScroll = 0 ;
		hScroll.xCurrentScroll = 0;
		hScroll.xMinScroll = 0 ;
		vScroll.yCurrentScroll = 0 ;
		vScroll.yMaxScroll = 0 ;
		vScroll.yMinScroll = 0 ;	
	};		
	
	void OnPaint();
	void OnLButtonDown (WPARAM wParam, LPARAM lParam) ;	
	void OnLButtonUP (WPARAM wParam, LPARAM lParam) ;
	void OnMouseMove (WPARAM wParam, LPARAM lParam) ;
	void InitializeHScroll (WPARAM wParam, LPARAM lParam) ;
	void InitializeVScroll (WPARAM wParam, LPARAM lParam) ;
	void OnHScroll (WPARAM wParam, LPARAM lParam) ;
	void OnVScroll (WPARAM wParam, LPARAM lParam) ;
	int  GetLineSize () ;
	COLORREF GetColor () ;
	void ShowPictureInEditModel () ;
	void Shape (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
	void DrawLine(HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
	HWND CreateEditBox() ;
	LRESULT SetEditBox(WPARAM wParam, LPARAM lParam) ;
	void TextOutFromEditBoxToCanvas () ;
	void DrawRectangle(HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
	void DrawEllipse(HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
	HBITMAP EditWindow::SaveBitmapToMemory () ;
	HWND GetHwnd () ;

protected:		
	HBITMAP	hBitmap ;
	HWND    hwndEditBox ;
	bool    fScroll ;
	HScroll hScroll ;
	VScroll vScroll ;
	bool    isEdit ;
	bool	bSelection ;
	bool	bDrawing ;
	int 	iShape ;	
	bool	bPenLine ;
	int	    iPenSize ;
	int  	iPenColor ;
	POINT   pBeg ;
	POINT   pEnd ;
	RECT    rBitmapRect ;
	LRESULT CALLBACK WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;	
};

#endif 