#ifndef _EDITPICTURECHILDWINDOW_H_
#define _EDITPICTURECHILDWINDOW_H_
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

bool				RegisterEditPictureChildWindowClass (HINSTANCE);
HWND				CreateEditPictureChildWindow (HINSTANCE, HWND, int);
LRESULT CALLBACK	EditPictureChildWindowWndProc (HWND, UINT, WPARAM, LPARAM);

static void OnPaintEditWindow ();
static void OnLButtonDownEditWindow (WPARAM wParam, LPARAM lParam) ;	
static void OnLButtonUPEditWindow (WPARAM wParam, LPARAM lParam) ;
static void OnMouseMoveEditWindow (WPARAM wParam, LPARAM lParam) ;
static void InitializeScrollsEditWindow (HWND hwnd, int cx, int cy) ;
static void InitializeHScrollEditWindow (WPARAM wParam, LPARAM lParam) ;
static void InitializeVScrollEditWindow (WPARAM wParam, LPARAM lParam) ;
static void OnHScrollEditWindow (WPARAM wParam, LPARAM lParam) ;
static void OnVScrollEditWindow (WPARAM wParam, LPARAM lParam) ;
static int  GetLineSizeEditWindow () ;
static COLORREF GetColorEditWindow () ;
static void ShowPictureInEditModelEditWindow () ;
static void ShapeEditWindow (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
static void DrawLineEditWindow (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
static HWND CreateEditBoxEditWindow () ;
static LRESULT SetEditBoxEditWindow (WPARAM wParam, LPARAM lParam) ;
static void TextOutFromEditBoxToCanvasEditWindow () ;
static void DrawRectangleEditWindow (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
static void DrawEllipseEditWindow (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
static void InvalideAndUpdateClient () ;
static HBITMAP SelectBitmap () ;
static HBITMAP SaveBitmapToMemoryEditWindow () ;
static HWND GetHwndEditWindow () ;
static HBITMAP SetBitmapEditWindow (HBITMAP	srcHBitmap) ;
static HBITMAP GetBitmapEditWindow () ;

#endif