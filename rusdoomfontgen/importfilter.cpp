#include "stdafx.h"
#include "importfilter.h"
#include "Resource.h"
#include <sstream>
#include <math.h>

ImportFilter::ImportFilter(void) : Filter("Import",0)
{

}

ImportFilter::ImportFilter(std::string StringRep) : Filter("Import",0) 
{
}

ImportFilter::~ImportFilter(void)
{

}

std::string ImportFilter::FilterToText(void)
{
	return "";
}

void ImportFilter::Apply(FontImage &image) 
{
	std::string fname = "images\\";
	unsigned char num = (unsigned char)image.GetCharRepresentation();
	fname += std::to_string((unsigned long long)num) + ".PCX";
	image.ReadFromPCX(fname);
}

bool ImportFilter::ShowDialog(void)
{
	return false;
}

bool ImportFilter::RegisterDialogClass(HINSTANCE hinst, HWND parentwnd)
{
	return false;
}