#pragma once

#ifndef __BORDERFILTER_H__
#define __BORDERFILTER_H__

#include "stdafx.h"
#include "fontimage.h"
#include "filter.h"

class BorderFilter : public Filter
{
public:
	BorderFilter(void);
	BorderFilter(std::string StringRep);
	~BorderFilter(void);
	std::string FilterToText(void);
	void Apply(FontImage &image);
	bool ShowDialog(void);
	bool RegisterDialogClass(HINSTANCE hinst, HWND parentwnd);
private:
	void DrawPreview(HDC dc, int x, int y);
	static void UpdateFromDialog(void);
	unsigned char CalcNewBright(FontImage & fi, int x, int y);
	static LRESULT CALLBACK FilterWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void CreateWindowInstance(void);
	static	WNDCLASSEXW winclass;
	static HWND wnd;

	static HWND input[5][5];

	static HDC resultdc;
	static HBITMAP resultbmp;

	static HWND okbutton;
	static HWND drawbutton;

	static HWND parent;

	static bool registered;

	float matrix[5][5];
};

#endif