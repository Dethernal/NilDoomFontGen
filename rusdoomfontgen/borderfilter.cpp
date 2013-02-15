#include "stdafx.h"
#include "borderfilter.h"
#include "Resource.h"
#include <sstream>
#include <math.h>
#include "translation.h"

static BorderFilter *current = 0;

HWND BorderFilter::wnd = 0;

WNDCLASSEXW BorderFilter::winclass;

HWND BorderFilter::input[5][5];

HDC BorderFilter::resultdc = 0;
HBITMAP BorderFilter::resultbmp = 0;

HWND BorderFilter::okbutton = 0;
HWND BorderFilter::drawbutton = 0;

bool BorderFilter::registered = 0;

HWND BorderFilter::parent = 0;

BorderFilter::BorderFilter(void) : Filter("Border",0)
{
	for (int j = 0; j < 5; j++)
		for (int i = 0; i < 5; i++)
				if ((i != 2) || (j!=2))
					matrix[j][i] = 0.0;
}

void BorderFilter::UpdateFromDialog(void)
{
	char buf[32];
	for (int j = 0; j < 5; j++)
		for (int i = 0; i < 5; i++)
		{
				if ((i != 2) || (j!=2))
				{
					GetWindowTextA(input[j][i],buf,32);
					current->matrix[j][i] = (float) atof(buf);
				}
		}
}

BorderFilter::BorderFilter(std::string StringRep) : Filter("Border",0) 
{
	std::stringstream ss(StringRep);
	for (int j = 0; j < 5; j++)
		for (int i = 0; i < 5; i++)
			if ((i != 2) || (j!=2))
				ss >> matrix[j][i];
}

BorderFilter::~BorderFilter(void)
{

}

std::string BorderFilter::FilterToText(void)
{
	std::stringstream ss;
	for (int j = 0; j < 5; j++)
		for (int i = 0; i < 5; i++)
			if ((i != 2) || (j!=2))
				ss << matrix[j][i] << " ";
	std::string s = ss.str();
	return s;
}

unsigned char BorderFilter::CalcNewBright(FontImage & fi, int x, int y)
{
	bool ishole = 0;
	float mass = 0;
	int newbright = 0;
	for (int j = -2; j < 3; j++)
	{
		for (int i = -2; i < 3; i++)
		{
			ishole = 0;
			if ((i != 0) || (j != 0))
			{
				if (x+i < 0) 
					ishole = 1;
				else if (x+i >= (int)fi.GetWidth()) 
					ishole = 1;
				else if (y+j >= (int)fi.GetHeigth()) 
					ishole = 1;
				else if (y+j < 0) ishole = 1;
				else if (GetColor(fi,x+i,y+j) == 0) 
					ishole = 1;
				
				if (ishole) 
					mass+=matrix[j+2][i+2];
			}
		}
	}
	//TODO: replace by SummBright()!
	int oldbright = GetColor(fi,x,y);
	newbright = oldbright - (int) floor(mass + 0.5);;
	if (newbright<80) newbright = 80;
	else if (newbright>111) newbright = 0;
	return newbright;
}

void BorderFilter::Apply(FontImage &image) 
{
	unsigned char n;
	FontImage Temp = image;
	for (int j = 0; j < (int)image.GetHeigth(); j++)
		for (int i = 0; i < (int)image.GetWidth(); i++)
		{
			if (GetColor(image,i,j) == 0) continue;
			n = CalcNewBright(Temp, i,j);
			SetColor(image,i,j, (unsigned char) n);
		}
}

bool BorderFilter::ShowDialog(void)
{
	char buf[32];
	if (!registered) return false;
	if (!wnd) CreateWindowInstance();

	for (int j = 0; j < 5; j++)
		for (int i = 0;i < 5; i++)
		{
			if ((i != 2) || (j != 2))
			{
				sprintf(buf,"%.3f",matrix[j][i]);
				SetWindowTextA(input[j][i],buf);
			}
		}

	ShowWindow(wnd,1);
	EnableWindow(parent,0);
	current = this;
	return true;
}

