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
	//if(!Shell_NotifyIcon(NIM_ADD,&nid))	MessageBox(NULL,"Unable to Set Notification Icon","Error",MB_ICONINFORMATION|MB_OK);
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
		RECT rcClient;
		GetClientRect(m_hwnd, &rcClient);
		HDC hdcClient = BeginPaint(m_hwnd, &ps);
		//SetCursor(LoadCursor(NULL,IDC_WAIT)); 

		//hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL); 
		//hdcCompatible = CreateCompatibleDC(hdcScreen);
		//hbmScreen = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);				
		//SelectObject(hdcCompatible, hbmScreen);	

	 //   fileName = "G:\ScreenShot.bmp";
		//HBITMAP   hbmp=(HBITMAP)LoadImage(hInstance, // small class icon 
  //      fileName,
  //      IMAGE_BITMAP, 
  //      0, 
  //      0, 
  //      LR_LOADFROMFILE); 

		//hdcCompatible = CreateCompatibleDC(hdcScreen);
		//hbmScreen = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);				
		//SelectObject(hdcCompatible, hbmScreen);	
		////BitBlt(hdcClient, 0,0, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, hdcScreen, 0, 0, SRCCOPY);			
		//StretchBlt(hdcClient, 0, 0,  rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, hdcScreen, 0, 0, nWidth, nHeight, MERGECOPY);	
		//
		BITMAP         bm ;
		if (hBmpFileBitmap)
          {
               GetClientRect (m_hwnd, &rcClient) ;

               HDC hdcMem = CreateCompatibleDC (hdcClient) ;
               SelectObject (hdcMem, hBmpFileBitmap) ;
               GetObject (hBmpFileBitmap, sizeof (BITMAP), (PSTR) &bm) ;
               SetStretchBltMode (hdcClient, COLORONCOLOR) ;

               StretchBlt (hdcClient,    0, 0, rcClient.right, rcClient.bottom,
                           hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY) ;

               DeleteDC (hdcMem) ;
          }
		EndPaint(m_hwnd, &ps);
	}			
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
    /*HBITMAP*/	hBmpFileBitmap = CreateCompatibleBitmap(hDesktopDC,nWidth,nHeight);
    HBITMAP hOldBitmap = (HBITMAP) SelectObject(hBmpFileDC,hBmpFileBitmap);
    BitBlt(hBmpFileDC,0,0,nWidth,nHeight,hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);
    SelectObject(hBmpFileDC,hOldBitmap);
	fileName = new char[255];
	int i = 0;
	while(szFileName[i]!=0)
	{
		fileName[i] = szFileName[i];
		i++;
	}	
	
    SaveBitmap(ofn.lpstrFile,hBmpFileBitmap);				

  /*  DeleteDC(hBmpFileDC);
    DeleteObject(hBmpFileBitmap);*/
    SetCursor(LoadCursor(NULL,IDC_ARROW));

	hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL); 
	hdcCompatible = CreateCompatibleDC(hdcScreen);
	hbmScreen = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);				
	SelectObject(hdcCompatible, hbmScreen);	
	ShowWindow(m_hwnd,SW_HIDE);	

	CaptureAnImage();
	ShowWindow(m_hwnd,SW_SHOW); 	
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

int CDerivedWindow::CaptureAnImage()
{
   /* HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;*/

    // Retrieve the handle to a display device context for the client 
    // area of the window. 
    hdcScreen = GetDC(NULL);
    hdcWindow = GetDC(m_hwnd);

    // Create a compatible DC which is used in a BitBlt from the window DC
    hdcMemDC = CreateCompatibleDC(hdcWindow); 

    if(!hdcMemDC)
    {
        MessageBox(m_hwnd, "CreateCompatibleDC has failed","Failed", MB_OK);
        goto done;
    }

    // Get the client area for size calculation
    RECT rcClient;
    GetClientRect(m_hwnd, &rcClient);

    //This is the best stretch mode
    SetStretchBltMode(hdcWindow,HALFTONE);

    //The source DC is the entire screen and the destination DC is the current window (HWND)
    if(!StretchBlt(hdcWindow, 
               0,0, 
               rcClient.right, rcClient.bottom, 
               hdcScreen, 
               0,0,
               GetSystemMetrics (SM_CXSCREEN),
               GetSystemMetrics (SM_CYSCREEN),
               SRCCOPY))
    {
        MessageBox(m_hwnd, "StretchBlt has failed","Failed", MB_OK);
        goto done;
    }
    
    // Create a compatible bitmap from the Window DC
    hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top);
    
    if(!hbmScreen)
    {
        MessageBox(m_hwnd, "CreateCompatibleBitmap Failed","Failed", MB_OK);
        goto done;
    }

    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(hdcMemDC,hbmScreen);
    
    // Bit block transfer into our compatible memory DC.
    if(!BitBlt(hdcMemDC, 
               0,0, 
               rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, 
               hdcWindow, 
               0,0,
               SRCCOPY))
    {
        MessageBox(m_hwnd, "BitBlt has failed", "Failed", MB_OK);
        goto done;
    }

    // Get the BITMAP from the HBITMAP
    GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);
     
    BITMAPFILEHEADER   bmfHeader;    
    BITMAPINFOHEADER   bi;
     
    bi.biSize = sizeof(BITMAPINFOHEADER);    
    bi.biWidth = bmpScreen.bmWidth;    
    bi.biHeight = bmpScreen.bmHeight;  
    bi.biPlanes = 1;    
    bi.biBitCount = 32;    
    bi.biCompression = BI_RGB;    
    bi.biSizeImage = 0;  
    bi.biXPelsPerMeter = 0;    
    bi.biYPelsPerMeter = 0;    
    bi.biClrUsed = 0;    
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
    // have greater overhead than HeapAlloc.
    HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize); 
    char *lpbitmap = (char *)GlobalLock(hDIB);    

    // Gets the "bits" from the bitmap and copies them into a buffer 
    // which is pointed to by lpbitmap.
    GetDIBits(hdcWindow, hbmScreen, 0,
        (UINT)bmpScreen.bmHeight,
        lpbitmap,
        (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    // A file is created, this is where we will save the screen capture.
    HANDLE hFile = CreateFile("captureqwsx.bmp",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);   
    
    // Add the size of the headers to the size of the bitmap to get the total file size
    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
    //Offset to where the actual bitmap bits start.
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 
    
    //Size of the file
    bmfHeader.bfSize = dwSizeofDIB; 
    
    //bfType must always be BM for Bitmaps
    bmfHeader.bfType = 0x4D42; //BM   
 
    DWORD dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
    
    //Unlock and Free the DIB from the heap
    GlobalUnlock(hDIB);    
    GlobalFree(hDIB);

    //Close the handle for the file that was created
    CloseHandle(hFile);
       
    //Clean up
done:
    /*DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL,hdcScreen);
    ReleaseDC(m_hwnd,hdcWindow);*/

    return 0;
}