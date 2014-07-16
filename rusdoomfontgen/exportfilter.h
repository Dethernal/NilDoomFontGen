#pragma once

#ifndef __EXPORTFILTER_H__
#define __EXPORTFILTER_H__

#include "stdafx.h"
#include "fontimage.h"
#include "filter.h"

class ExportFilter : public Filter
{
public:
	ExportFilter(void);
	ExportFilter(std::string StringRep);
	~ExportFilter(void);
	std::string FilterToText(void);
	void Apply(FontImage &image);
	bool ShowDialog(void);
	bool RegisterDialogClass(HINSTANCE hinst, HWND parentwnd);
	void OnAfterAll();
};

#endif