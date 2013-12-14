#pragma once
#include <windows.h>

typedef struct HookData
{    
	HINSTANCE  hInst ;
	HHOOK      hMouse ;
	HHOOK      hKeyb ;
	HWND       hwndPointNow ;	
} HookData, *PHookData ;

class MouseHook
{
public:
	MouseHook (void) ;
	MouseHook (HINSTANCE hInstance) ;
	~MouseHook(void) ;

	HookData hookData ;
	void SetHook () ;
	void UnHook () ;
	static LRESULT CALLBACK MouseProc (int nCode, WPARAM wParam, LPARAM lParam ) ;
	LRESULT CALLBACK MouseMsgHandler (int nCode, WPARAM wParam, LPARAM lParam ) ;
	static LRESULT CALLBACK KeybdProc (int nCode, WPARAM wParam, LPARAM lParam) ;
	DWORD WINAPI MouseHookThreadFunction ( LPVOID lpParam ) ;
};