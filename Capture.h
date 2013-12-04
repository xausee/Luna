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
	HBITMAP	hBitmap;
	HWND hwndScreen;

	HBITMAP CaptureFullScreen () ;
	void SaveBitmap (HBITMAP hBitmap) ;
	void InvertBlock (HWND hwnd);
};

