#include "DerivedWindow.h"

LRESULT CALLBACK CDerivedWindow::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		OnCreate();
		break;		
	case WM_PAINT:
		OnPaint();
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) 
        {
		case ID_CAPTURER_FULLSCREEN:
			OnCaptureFullScreen();		
			break;			
	    case ID_CAPTURER_SINGLEWINDOW: 
			exit(0);  
			break;
		case ID_CAPTURER_ANYAREA:
			exit(0);   
            break; 
		case ID_CAPTURER_EXIT: 
			exit(0);
		case ID_HELP_ABOUT:
			SendMessage(hwnd,WM_SYSCOMMAND,SC_MINIMIZE,0);			
			break;
		default: 
			break; 
		} 
		return 0; 
	case WM_DESTROY:
		bWindowClosed = TRUE;
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void CDerivedWindow::OnCreate()
{
	#define         BITSPERPIXEL 32;
	LPVOID		    pBits=NULL;
	static HBITMAP	hDesktopCompatibleBitmap=NULL;
	static HDC		hDesktopCompatibleDC=NULL;
	//static HDC		hDesktopDC=NULL;
	static HWND		hDesktopWnd=NULL;
	static NOTIFYICONDATA	nid;

	BITMAPINFO	bmpInfo;
			ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
			bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
			bmpInfo.bmiHeader.biBitCount=BITSPERPIXEL;
			bmpInfo.bmiHeader.biCompression = BI_RGB;
			bmpInfo.bmiHeader.biWidth=GetSystemMetrics(SM_CXSCREEN);
			bmpInfo.bmiHeader.biHeight=GetSystemMetrics(SM_CYSCREEN);
			bmpInfo.bmiHeader.biPlanes=1;
			bmpInfo.bmiHeader.biSizeImage=abs(bmpInfo.bmiHeader.biHeight)*bmpInfo.bmiHeader.biWidth*bmpInfo.bmiHeader.biBitCount/8;

			hDesktopWnd=GetDesktopWindow();
			hDesktopDC=GetDC(hDesktopWnd);
			hDesktopCompatibleDC=CreateCompatibleDC(hDesktopDC);
			hDesktopCompatibleBitmap=CreateDIBSection(hDesktopDC,&bmpInfo,DIB_RGB_COLORS,&pBits,NULL,0);
			if(hDesktopCompatibleDC==NULL || hDesktopCompatibleBitmap == NULL)
			{
				MessageBox(m_hwnd, "Unable to Create Desktop Compatible DC/Bitmap", "error",MB_OK);	
				return ;
			}
			SelectObject(hDesktopCompatibleDC,hDesktopCompatibleBitmap);

			ZeroMemory(&nid,sizeof(nid));
			nid.cbSize=sizeof(nid);
			nid.uID=1000;
			nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
			nid.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_DEFAULT));
			nid.hWnd=m_hwnd;
			strcpy(nid.szTip,"Screen Capture Application - Double Click to Start Capturing");
			nid.uCallbackMessage=NULL;//WM_NOTIFYICON_MESSAGE;
			if(!Shell_NotifyIcon(NIM_ADD,&nid))	MessageBox(NULL,"Unable to Set Notification Icon","Error",MB_ICONINFORMATION|MB_OK);

			//if((ghMenu=LoadMenu(hInst,MAKEINTRESOURCE(IDC_SCREENCAPTURE)))==NULL)
			//{
			//	ErrorMessage("Unable to Load Menu");	return -1;
			//}

			//nTimerId=SetTimer(hWnd,12345,500,NULL);	//Timer set to 500 ms.
}

void CDerivedWindow::OnPaint()
{
	if (captuered)
	{
		
		PAINTSTRUCT ps;	
		HDC hdcClient = BeginPaint(m_hwnd, &ps);
		HDC hdcWindow = GetWindowDC (m_hwnd) ;
		TextOut(hdcClient, 0, 0, "capture success!!!", 17);
		StretchBlt(hdcClient, 0, 0, nWidth, nHeight, hdcWindow, 0, 0, nWidth, nHeight, MERGECOPY) ;
		StretchBlt(hdcClient, 0, 0, nWidth, nHeight, hBmpFileDC, 0, 0, nWidth, nHeight, MERGECOPY) ;
		//BitBlt(hBmpFileDC,0,0,nWidth,nHeight,hdcClient,0,0,SRCCOPY|CAPTUREBLT);
		EndPaint(m_hwnd, &ps);
	}

	//HDC  hdcClient, hdcWindow ;
	//hdcClient = BeginPaint (m_hwnd, &ps) ;  
	//hdcWindow = GetWindowDC (m_hwnd) ;
	////StretchBlt (hdcClient, 0, 0, cxClient, cyClient, hdcWindow, 0, 0, cxSource, cySource, MERGECOPY) ;
	//BitBlt(hBmpFileDC,0,0,nWidth,nHeight,hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);
	//ReleaseDC (m_hwnd, hdcWindow) ;
	//EndPaint (m_hwnd, &ps) ;
}

void CDerivedWindow::OnCaptureFullScreen()
{
	OPENFILENAME	ofn;
	char	szFileName[512];
	ShowWindow(m_hwnd,SW_HIDE);

    strcpy(szFileName,"ScreenShot.bmp");

    ZeroMemory(&ofn,sizeof(ofn));
    ofn.lStructSize=sizeof(OPENFILENAME);
    ofn.Flags=OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;
    ofn.lpstrFilter="Bitmap Files (*.bmp)\0*.bmp\0";
    ofn.lpstrDefExt="bmp";
    ofn.lpstrFile=szFileName;
    ofn.nMaxFile=512;
    ofn.hwndOwner = m_hwnd;
    if(!GetSaveFileName(&ofn))	return;				

    SetCursor(LoadCursor(NULL,IDC_WAIT));	
    nWidth = GetSystemMetrics(SM_CXSCREEN);
    nHeight = GetSystemMetrics(SM_CYSCREEN);
    
	hBmpFileDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP	hBmpFileBitmap = CreateCompatibleBitmap(hDesktopDC,nWidth,nHeight);

    HBITMAP hOldBitmap = (HBITMAP) SelectObject(hBmpFileDC,hBmpFileBitmap);
    BitBlt(hBmpFileDC,0,0,nWidth,nHeight,hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);
    SelectObject(hBmpFileDC,hOldBitmap);

    SaveBitmap(ofn.lpstrFile,hBmpFileBitmap);				

    DeleteDC(hBmpFileDC);
    DeleteObject(hBmpFileBitmap);
    SetCursor(LoadCursor(NULL,IDC_ARROW));

	ShowWindow(m_hwnd,SW_SHOW);
    RECT ClientRect;                                        
    GetClientRect(m_hwnd, &ClientRect);
    InvalidateRect(m_hwnd, &ClientRect, TRUE);
	captuered = true;
}

void CDerivedWindow::SaveBitmap(char *szFilename,HBITMAP hBitmap)
{
	HDC					hdc=NULL;
	FILE*				fp=NULL;
	LPVOID				pBuf=NULL;
	BITMAPINFO			bmpInfo;
	BITMAPFILEHEADER	bmpFileHeader;

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

		if((fp=fopen(szFilename,"wb"))==NULL)
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