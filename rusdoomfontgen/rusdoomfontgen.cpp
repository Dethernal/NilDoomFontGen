// rusdoomfontgen.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "rusdoomfontgen.h"
#include "bigfont.h"
#include "confont.h"
#include "fontimage.h"
#include <unordered_map>
#include "filterstack.h"
#include "translation.h"

const std::string defaultchars(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~������������������������������������������������������������������");

FilterStack* filters = 0;

std::string chars;

std::unordered_map<std::string,int> weightmap;

std::unordered_map<std::string,int> codepages;

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����

HWND mainwindow;
HWND fontselect;
HWND heigthselect;
HWND weigthselect;
HWND kerningselect;
HWND charsetselect;
HWND italicselect;

HWND Processbutton;
HWND Filtersbutton;

BigFont *curfont = 0;

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	// ������������� ���������� �����
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RUSDOOMFONTGEN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HFONT fnt = CreateFont(24,0,0,0,FW_HEAVY,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,VARIABLE_PITCH | FF_ROMAN, 0);

	HDC wnddc = GetDC(mainwindow);

	FILE *f = fopen("chars.txt","rb");
	char c = 0;
	if (f)
	{
		while ((c = getc(f)) != EOF)
			chars.push_back(c);

	} else chars = defaultchars;
	curfont = new BigFont(wnddc,fnt,chars);
	curfont->ExportToLMP("dbigfont.fon2");

	ReleaseDC(mainwindow,wnddc);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RUSDOOMFONTGEN));

	InvalidateRect(mainwindow,0,0);

	// ���� ��������� ���������:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete curfont;
	return (int) msg.wParam;
}



//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
//  �����������:
//
//    ��� ������� � �� ������������� ���������� ������ � ������, ���� �����, ����� ������ ���
//    ��� ��������� � ��������� Win32, �� �������� ������� RegisterClassEx'
//    ������� ���� ��������� � Windows 95. ����� ���� ������� ����� ��� ����,
//    ����� ���������� �������� "������������" ������ ������ � ���������� �����
//    � ����.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RUSDOOMFONTGEN));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= L"PRORAMCLASSNAME1";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//

