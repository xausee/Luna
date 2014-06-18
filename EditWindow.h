#ifndef _EDITWINDOW_H_
#define _EDITWINDOW_H_
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

bool				RegisterEditWindowClass (HINSTANCE);
HWND				CreateEditWindow (HINSTANCE, HWND, int);
LRESULT CALLBACK	EditWindowWndProc (HWND, UINT, WPARAM, LPARAM);

static void OnPaint ();
static void OnLButtonDown (WPARAM wParam, LPARAM lParam) ;	
static void OnLButtonUP (WPARAM wParam, LPARAM lParam) ;
static void OnMouseMove (WPARAM wParam, LPARAM lParam) ;
static void OnKeyDown (UINT uMsg, WPARAM wParam, LPARAM lParam) ;
static void InitializeScrolls (HWND hwnd, int cx, int cy) ;
static void InitializeHScroll (WPARAM wParam, LPARAM lParam) ;
static void InitializeVScroll (WPARAM wParam, LPARAM lParam) ;
static void OnHScroll (WPARAM wParam, LPARAM lParam) ;
static void OnVScroll (WPARAM wParam, LPARAM lParam) ;
static int  GetLineSize () ;
static COLORREF GetColor () ;
static void ShowPictureInEditModel () ;
static void Shape (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
static void DrawLine (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
static HWND CreateEditBox () ;
static LRESULT SetEditBox (WPARAM wParam, LPARAM lParam) ;
static void TextOutFromEditBoxToCanvas () ;
static void DrawRectangle (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
static void DrawEllipse (HWND hwnd, POINT pBeg, POINT pEnd, int bModel) ;
static void InvalideAndUpdateClient () ;
static HBITMAP SelectBitmap () ;
static HBITMAP SaveBitmapToMemory () ;
static HWND GetHwnd () ;
static HBITMAP SetBitmap (HBITMAP	srcHBitmap) ;
static HBITMAP GetBitmap () ;

#endif