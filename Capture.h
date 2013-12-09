#pragma once
#pragma warning( disable : 4996)
#include <windows.h>
#include <stdio.h>

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
    member functions of capture any area
	*/
	void InvertBlock () ;
	void InitCaptureAnyArea() ;
	void StartCaptureAnyArea (POINT pBeg) ;
	void MarkCaptureArea (POINT pEnd) ;
	HBITMAP EndCaptureAnyArea (POINT ptEnd) ;

	/*
	member functions of capture full screnn
	*/
	HBITMAP CaptureFullScreen () ;

	/*
	member functions of capture specified window
	*/
	void CaptureSpecifiedWindow (POINT point);

	/*
	save the bitmap member function
	*/
	void SaveBitmap (HBITMAP hBitmap) ;	
};