int CALLBACK EnumFontFamExProcW(const LOGFONTW *lpelfe,const TEXTMETRICW *lpntme,DWORD FontType,LPARAM lParam)
{
	
	SendMessageW(fontselect,CB_ADDSTRING,0,(LPARAM)lpelfe->lfFaceName);
	return 1;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

   hWnd = CreateWindowW(L"PRORAMCLASSNAME1", t::programname.c_str(), WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   RECT r;

   GetClientRect(hWnd, &r);

   fontselect = CreateWindowExW(0,L"COMBOBOX",0,WS_CHILD | CBS_DROPDOWN | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_OVERLAPPED | WS_VSCROLL ,r.right - 200,24,180,500,hWnd,0,hInst,0);
   LOGFONTW logfont;
   memset(&logfont,0,sizeof(LOGFONTW));
   HDC tmpDC = GetDC(0);
   EnumFontFamiliesExW(tmpDC, &logfont,&EnumFontFamExProcW,0,0);
   ReleaseDC(0,tmpDC);
   heigthselect = CreateWindowExW(WS_EX_CLIENTEDGE,L"EDIT",L"24",WS_CHILD | WS_OVERLAPPED | WS_BORDER,r.right - 200,56,180,24,hWnd,0,hInst,0);
   weigthselect =CreateWindowExW(0,L"COMBOBOX",0,WS_CHILD | CBS_DROPDOWN | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_OVERLAPPED | WS_VSCROLL,r.right - 200,88,180,500,hWnd,0,hInst,0);
   kerningselect = CreateWindowExW(WS_EX_CLIENTEDGE,L"EDIT",L"0",WS_CHILD | WS_OVERLAPPED | WS_BORDER,r.right - 200,120,180,24,hWnd,0,hInst,0);
   italicselect = CreateWindowExW(0,L"BUTTON",0,WS_CHILD | WS_OVERLAPPED |  BS_AUTOCHECKBOX,r.right - 200,152,180,24,hWnd,0,hInst,0);
   charsetselect = CreateWindowExW(0,L"COMBOBOX",0,WS_CHILD | CBS_DROPDOWN | CBS_HASSTRINGS | WS_OVERLAPPED | WS_VSCROLL,r.right - 200,184,180,500,hWnd,0,hInst,0);
   Filtersbutton = CreateWindowExW(0,L"BUTTON",t::strfiltersselect.c_str(),WS_CHILD | WS_OVERLAPPED,r.right - 200,216,180,24,hWnd,0,hInst,0);
   Processbutton = CreateWindowExW(0,L"BUTTON",t::strcreatefont.c_str(),WS_CHILD | WS_OVERLAPPED,r.right - 200,248,180,24,hWnd,0,hInst,0);
   //SendMessageW(fontselect,LB_SETCOUNT,100,0);
   //MakeDragList(fontselect);

	weightmap["Don't Care (0)"] = 0;
	weightmap["Thiny (100)"] = 100;
	weightmap["Ultra light (200)"] = 200;
	weightmap["Light (300)"] = 300;
	weightmap["Normal (400)"] = 400;
	weightmap["Medium (500)"] = 500;
	weightmap["Semi bold (600)"] = 600;
	weightmap["Bold (700)"] = 700;
	weightmap["Ultra Bold (800)"] = 800;
	weightmap["Heavy (900)"] = 900;

	for (std::unordered_map<std::string,int>::iterator r = weightmap.begin(); r!=weightmap.end();r++)
	{
		SendMessageA(weigthselect,CB_ADDSTRING,0,(LPARAM)r->first.c_str());
	}

	SendMessageA(weigthselect,CB_SETCURSEL,0,0);
	SendMessageA(fontselect,CB_SETCURSEL,0,0);

	codepages["ANSI_CHARSET"] = ANSI_CHARSET;
	codepages["BALTIC_CHARSET"] = BALTIC_CHARSET;
	codepages["DEFAULT_CHARSET"] = DEFAULT_CHARSET;
	codepages["EASTEUROPE_CHARSET"] = EASTEUROPE_CHARSET;
	codepages["GB2312_CHARSET"] = GB2312_CHARSET;
	codepages["GREEK_CHARSET"] = GREEK_CHARSET;
	codepages["HANGUL_CHARSET"] = HANGUL_CHARSET;
	codepages["MAC_CHARSET"] = MAC_CHARSET;
	codepages["OEM_CHARSET"] = OEM_CHARSET;
	codepages["RUSSIAN_CHARSET"] = RUSSIAN_CHARSET;
	codepages["SHIFTJIS_CHARSET"] = SHIFTJIS_CHARSET;
	codepages["SYMBOL_CHARSET"] = SYMBOL_CHARSET;
	codepages["TURKISH_CHARSET"] = TURKISH_CHARSET;

	for (std::unordered_map<std::string,int>::iterator r = codepages.begin(); r!=codepages.end();r++)
	{
		SendMessageA(charsetselect,CB_ADDSTRING,0,(LPARAM)r->first.c_str());
	}
	SendMessageA(charsetselect,CB_SETCURSEL,0,0);

   ShowWindow(fontselect,1);
   ShowWindow(heigthselect,1);
   ShowWindow(weigthselect,1);
   ShowWindow(kerningselect,1);
   ShowWindow(italicselect,1);
   ShowWindow(charsetselect,1);
   ShowWindow(Processbutton,1);
   ShowWindow(Filtersbutton,1);

   if (!hWnd)
   {
      return FALSE;
   }
   mainwindow = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND	- ��������� ���� ����������
//  WM_PAINT	-��������� ������� ����
//  WM_DESTROY	 - ������ ��������� � ������ � ���������.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	char buf[66];
	RECT r;
	bool b;
	int kerning;
	int fweigth;
	int fheight;
	int count;
	std::unordered_map<std::string,int>::iterator itcp;
	int codepage;

	HDC tdc;
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:

		if ((wmEvent == BN_CLICKED) && (lParam == (LPARAM)Processbutton))
		{
			delete curfont;
			tdc = GetDC(hWnd);
			count = GetWindowTextA(heigthselect,buf,32);
			buf[32] = 0;
			fheight = atoi(buf);

			count = GetWindowTextA(charsetselect,buf,32);
			buf[32] = 0;
			itcp = codepages.find(std::string(buf));
			if (itcp != codepages.end())
			{
				codepage = itcp->second;
			}
			else
			{
				errno = 0;
				codepage = atoi(buf);
				if (errno)
					codepage = ANSI_CHARSET;
			}
			errno = 0;
			count = GetWindowTextA(weigthselect,buf,32);
			buf[32] = 0;
			fweigth = weightmap[std::string(buf)];

			GetWindowTextA(kerningselect,buf,32);
			kerning = atoi(buf);

			count = GetWindowTextW(fontselect,(wchar_t*)buf,32);
			buf[65] = 0;
			buf[64] = 0;


			b =  (SendMessageA(italicselect,BM_GETCHECK,0,0) == BST_CHECKED);
			HFONT f = CreateFontW(fheight,0,0,0,fweigth,b,0,0,codepage,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,0,(wchar_t*) buf);
			curfont = new BigFont(tdc,f,chars,filters,kerning);
			curfont->ExportToLMP("dbigfont.fon2");
			DeleteObject(f);
			ReleaseDC(hWnd,tdc);
			InvalidateRect(hWnd,0,1);
			return 0;
		}

		if ((wmEvent == BN_CLICKED) && (lParam == (LPARAM)Filtersbutton))
		{
			if (filters) 
				filters->ShowFilterStackWindow();
			else
			{
				filters = new FilterStack();
				filters->RegisterDialogClass(hInst,hWnd);
				filters->ShowFilterStackWindow();
			}
			return 0;
		}

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (curfont)
		{
			GetClientRect(hWnd,&r);
			curfont->DrawPreviewTo(hdc, r.left, r.top, SRCCOPY);
			TextOutW(hdc,r.right - 350, 26,t::strtypeface.data(),t::strtypeface.length());
			TextOutW(hdc,r.right - 350, 58, t::strheight.data(),t::strheight.length());
			TextOutW(hdc,r.right - 350, 90, t::strweight.data(),t::strweight.length());
			TextOutW(hdc,r.right - 350, 122, t::stroffset.data(),t::stroffset.length());

			TextOutW(hdc,r.right - 350, 154, t::stritalic.data(),t::stritalic.length());
			TextOutW(hdc,r.right - 350, 186, t::strcodepage.data(),t::strcodepage.length());

		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���������� ��������� ��� ���� "� ���������".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
