#pragma once
#pragma warning( disable : 4996)
#include <windows.h>
#include <stdio.h>
#include "MouseHook.h"

class Capture
{
public:
	Capture(void);
	~Capture(void);

	bool bCapturing ;
	bool bBlocking ;
	bool bSpecifiedWindow ;
	POINT ptBeg, ptEnd ;	
	HBITMAP	hBitmap ;
	HWND hwndClient ;
	HWND hwndScreen ;

	/* 
    member functions of capturing any area
	*/
	void InvertBlock () ;
	void InitCaptureAnyArea() ;
	void StartCaptureAnyArea (POINT pBeg) ;
	void MarkCaptureArea (POINT pEnd) ;
	HBITMAP EndCaptureAnyArea (POINT ptEnd) ;

	/*
	member functions of capturing full screen
	*/
	HBITMAP CaptureFullScreen () ;

	/*
	member functions of capturing specified window
	*/
	HBITMAP CaptureSpecifiedWindow (HWND hwnd);

	/*
	save the bitmap member function
	*/
	void SaveBitmap (HBITMAP hBitmap) ;	
};

