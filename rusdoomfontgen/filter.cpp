#include "stdafx.h"
#include "filter.h"

Filter::Filter(void) : name("Dummy"), condition(0), conditionval("") {}
Filter::Filter(std::string StringRep) : name("Dummy"), condition(0), conditionval("")  {}
Filter::~Filter(void) {}
std::string Filter::FilterToText(void) {return std::string();}
const std::string Filter::GetFilterName(void) {return name;}

unsigned char Filter::GetColor(FontImage &image, int X, int Y) 
{
	return image.data[Y*image.width + X];
}

void Filter::SetColor(FontImage &image, int X, int Y, unsigned char C)
{
	image.data[Y*image.width + X] = C;
}

bool Filter::ShowDialog(void) { return true;}
bool Filter::RegisterDialogClass(HINSTANCE hinst, HWND parentwnd) {return false;}

Filter::Filter(std::string filtername, bool unused) : name(filtername), condition(0), conditionval("") {}



void Filter::SetCondition(int ci, std::string &val)
{
	condition = ci;
	conditionval = val; 
}

bool Filter::CheckCondition(char c)
{
	switch (condition) {
	case 0:
		return 1; // alvays
	case 1:
		return 0; // never
	case 2:
		return (isalpha((unsigned char)c)); // char
	case 3:
		return (isdigit((unsigned char)c)); // dight
	case 4:
		return (isalpha((unsigned char)c) && (tolower(c) == c)); // lower char
	case 5:
		return (isalpha((unsigned char)c) && (toupper(c) == c)); // upper char
	case 6:
		return (isdigit((unsigned char)c) && (c & 1)); //odd
	case 7:
		return (isdigit((unsigned char)c) && ((c & 1) == 0)); //even
	case 8:
		return ((!isalpha((unsigned char)c)) && (isalnum((unsigned char)c))); // another symbol
	case 9:
		return (conditionval.find(c) != std::string::npos); // symbol in string
	}
	return false;
}

int Filter::GetCondition()
{
	return condition;
}
std::string Filter::GetConditionVal()
{
	return conditionval;
}