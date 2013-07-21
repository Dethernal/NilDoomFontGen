#pragma once

#ifndef __BIGFONT_H__
#define __BIGFONT_H__
#include "stdafx.h"
#include "fontimage.h"
#include "filterstack.h"

class BigFont
{
public:
//	BigFont(HBITMAP bmp, std::string chars)
	BigFont(HDC modeldc, HFONT fnt, std::string chars, FilterStack *filters = 0,int fkerning = 0) ;
	~BigFont();
	void ExportToLMP(std::string path);
	void DrawPreviewTo(HDC dst, int x, int y, unsigned int mode);
	void DrawChar(char c, HDC dc, int x, int y);
private:
	std::string charset;
	FontImage** images;
	HDC imagedc;
	HBITMAP imagebmp;
	RECT imagerect;
	short int kerning;

};

#endif