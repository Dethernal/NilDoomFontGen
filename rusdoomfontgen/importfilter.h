#pragma once

#ifndef __IMPORTFILTER_H__
#define __IMPORTFILTER_H__

#include "stdafx.h"
#include "fontimage.h"
#include "filter.h"

class ImportFilter : public Filter
{
public:
	ImportFilter(void);
	ImportFilter(std::string StringRep);
	~ImportFilter(void);
	std::string FilterToText(void);
	void Apply(FontImage &image);
	bool ShowDialog(void);
	bool RegisterDialogClass(HINSTANCE hinst, HWND parentwnd);
};

#endif