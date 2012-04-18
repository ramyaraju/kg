#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <Commctrl.h>
#include "resource.h"	// resources
#include "libs/music.h"	// mod to play
#include "libs/ufmod.h" // lib to play the mod
#include "keygens.h"	// keygen classes

#pragma comment(lib, "ufmod.lib")
#pragma comment(lib, "winmm.lib")

INT_PTR CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;
PCKeygener kg;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// initialise once, for randomisation purposes
	kg = new happy_kgme_8;
	if (kg == NULL)
	{
		MessageBox(NULL, "Memory allocation error! :(", "Error!", MB_ICONERROR);
		return 0;
	}
	if (kg->init() == false)
	{
		MessageBox(NULL, "Error initialising instance! :(", "Error!", MB_ICONERROR);
		return 0;
	}
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG), NULL, (DLGPROC)WndProc, NULL);
	delete kg;
	return 0;
}

// generate serial for a given name and public key
char * GenerateSerial(char *name)
{
	string ser;
	if ((kg->GenerateSerial(name, ser)) == false)
	{
		return NULL;
	}
	int len = ser.length();
	char *serial = new char[len+1];
	strcpy_s(serial, len+1, ser.c_str());
	return serial;
}

void DrawBorder(HWND hWnd, int IDENT, PAINTSTRUCT ps)
{
	POINT pt;
	RECT rc;

	GetWindowRect(GetDlgItem(hWnd, IDENT), &rc);
	pt.x = rc.left;
	pt.y = rc.top;
	ScreenToClient(hWnd, &pt);
	rc.left = pt.x;
	rc.top = pt.y;
	pt.x = rc.right;
	pt.y = rc.bottom;
	ScreenToClient(hWnd, &pt);
	rc.right = pt.x;
	rc.bottom = pt.y;
	Rectangle(ps.hdc, rc.left-1,rc.top-1,rc.right+1,rc.bottom+1);

	return;
}

INT_PTR CALLBACK WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	PAINTSTRUCT ps;
	HFONT hFont;
	DWORD lpnSize = 255;
	static HBRUSH hBrush = NULL;
	static HBRUSH hBrush2 = NULL;
	static int nWidth = 326;
	static int nHeight = 109;

	switch (message)
	{
	case WM_INITDIALOG:
		{
			// Create font to be used in keymaker
			hFont = CreateFont(12,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_DONTCARE,TEXT("Verdana"));

			// Retrieve and set the text of the static.
			SetDlgItemText(hDlg, IDC_STATIC, (kg->GetName()).c_str());

			// Set the fonts of the components
			SendDlgItemMessage(hDlg, IDC_NAME, WM_SETFONT, (WPARAM)hFont, TRUE);
			SendDlgItemMessage(hDlg, IDC_SERIAL, WM_SETFONT, (WPARAM)hFont, TRUE);
			SendDlgItemMessage(hDlg, IDC_COMP, WM_SETFONT, (WPARAM)hFont, TRUE);
			SendDlgItemMessage(hDlg, IDC_BUTTON1, WM_SETFONT, (WPARAM)hFont, TRUE);
			SendDlgItemMessage(hDlg, IDC_STATIC, WM_SETFONT, (WPARAM)hFont, TRUE);
			SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)hFont, TRUE);

			// Start playing music
			uFMOD_PlaySong(music, (void *)music_size, XM_MEMORY);

			// Create brushes to draw
			hBrush = CreateSolidBrush(RGB(72, 146, 189));
			hBrush2 = CreateSolidBrush(RGB(162, 199, 221));
			
			// Generate initial serial
			SetDlgItemText(hDlg, IDC_NAME, "tamaroth");
			char *serial = GenerateSerial("tamaroth");
			if (serial == NULL)
				SetDlgItemText(hDlg, IDC_SERIAL, "Something went wrong.");
			SetDlgItemText(hDlg, IDC_SERIAL, serial);
			delete [] serial;

			return (INT_PTR)TRUE;
		}

	case WM_LBUTTONDOWN:
		SendMessage(hDlg, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		break;

	case WM_CTLCOLORDLG:
		return (INT_PTR)hBrush;

	case WM_CTLCOLORSTATIC:
		SetTextColor((HDC)wParam, NULL);
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (INT_PTR) hBrush;

	case WM_CTLCOLOREDIT:
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (INT_PTR) hBrush2;

	case WM_NCPAINT:
		BeginPaint(hDlg, &ps);
		DrawBorder(hDlg, IDC_NAME, ps);
		DrawBorder(hDlg, IDC_SERIAL, ps);
		DrawBorder(hDlg, IDC_COMP, ps);
		EndPaint(hDlg, &ps);
		return (INT_PTR)0;

	case WM_COMMAND:
		if ((LOWORD(wParam) == IDC_BUTTON1) || (LOWORD(wParam) == IDOK))
		{
			char name[255];
			GetDlgItemText(hDlg, IDC_NAME, name, 255);
			char *serial = GenerateSerial(name);
			if (serial == NULL)
				SetDlgItemText(hDlg, IDC_SERIAL, "Something went wrong.");
			SetDlgItemText(hDlg, IDC_SERIAL, serial);
			delete [] serial;
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
