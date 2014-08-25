#pragma once

#ifndef __BIGFONT_H__
#define __BIGFONT_H__
#include "stdafx.h"
#include "fontimage.h"
#include "filterstack.h"
#include "font.h"

class BigFont : public Font
{
public:
	BigFont(HDC modeldc, HFONT fnt, std::string chars, FilterStack *filters = 0,int fkerning = 0);
	~BigFont();
	void ExportToLMP(std::string path);
};

#endif