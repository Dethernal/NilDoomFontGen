#pragma once

#ifndef __FONTIMAGE_H__
#define __FONTIMAGE_H__

#include "stdafx.h"
#include "pcx.h"
#include "filter.h"

enum RLEMode {RLE_raw, RLE_lines};

class FontImage
{
public:
	FontImage(HDC screen, RECT* r ,char rchar = 0);
	FontImage(FontImage& source);
	~FontImage();
	void DrawBorder(int x1,int y1,int x2,int y2, unsigned char color);
	void CompressBright(int div, int offset);
	void SaveAsPCX(std::string path);
	void ReadFromPCX(std::string path);
	unsigned int GetHeigth() const; 
	unsigned int GetWidth() const;
	RLERep* GetRLERep(RLEMode mode);
	void DumpWithRLE(FILE * f);
	void DrawTo(HDC dc, int x, int y);
	char GetCharRepresentation(void);
private:
	friend class Filter;
	unsigned int heigth;
	unsigned int width;
	char representchar;
	unsigned char *data; //from top-left corner by rows
};

std::string SortChars(std::string str);
#endif