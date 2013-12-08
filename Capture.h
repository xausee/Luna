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

	/* 
    捕捉任意区域的成员函数
	*/
	void InvertBlock () ;
	void InitCaptureAnyArea() ;
	void StartCaptureAnyArea (POINT pBeg) ;
	void MarkCaptureArea (POINT pEnd) ;
	HBITMAP EndCaptureAnyArea (POINT ptEnd) ;

	/*
	捕捉全屏的成员函数
	*/
	HBITMAP CaptureFullScreen () ;

	/*
	捕捉鼠标所在窗口的成员函数
	*/
	void CaptureSpecifiedWindow ();

	/*
	保存截图的成员函数
	*/
	void SaveBitmap (HBITMAP hBitmap) ;	
};

