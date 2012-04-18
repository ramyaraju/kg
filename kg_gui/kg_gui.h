#ifndef _KG_GUI_H_
#define _KG_GUI_H_

#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <Commctrl.h>

#include "resource.h"
#include "libs/music.h"
#include "libs/ufmod.h"
#include "keygens.h"

#pragma comment(lib, "ufmod.lib")
#pragma comment(lib, "winmm.lib")

INT_PTR	CALLBACK	WndProc (HWND, UINT, WPARAM, LPARAM);

HINSTANCE 	hInst;
PCKeygener	kg;

#endif // _KG_GUI_H_