void BorderFilter::DrawPreview(HDC dc, int x, int y)
{
	HDC canvasDC = CreateCompatibleDC(dc);
	HBITMAP bmp = CreateCompatibleBitmap(dc,200,200);

	SelectObject(canvasDC,bmp);
	HBRUSH whitebrush = CreateSolidBrush(RGB(0xE0,0xE0,0xE0));
	HBRUSH blackbrush = CreateSolidBrush(RGB(0x00,0x00,0x00));

	HPEN whitepen = CreatePen(PS_SOLID,0,RGB(0xE0,0xE0,0xE0));
	HPEN blackpen = CreatePen(PS_SOLID,0,RGB(0x00,0x00,0x00));

	RECT r; r.left = 0; r.right = 200; r.top = 0; r.bottom = 200;

	FillRect(canvasDC,&r,blackbrush);

	SelectObject(canvasDC,whitepen);
	SelectObject(canvasDC,whitebrush);

	Ellipse(canvasDC,25,25,175,175);

	SelectObject(canvasDC,blackpen);
	SelectObject(canvasDC,blackbrush);

	Ellipse(canvasDC,38,70,98,130);
	Ellipse(canvasDC,101,70,161,130);
	//Ellipse(canvasDC,25,25,175,175);

	FontImage t1(canvasDC,&r);
	t1.CompressBright(8,111);
	Apply(t1);

	t1.DrawTo(dc,x,y);

	//BitBlt(dc,x,y,200,200,canvasDC,0,0,SRCCOPY);

	DeleteObject(blackpen);
	DeleteObject(whitepen);
	DeleteObject(whitebrush);
	DeleteObject(blackbrush);

	DeleteObject(bmp);
	DeleteObject(canvasDC);
}

LRESULT CALLBACK BorderFilter::FilterWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_CLOSE:
		SetForegroundWindow(parent);
		EnableWindow(parent,TRUE);
		ShowWindow(wnd,0);
		current = 0;
		return 0;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:

		if ((wmEvent == BN_CLICKED) && (lParam == (LPARAM)okbutton))
		{
			UpdateFromDialog();
			SendMessage(hWnd,WM_CLOSE,0,0);
			break;
		}

		if ((wmEvent == BN_CLICKED) && (lParam == (LPARAM)drawbutton))
		{
			UpdateFromDialog();
			InvalidateRect(wnd,0,0);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		current->DrawPreview(hdc,25,160);
		//BitBlt(hdc,25,160,200,200,resultdc,0,0,SRCCOPY);

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

bool BorderFilter::RegisterDialogClass(HINSTANCE hinst, HWND parentwnd)
{
	if (registered) return true;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style			= CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= &(BorderFilter::FilterWndProc);
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinst;
	winclass.hIcon			= LoadIcon(hinst, MAKEINTRESOURCE(IDI_RUSDOOMFONTGEN));
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	winclass.lpszMenuName	= 0;
	winclass.lpszClassName	= L"FilterBorderDialog";
	winclass.hIconSm		= LoadIcon(winclass.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	parent = parentwnd;
	registered = (RegisterClassExW(&winclass) != 0);
	return registered;
}

void BorderFilter::CreateWindowInstance(void)
{
	wnd = CreateWindowExW(0,L"FilterBorderDialog", t::borderfilterwnd.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 300, 500, NULL, NULL, winclass.hInstance, NULL);

	for (int j = 0; j < 5; j++)
		for (int i = 0; i< 5; i++)
			if((i != 2) || (j != 2)) input[j][i] = CreateWindowExW(WS_EX_CLIENTEDGE,L"EDIT",L"0.0",WS_CHILD | WS_OVERLAPPED | WS_BORDER | WS_VISIBLE,25+i * 50 ,25 + j * 32,50,24,wnd,0,winclass.hInstance,0);

	HDC tempDC = GetDC(wnd);
	resultdc = CreateCompatibleDC(tempDC);
	resultbmp = CreateCompatibleBitmap(tempDC,200,200);
	ReleaseDC(wnd,tempDC);

	drawbutton  = CreateWindowExW(0,L"BUTTON",t::borderfilterdraw.c_str(),WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,25,368,250,24,wnd,0,0,0);
	okbutton    = CreateWindowExW(0,L"BUTTON",t::universalbuttonok.c_str(),WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,25,400,250,24,wnd,0,0,0);

}