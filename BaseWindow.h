// here is the base class for my windows

#ifndef _BASEWINDOW_H_
#define _BASEWINDOW_H_
#include <windows.h>

class CBaseWindow
{
public:
	// many different ways to register
	virtual BOOL RegisterWindow();
	virtual BOOL RegisterWindow(UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, 
		LPCTSTR lpszMenuName, LPCTSTR lpszClassName, HICON hIconSm);
	virtual BOOL RegisterWindow(CONST WNDCLASSEX* wcx);

	// static message handler to put in WNDCLASSEX structure
	static LRESULT CALLBACK stWinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// just so you can change the window caption...
	void SetWindowTitle(LPCTSTR lpszTitle) 
	{
		ZeroMemory(szWindowTitle, sizeof(char)*256);
		strcpy(szWindowTitle, lpszTitle);
	};

	// 2 ways to create
	virtual BOOL Create();
	virtual BOOL Create(DWORD dwStyles, RECT* rect);

	void MsgLoop();
	BOOL IsWindowClosed() { return bWindowClosed; };

protected:
	HINSTANCE hInstance;
	HWND m_hwnd;
	BOOL bWindowClosed;
	char szClassName[256];
	char szWindowTitle[256];

	//contructor 
	CBaseWindow(HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL) 
		:m_hwnd(NULL),
		hInstance(NULL),
		bWindowClosed(FALSE)
	{ 
		hInstance = hInst; 
		if (wcx != NULL) RegisterWindow(wcx);
	};

	// the real message handler
	virtual LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	// returns a pointer the window (stored as the WindowLong)
	inline static CBaseWindow *GetObjectFromWindow(HWND hWnd)
	{
		return (CBaseWindow *)GetWindowLong(hWnd, GWL_USERDATA);
	}
};

#endif 