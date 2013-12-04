#include "Capture.h"


Capture::Capture(void)
{
	bCapturing = false ;
	bBlocking  = false ;
	hBitmap    = NULL ; 
	hwndScreen = NULL;
	ptBeg.x    = 0;
	ptBeg.y    = 0;
	ptEnd.x    = 0;
	ptEnd.y    = 0;
}

Capture::~Capture(void)
{
}

void Capture::InvertBlock (HWND hwnd)
{	
	HDC hdc = GetDCEx (hwndScreen, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE) ;
	ClientToScreen (hwnd, &ptBeg) ;
	ClientToScreen (hwnd, &ptEnd) ;
	PatBlt (hdc, ptBeg.x, ptBeg.y, ptEnd.x - ptBeg.x, ptEnd.y - ptBeg.y, DSTINVERT) ;
    ReleaseDC (hwndScreen, hdc) ;
}

HBITMAP Capture::CaptureFullScreen()
{	
	HWND hwnd = GetForegroundWindow();
	ShowWindow (hwnd, SW_HIDE);
	Sleep(1000);
    SetCursor(LoadCursor(NULL,IDC_WAIT));	
    int nWidth = GetSystemMetrics(SM_CXSCREEN);
    int nHeight = GetSystemMetrics(SM_CYSCREEN);

	HWND hDesktopWnd = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktopWnd);    
	HDC hBmpFileDC = CreateCompatibleDC(hDesktopDC);

    hBitmap = CreateCompatibleBitmap(hDesktopDC,nWidth,nHeight);

    HBITMAP hOldBitmap = (HBITMAP) SelectObject(hBmpFileDC, hBitmap);
    BitBlt(hBmpFileDC,0,0,nWidth,nHeight,hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);
    SelectObject(hBmpFileDC,hOldBitmap); 
	 
	DeleteDC(hDesktopDC);
	DeleteObject(hDesktopWnd);
	DeleteObject(hOldBitmap);
	DeleteDC(hBmpFileDC);
	
    SetCursor(LoadCursor(NULL,IDC_ARROW));
	ShowWindow(hwnd,SW_SHOW);	
		
	return hBitmap;
}

void Capture::SaveBitmap(HBITMAP hBitmap)
{
	if (!hBitmap)	
		return;

	HDC					hdc=NULL;
	FILE*				fp=NULL;
	LPVOID				pBuf=NULL;
	BITMAPINFO			bmpInfo;
	BITMAPFILEHEADER	bmpFileHeader;
	char	            szFileName[512];

	strcpy(szFileName,"ScreenShot.bmp");

	OPENFILENAME	    ofn;	
    ZeroMemory(&ofn,sizeof(ofn));
    ofn.lStructSize=sizeof(OPENFILENAME);
    ofn.Flags=OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;
    ofn.lpstrFilter="Bitmap Files (*.bmp)\0*.bmp\0";
    ofn.lpstrDefExt="bmp";
    ofn.lpstrFile=szFileName;
    ofn.nMaxFile=512;	
    ofn.hwndOwner = GetForegroundWindow();
    if(!GetSaveFileName(&ofn))	return;	
	char *fileName = ofn.lpstrFile;

	do{
		hdc=GetDC(NULL);
		ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
		bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		GetDIBits(hdc,hBitmap,0,0,NULL,&bmpInfo,DIB_RGB_COLORS);

		if(bmpInfo.bmiHeader.biSizeImage<=0)
			bmpInfo.bmiHeader.biSizeImage=bmpInfo.bmiHeader.biWidth*abs(bmpInfo.bmiHeader.biHeight)*(bmpInfo.bmiHeader.biBitCount+7)/8;

		if((pBuf=malloc(bmpInfo.bmiHeader.biSizeImage))==NULL)
		{
			MessageBox(NULL,"Unable to Allocate Bitmap Memory","Error",MB_OK|MB_ICONERROR);
			break;
		}
		
		bmpInfo.bmiHeader.biCompression=BI_RGB;
		GetDIBits(hdc,hBitmap,0,bmpInfo.bmiHeader.biHeight,pBuf,&bmpInfo,DIB_RGB_COLORS);	

		if((fp=fopen(fileName,"wb"))==NULL)
		{
			MessageBox(NULL,"Unable to Create Bitmap File","Error",MB_OK|MB_ICONERROR);
			break;
		}

		bmpFileHeader.bfReserved1=0;
		bmpFileHeader.bfReserved2=0;
		bmpFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+bmpInfo.bmiHeader.biSizeImage;
		bmpFileHeader.bfType='MB';
		bmpFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

		fwrite(&bmpFileHeader,sizeof(BITMAPFILEHEADER),1,fp);
		fwrite(&bmpInfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);
		fwrite(pBuf,bmpInfo.bmiHeader.biSizeImage,1,fp);

	}while(false);
	if(hdc)
		ReleaseDC(NULL,hdc);
	if(pBuf)
		free(pBuf);
	if(fp)
		fclose(fp);
}