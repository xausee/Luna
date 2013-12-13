#pragma once
#include <windows.h>

typedef struct HookData
{    
	HINSTANCE g_hInst ;
	HHOOK g_hMouse ;
	HHOOK g_hKeyb ;
	HWND  g_hwndPointNow ;
	HWND  hwndClient ;
	HBITMAP hBitmap ;
	bool captured ;
} HookData, *PHookData;

class MouseHook
{
public:
	MouseHook(void);
	MouseHook(HINSTANCE hInstance) ;
	~MouseHook(void);

	HookData hookData ;
	void SetHook () ;
	void UnHook () ;
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam ) ;
	LRESULT CALLBACK MouseMsgHandler(int nCode, WPARAM wParam, LPARAM lParam ) ;
	static LRESULT CALLBACK KeybdProc(int nCode, WPARAM wParam, LPARAM lParam) ;
	DWORD WINAPI MouseHookThreadFunction( LPVOID lpParam ) ;
};