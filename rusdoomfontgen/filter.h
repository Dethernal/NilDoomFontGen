#pragma once

#ifndef __FILTER_H__
#define __FILTER_H__

#include "stdafx.h"
#include "fontimage.h"

//#include "filterstack.h"

class Filter
{
public:
	Filter(void);
	Filter(std::string StringRep);
	~Filter(void);
	virtual std::string FilterToText(void);
	virtual void OnBeforeAll(void);
	virtual void Apply(class FontImage &image) = 0;
	virtual void OnAfterAll(void);
	const std::string GetFilterName(void);
	virtual bool ShowDialog(void);
	virtual bool RegisterDialogClass(HINSTANCE hinst, HWND parentwnd);
	void SetCondition(int ci, std::string &val);
	int GetCondition();
	std::string GetConditionVal();
	bool CheckCondition(char c);
protected:
	friend class FontImage;
	Filter(std::string filtername, bool unused);
	const std::string name;
	unsigned char GetColor(FontImage &image, int X, int Y);
	void SetColor(FontImage &image, int X, int Y, unsigned char C);
	int condition;
	std::string conditionval;
};

#endif