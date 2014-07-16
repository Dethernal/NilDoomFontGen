#include "stdafx.h"
#include "exportfilter.h"
#include "Resource.h"
#include <sstream>
#include <math.h>
#include "translation.h"

ExportFilter::ExportFilter(void) : Filter("Export",0)
{

}

ExportFilter::ExportFilter(std::string StringRep) : Filter("Export",0) 
{
}

ExportFilter::~ExportFilter(void)
{

}

void ExportFilter::OnAfterAll(void)
{
	MessageBoxW(0,t::exportend.c_str(),L"Export",MB_OK);
}

std::string ExportFilter::FilterToText(void)
{
	return "";
}

void ExportFilter::Apply(FontImage &image) 
{
	std::string fname = "images\\";
	unsigned char num = (unsigned char)image.GetCharRepresentation();
	fname += std::to_string((unsigned long long)num) + ".PCX";
	image.SaveAsPCX(fname);
}

bool ExportFilter::ShowDialog(void)
{
	return false;
}

bool ExportFilter::RegisterDialogClass(HINSTANCE hinst, HWND parentwnd)
{
	return false;
}