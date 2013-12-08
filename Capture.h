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
    ��׽��������ĳ�Ա����
	*/
	void InvertBlock () ;
	void InitCaptureAnyArea() ;
	void StartCaptureAnyArea (POINT pBeg) ;
	void MarkCaptureArea (POINT pEnd) ;
	HBITMAP EndCaptureAnyArea (POINT ptEnd) ;

	/*
	��׽ȫ���ĳ�Ա����
	*/
	HBITMAP CaptureFullScreen () ;

	/*
	��׽������ڴ��ڵĳ�Ա����
	*/
	void CaptureSpecifiedWindow ();

	/*
	�����ͼ�ĳ�Ա����
	*/
	void SaveBitmap (HBITMAP hBitmap) ;	
};

