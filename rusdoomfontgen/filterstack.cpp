#include "stdafx.h"
#include "filterstack.h"
#include "Resource.h"
#include "translation.h"

FilterStack *curfilterstack = 0;

FilterStack::FilterStack(void)
{
	filterstype["Border"] = 0;
	filterstype["Threshold"] = 1;
	filterstype["Import"] = 2;
	filterstype["Export"] = 3;

	filtersconditions["always"] = 0;
	filtersconditions["never"] = 1;
	filtersconditions["char"] = 2;
	filtersconditions["dight"] = 3;
	filtersconditions["lower char"] = 4;
	filtersconditions["upper char"] = 5;
	filtersconditions["odd dight"] = 6;
	filtersconditions["even dight"] = 7;
	filtersconditions["sign"] = 8;
	filtersconditions["in"] = 9;
}
FilterStack::~FilterStack(void)
{

}

void FilterStack::RemakeList(void)
{
	SendMessage(filterslist,LB_RESETCONTENT,0,0);
	for (std::list<Filter*>::iterator i = filters.begin(); i != filters.end(); i++)
		SendMessageA(filterslist,LB_ADDSTRING,0,(LPARAM) (*i)->GetFilterName().c_str());
}
void FilterStack::AddFilter(int filtertype)
{
	Filter *f;
	switch(filtertype)
	{
	case 0:
		f = new BorderFilter();
		filters.push_back(f);
		break;
	case 1:
		f = new ThresholdFilter();
		filters.push_back(f);
		break;
	case 2:
		f = new ImportFilter();
		filters.push_back(f);
		break;
	case 3:
		f = new ExportFilter();
		filters.push_back(f);
		break;
	default:
		return;
	}
	RemakeList();
}
void FilterStack::RemoveFilter(int pos)
{
	std::list<Filter*>::iterator r = filters.begin();
	while(pos-- > 0)
	{
		r++;
	}
	filters.erase(r);
	RemakeList();
}

void FilterStack::ChangeCondition(int pos, int type, std::string value)
{
	std::list<Filter*>::iterator r = filters.begin();
	while(pos-- > 0)
	{
		r++;
	}
	(*r)->SetCondition(type,value);
}
void FilterStack::LoadCondition(int pos)
{
	std::list<Filter*>::iterator r = filters.begin();
	while(pos-- > 0)
	{
		r++;
	}
	std::string s1 = (*r)->GetConditionVal();
	int i = (*r)->GetCondition();

	std::unordered_map<std::string,int>::iterator rr = filtersconditions.begin();
	while(rr->second != i)
	{
		rr++;
		if (rr == filtersconditions.end()) return;
	}
	int index = SendMessageA(filtersconditionselect, CB_FINDSTRING,-1,(LPARAM) rr->first.c_str());
	SendMessageA(filtersconditionselect,CB_SETCURSEL,index,0);
	SetWindowTextA(filtersconditionvalue,s1.c_str());
}


void FilterStack::FilterOptions(int pos)
{
	std::list<Filter*>::iterator r = filters.begin();
	while(pos-- > 0)
	{
		r++;
	}
	if (!((*r)->ShowDialog()))
	{
		bool hasdlg = (*r)->RegisterDialogClass(curfilterstack->winclass.hInstance,curfilterstack->wnd);
		if (hasdlg) (*r)->ShowDialog();
	}
}

bool FilterStack::UpFilter(int pos)
{
	if (pos == 0) return 0;
	std::list<Filter*>::iterator r = filters.begin();
	std::list<Filter*>::iterator prev;
	while(pos-- > 0)
	{
		prev = r;
		r++;
	}
	filters.insert(prev,(*r));
	filters.erase(r);
	RemakeList();
	return 1;
}
bool FilterStack::DownFilter(int pos)
{
	int sz = filters.size()-1;
	if (pos >= sz) return 0;
	std::list<Filter*>::iterator r = filters.begin();
	while(pos-- > 0)
	{
		r++;
	}
	std::list<Filter*>::iterator prev = r++;
	filters.insert(prev,(*r));
	filters.erase(r);
	RemakeList();
	return 1;
}

