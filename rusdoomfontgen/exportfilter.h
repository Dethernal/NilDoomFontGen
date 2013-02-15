#pragma once

#ifndef __EXPORTFILTER_H__
#define __EXPORTFILTER_H__

#include "stdafx.h"
#include "fontimage.h"
#include "filter.h"

class ThresholdFilter : public Filter
{
public:
	ThresholdFilter(void);
	ThresholdFilter(std::string StringRep);
	~ThresholdFilter(void);
	std::string FilterToText(void);
	void Apply(FontImage &image);
	bool ShowDialog(void);
	bool RegisterDialogClass(HINSTANCE hinst, HWND parentwnd);
private:
	static LRESULT CALLBACK SlideBarWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK FilterWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void CreateWindowInstance(void);
	static	WNDCLASSEX winclass;
	static HWND wnd;

	static HWND blackmark;
	static HWND whitemark;

	static HWND okbutton;

	static HWND parent;

	static bool registered;

	int blackindex;
	int whiteindex;
};

#endif