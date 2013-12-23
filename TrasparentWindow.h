#pragma once
#include "BaseWindow.h"

class TrasparentWindow : public CBaseWindow
{
public:
	TrasparentWindow::TrasparentWindow (HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL) : CBaseWindow(hInst, wcx)
	{
		SetWindowTitle ("Luna") ;
	} ;

	~TrasparentWindow(void) ;

	BOOL Create() ;

protected:	
	LRESULT CALLBACK WinMsgHandler (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;
};

