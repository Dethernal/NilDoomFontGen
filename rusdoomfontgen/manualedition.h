#pragma once

#ifndef __MANUALEDITION_H__
#define __MANUALEDITION_H__

#include "stdafx.h"
#include "filter.h"

class ManualEditFilter : public Filter
{
public:
	ManualEditFilter(void);
	ManualEditFilter(std::string StringRep);
	~ManualEditFilter(void);
	std::string FilterToText(void);
	void Apply(FontImage &image);
	bool ShowDialog(void);
	bool RegisterDialogClass(HINSTANCE hinst, HWND parentwnd);
private:
	std::string s;
}

#endif