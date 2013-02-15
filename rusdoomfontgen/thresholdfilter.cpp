#include "stdafx.h"
#include "thresholdfilter.h"
#include "Resource.h"
#include <sstream>
#include <math.h>
#include "translation.h"
#include <windowsx.h>
static ThresholdFilter *current = 0;

HWND ThresholdFilter::wnd = 0;

WNDCLASSEXW ThresholdFilter::winclass;

HWND ThresholdFilter::okbutton = 0;

bool ThresholdFilter::registered = 0;

HWND ThresholdFilter::parent = 0;

HWND ThresholdFilter::blackmark = 0;
HWND ThresholdFilter::whitemark = 0;

ThresholdFilter::ThresholdFilter(void) : Filter("Threshold",0), blackindex(0), whiteindex(31)
{
	
}

ThresholdFilter::ThresholdFilter(std::string StringRep) : Filter("Threshold",0) 
{
	std::stringstream ss(StringRep);
	ss >> blackindex >> whiteindex;
}

ThresholdFilter::~ThresholdFilter(void)
{

}

std::string ThresholdFilter::FilterToText(void)
{
	std::stringstream ss;
	ss << blackindex << whiteindex;
	std::string s = ss.str();
	return s;
}

void ThresholdFilter::Apply(FontImage &image) 
{
	int len = image.GetHeigth() * image.GetWidth();

	unsigned char oldc, newc;
	for (int i =0; i<len;i++)
	{
		oldc = GetColor(image,i,0);
		if (oldc == 0) continue;
		oldc = 111 - oldc;
		if (oldc<=blackindex) 
			newc = 0;
		else if(oldc>=whiteindex)
			newc = 80;
		else
			newc = 111 - oldc;
		SetColor(image,i,0,newc);
	}

}

bool ThresholdFilter::ShowDialog(void)
{
	if (!registered) return false;
	if (!wnd) CreateWindowInstance();

	ShowWindow(wnd,1);
	EnableWindow(parent,0);
	current = this;
	return true;
}

int blackmarkmove = 0;
int whitemarkmove = 0;

int markstart = 0;
int mousestart = 0;