void FilterStack::ShowFilterStackWindow(void)
{
	if (!registered) return;
	if (!wnd) CreateWindowInstance();
	if (!wnd) return;
	curfilterstack = this;
	ShowWindow(wnd,1);
	EnableWindow(parent,FALSE);
}
bool FilterStack::RegisterDialogClass(HINSTANCE hinst, HWND parentwnd)
{
	if (registered) return true;
	winclass.cbSize = sizeof(WNDCLASSEXW);
	winclass.style			= CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= &(FilterStack::FiltersWndProc);
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinst;
	winclass.hIcon			= LoadIcon(hinst, MAKEINTRESOURCE(IDI_RUSDOOMFONTGEN));
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)GetSysColorBrush(COLOR_3DFACE);
	winclass.lpszMenuName	= 0;
	winclass.lpszClassName	= L"FiltersClassDialog";
	winclass.hIconSm		= LoadIcon(winclass.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	registered = (RegisterClassExW(&winclass) != 0);
	parent = parentwnd;
	return registered;
}
LRESULT CALLBACK FilterStack::FiltersWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	int pos = -1;
	char buf[64];
	std::string s;
	switch (message)
	{
	case WM_CLOSE:
		SetForegroundWindow(parent);
		EnableWindow(parent,TRUE);
		ShowWindow(wnd,0);
		return 0;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		if ((wmEvent == BN_CLICKED) && (lParam == (LPARAM)filtersaadd))
		{
			GetWindowTextA(filterstypes,buf,64);
			curfilterstack->AddFilter(curfilterstack->filterstype[buf]);
		}

		if ((wmEvent == BN_CLICKED) && (lParam == (LPARAM)filtersok))
		{
			SendMessageA(hWnd,WM_CLOSE,0,0);
			break;
		}

		if ((wmEvent == BN_CLICKED) && (lParam == (LPARAM)filtersadelete))
		{
			pos = SendMessageA(filterslist,LB_GETCURSEL,0,0);
			if (pos != -1)
				curfilterstack->RemoveFilter(pos);
			break;
		}

		if ((wmEvent == BN_CLICKED) && (lParam == (LPARAM)upbutton))
		{
			pos = SendMessageA(filterslist,LB_GETCURSEL,0,0);
			if (pos != -1)
				if (curfilterstack->UpFilter(pos))
					SendMessageA(filterslist,LB_SETCURSEL,pos-1,0);
			break;
		}

		if ((wmEvent == BN_CLICKED) && (lParam == (LPARAM)downbutton))
		{
			pos = SendMessageA(filterslist,LB_GETCURSEL,0,0);
			if (pos != -1)
				if (curfilterstack->DownFilter(pos))
					SendMessageA(filterslist,LB_SETCURSEL,pos+1,0);
			break;
		}


		if ((wmEvent == BN_CLICKED) && (lParam == (LPARAM)filterschangeconditionbutton))
		{
			pos = SendMessageA(filterslist,LB_GETCURSEL,0,0);
			if (pos != -1)
			{
				GetWindowTextA(filtersconditionvalue,buf,64);
				s = buf;
				GetWindowTextA(filtersconditionselect,buf,64);
				curfilterstack->ChangeCondition(pos,filtersconditions[std::string(buf)],s);
			}
			break;
		}



		if ((wmEvent == LBN_DBLCLK) && (lParam == (LPARAM)filterslist))
		{
			curfilterstack->FilterOptions(SendMessageW(filterslist,LB_GETCURSEL,0,0));
		}

		if ((wmEvent == LBN_SELCHANGE) && (lParam == (LPARAM)filterslist))
		{
			curfilterstack->LoadCondition(SendMessageW(filterslist,LB_GETCURSEL,0,0));
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void FilterStack::CreateWindowInstance(void)
{
	wnd = CreateWindowExW(0,L"FiltersClassDialog", t::filterswindow.c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, 0, 300, 432, parent, NULL, winclass.hInstance, NULL);
//	PrintLastError();

	HFONT fnt = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	filterstypes  =CreateWindowExW(0,L"COMBOBOX",0,WS_CHILD | CBS_DROPDOWN | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_OVERLAPPED | WS_VSCROLL | WS_VISIBLE,25,24,250,500,wnd,0,0,0);
	SendMessage(filterstypes, WM_SETFONT, WPARAM (fnt), TRUE);

	filtersaadd  = CreateWindowExW(0,L"BUTTON",t::filterswindowaddfilter.c_str(),WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,25,56,125,24,wnd,0,0,0);
	SendMessage(filtersaadd, WM_SETFONT, WPARAM (fnt), TRUE);

	filtersadelete  = CreateWindowExW(0,L"BUTTON",t::filterswindowdeletefilter.c_str(),WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,150,56,125,24,wnd,0,0,0);
	SendMessage(filtersadelete, WM_SETFONT, WPARAM (fnt), TRUE);

	filterslist = CreateWindowExW(0,L"LISTBOX",0,WS_CHILD | LBS_NOTIFY | WS_BORDER | WS_VSCROLL | WS_VISIBLE,25,88,250,120,wnd,0,0,0);
	SendMessage(filterslist, WM_SETFONT, WPARAM (fnt), TRUE);

	upbutton  = CreateWindowExW(0,L"BUTTON",t::filterswindowupfilter.c_str(),WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,25,272,125,24,wnd,0,0,0);
	SendMessage(upbutton, WM_SETFONT, WPARAM (fnt), TRUE);

	downbutton  = CreateWindowExW(0,L"BUTTON",t::filterswindowdownfilter.c_str(),WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,150,272,125,24,wnd,0,0,0);
	SendMessage(downbutton, WM_SETFONT, WPARAM (fnt), TRUE);

	filtersok  = CreateWindowExW(0,L"BUTTON",t::universalbuttonok.c_str(),WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,25,304,250,24,wnd,0,0,0);
	SendMessage(filtersok, WM_SETFONT, WPARAM (fnt), TRUE);

	filtersconditionselect = CreateWindowExW(0,L"COMBOBOX",0,WS_CHILD | CBS_DROPDOWN | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_OVERLAPPED | WS_VSCROLL | WS_VISIBLE,25,208,100,500,wnd,0,0,0);
	SendMessage(filtersconditionselect, WM_SETFONT, WPARAM (fnt), TRUE);
	
	filtersconditionvalue = CreateWindowExW(WS_EX_CLIENTEDGE,L"EDIT",0,WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,125,208,150,24,wnd,0,winclass.hInstance,0);
	SendMessage(filtersconditionvalue, WM_SETFONT, WPARAM (fnt), TRUE);

	filterschangeconditionbutton = CreateWindowExW(0,L"BUTTON",t::filtersapplycondition.c_str(),WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,25,240,250,24,wnd,0,0,0);
	SendMessage(filterschangeconditionbutton, WM_SETFONT, WPARAM (fnt), TRUE);

	for (std::unordered_map<std::string, int>::iterator i = filterstype.begin(); i != filterstype.end() ; i++)
		SendMessageA(filterstypes, CB_ADDSTRING,0,(LPARAM)i->first.c_str());
	SendMessageA(filterstypes,CB_SETCURSEL,0,0);

	for (std::unordered_map<std::string, int>::iterator i = filtersconditions.begin(); i != filtersconditions.end() ; i++)
		SendMessageA(filtersconditionselect, CB_ADDSTRING,0,(LPARAM)i->first.c_str());
	SendMessageA(filtersconditionselect,CB_SETCURSEL,0,0);

	SendMessageA(filtersconditionvalue,EM_SETLIMITTEXT,256,0);
}

void FilterStack::Process(FontImage **img, int count)
{
	for (std::list<Filter*>::iterator i = filters.begin(); i != filters.end(); i++)
	{
		(*i)->OnBeforeAll();
		for (int j = 0; j<count; j++)
		{
		if ((*i)->CheckCondition(img[j]->GetCharRepresentation()))
			(*i)->Apply(*img[j]);
		}
		(*i)->OnAfterAll();
	}
}

WNDCLASSEXW FilterStack::winclass;

HWND FilterStack::filtersaadd = 0;
HWND FilterStack::filtersadelete = 0;

HWND FilterStack::filterstypes = 0;

HWND FilterStack::wnd = 0;
HWND FilterStack::filterslist = 0;

HWND FilterStack::upbutton = 0;
HWND FilterStack::downbutton = 0;

HWND FilterStack::filtersok = 0;

HWND FilterStack::parent = 0;

bool FilterStack::registered = 0;

HWND FilterStack::filtersconditionselect;
HWND FilterStack::filtersconditionvalue;

HWND FilterStack::filterschangeconditionbutton = 0;

std::unordered_map<std::string, int> FilterStack::filterstype;
std::unordered_map<std::string, int> FilterStack::filtersconditions;