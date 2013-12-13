#pragma once

#ifndef __ACSMETRICS_H__
#define __ACSMETRICS_H__

#include "stdafx.h"
#include "bigfont.h"

class ACSMetrics
{
public:
	ACSMetrics(BigFont *fnt, const std::string &name);
	void Dump(std::string path);
private:
	ACSMetrics();
	BigFont * font;
	const std::string &prefix;
};

#endif