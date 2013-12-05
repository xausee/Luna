#pragma once
#include <windows.h>
#include <stdio.h>

class Capture
{
public:
	Capture(void);
	~Capture(void);

	bool bCapturing ;
	bool bBlocking ;
	POINT ptBeg, ptEnd ;	
	HBITMAP	hBitmap ;
	HWND hwndClient ;
	HWND hwndScreen ;

	void Init () ;
	void InitCaptureAnyArea() ;
	void StartCaptureAnyArea (POINT pBeg) ;
	HBITMAP EndCaptureAnyArea (POINT ptEnd) ;
	HBITMAP CaptureFullScreen () ;
	void SaveBitmap (HBITMAP hBitmap) ;
	void InvertBlock (HWND hwnd);
};