LRESULT CALLBACK ThresholdFilter::SlideBarWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	short int t;
	char buf[32];
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		if (hWnd == blackmark)
		{
			SetCapture(blackmark);
			blackmarkmove = 1;
			markstart = current->blackindex;
		} else
		{
			SetCapture(whitemark);
			whitemarkmove = 1;
			markstart = current->whiteindex;
		}
		mousestart = LOWORD(lParam);
		break;
	case WM_LBUTTONUP:
		if (hWnd == blackmark)
		{
			ReleaseCapture();
			blackmarkmove = 0;
			t = LOWORD(lParam);
			current->blackindex = current->blackindex + ((t- mousestart) / 7);
			if (current->blackindex > current->whiteindex) current->blackindex = current->whiteindex;
			if (current->blackindex < 0) current->blackindex = 0;
			if (current->blackindex > 31) current->blackindex = 31;
			SetWindowPos(blackmark,(HWND)0,34+current->blackindex*7,32,0,0,SWP_NOSIZE);
		} else
		{
			ReleaseCapture();
			t = LOWORD(lParam);
			whitemarkmove = 0;
			current->whiteindex  = current->whiteindex + ((t- mousestart) / 7);
			if (current->whiteindex < current->blackindex) current->whiteindex = current->blackindex;
			if (current->whiteindex < 0) current->whiteindex = 0;
			if (current->whiteindex > 31) current->whiteindex = 31;
			SetWindowPos(whitemark,(HWND)0,34+current->whiteindex*7,15,0,0,SWP_NOSIZE);

		}
		InvalidateRect(wnd,0,0);
		break;
	case WM_MOUSEMOVE:
		{
			if (blackmarkmove)
			{
	//			t = GET_X_LPARAM(lParam);
	//			SetWindowPos(blackmark,(HWND)0,34+current->blackindex*7 + ((t - mousestart)),32,0,0,SWP_NOSIZE);
			} 
			else if (whitemarkmove)
			{
	//			t = LOWORD(lParam);
	//			SetWindowPos(whitemark,(HWND)0,34+current->whiteindex*7 + ((t - mousestart)),32,0,0,SWP_NOSIZE);
			}
	//		InvalidateRect(wnd,0,0);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
	//TODO Здесь

		EndPaint(hWnd, &ps);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



LRESULT CALLBACK ThresholdFilter::FilterWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT r;
	HBRUSH blackbrush;
	HBRUSH whitebrush; 
	HBRUSH curbrush;

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
			SendMessage(hWnd,WM_CLOSE,0,0);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		blackbrush = CreateSolidBrush(RGB(0,0,0));
		whitebrush = CreateSolidBrush(RGB(0xe0,0xe0,0xe0));

	for (int i =0; i<32; i++)
	{
		r.left = 38+ 7*i;
		r.right = 45 + 7*i;
		r.top = 30;
		r.bottom = 37;
		if (i < current->blackindex)
			FillRect(hdc,&r,blackbrush);
		else if (i >= current->whiteindex)
			FillRect(hdc,&r,whitebrush);
		else
		{
			curbrush = CreateSolidBrush(RGB(7+7*i,7+7*i,7+7*i));
			FillRect(hdc,&r,curbrush);
			DeleteObject(curbrush);
		}
	}
	DeleteObject(blackbrush);
	DeleteObject(whitebrush);
		
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

static WNDCLASSEXW SlideBarClass;

bool ThresholdFilter::RegisterDialogClass(HINSTANCE hinst, HWND parentwnd)
{
	if (registered) return true;


	
	SlideBarClass.cbSize = sizeof(WNDCLASSEX);
	SlideBarClass.style			= CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
	SlideBarClass.lpfnWndProc	= &(ThresholdFilter::SlideBarWndProc);
	SlideBarClass.cbClsExtra		= 0;
	SlideBarClass.cbWndExtra		= 0;
	SlideBarClass.hInstance		= hinst;
	SlideBarClass.hIcon			= 0;
	SlideBarClass.hCursor		= LoadCursor(NULL, IDC_HAND);
	SlideBarClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	SlideBarClass.lpszMenuName	= 0;
	SlideBarClass.lpszClassName	= L"SlidePointer";
	SlideBarClass.hIconSm		= 0;

	RegisterClassExW(&SlideBarClass);

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style			= CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= &(ThresholdFilter::FilterWndProc);
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinst;
	winclass.hIcon			= LoadIcon(hinst, MAKEINTRESOURCE(IDI_RUSDOOMFONTGEN));
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	winclass.lpszMenuName	= 0;
	winclass.lpszClassName	= L"ThresholdFilterDialog";
	winclass.hIconSm		= LoadIcon(winclass.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	parent = parentwnd;
	registered = (RegisterClassExW(&winclass) != 0);
	return registered;
}

const POINT whiteregionpoints[5] = {{0,0},{14,0},{14,14},{7,18},{0,14}};
const POINT blackregionpoints[5] = {{0,7},{7,0},{14,7},{14,18},{0,18}};

void ThresholdFilter::CreateWindowInstance(void)
{

	wnd = CreateWindowExW(0,L"ThresholdFilterDialog", t::thresholdfilterwnd.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 300, 140, NULL, NULL, winclass.hInstance, NULL);

	blackmark = CreateWindowExW(0,L"SlidePointer", 0, WS_OVERLAPPED | WS_CHILD,
		34, 32, 14, 18, wnd, NULL, winclass.hInstance, NULL);

	whitemark = CreateWindowExW(0,L"SlidePointer", 0, WS_OVERLAPPED | WS_CHILD,
		251, 15, 14, 18, wnd, NULL, winclass.hInstance, NULL);


	HRGN blackregion = CreatePolygonRgn(blackregionpoints, 5, ALTERNATE);
	HRGN whiteregion = CreatePolygonRgn(whiteregionpoints, 5, ALTERNATE);

	SetWindowRgn(blackmark,blackregion,1);
	SetWindowRgn(whitemark,whiteregion,1);

	ShowWindow(blackmark,1);
	ShowWindow(whitemark,1);

	okbutton    = CreateWindowExW(0,L"BUTTON",t::universalbuttonok.c_str(),WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,25,60,250,24,wnd,0,0,0);

}