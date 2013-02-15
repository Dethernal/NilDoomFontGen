#pragma once

#ifndef __FILTERSTACK_H__
#define __FILTERSTACK_H__

#include "stdafx.h"
#include "fontimage.h"
#include "borderfilter.h"
#include "thresholdfilter.h"
#include "filter.h"
#include <list>
#include <unordered_map>


class FilterStack
{
public:
	FilterStack(void);
	~FilterStack(void);
	void ShowFilterStackWindow(void);
	void Process(FontImage &img);
	bool RegisterDialogClass(HINSTANCE hinst, HWND parentwnd);
private:
	void RemakeList(void);
	void AddFilter(int filtertype);
	void RemoveFilter(int pos);
	void FilterOptions(int pos);
	bool UpFilter(int pos);
	bool DownFilter(int pos);

	void ChangeCondition(int pos, int type, std::string value);
	void LoadCondition(int pos);

	static LRESULT CALLBACK FiltersWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	std::list<Filter*> filters;
	
	static std::unordered_map<std::string, int> filterstype;
	static std::unordered_map<std::string, int> filtersconditions;
	static bool registered;

	void CreateWindowInstance(void);
	static	WNDCLASSEXW winclass;

	static HWND filtersaadd;
	static HWND filtersadelete;

	static HWND parent;

	static HWND filterstypes;

	static HWND wnd;
	static HWND filterslist;

	static HWND upbutton;
	static HWND downbutton;

	static HWND filtersok;
	static HWND filterschangeconditionbutton;


	static HWND filtersconditionselect;
	static HWND filtersconditionvalue;

};

#endif