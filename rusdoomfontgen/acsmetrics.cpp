#include "stdafx.h"
#include "acsmetrics.h"
#include <fstream>

const int doom_letterspace = 1;

ACSMetrics::ACSMetrics(BigFont *fnt, const std::string &name) : font(fnt), prefix(name) {};

void ACSMetrics::Dump(std::string path)
{
	std::ofstream fs(path, std::ios::trunc);
	fs << "#include \"zcommon.acs\"";
	fs << "#define " << prefix << "_LETTERSPACE " << doom_letterspace;
	fs << "#define " << prefix << "_HEIGTH " << font->GetImage(0)->GetHeigth();
	fs << "#define " << prefix << "SPACEWIDTH ";



		if (FirstChar <= ' ' && LastChar >= ' ')
{
SpaceWidth = widths2[' '-FirstChar];
}
else if (FirstChar <= 'N' && LastChar >= 'N')
{
SpaceWidth = (widths2['N' - FirstChar] + 1) / 2;
}



	fs << "const int " << prefix << "_charwidths[256] = {";
	const std::string charset = font->GetCharset();
	const int chrslen = charset.length();
	unsigned char p = 0;
	for (unsigned char i =0; i<255;i++)
	{
		if (charset[p++] == i) fs << charset[p];
		else fs << 0;
		if (i<254) fs << ", ";
	}
	fs << "};";


}

/*
#define $$PREFIX$$_STRING_MAX_LENGTH 32766
#define $$PREFIX$$_STRING_MAX_LINES 100
#define $$PREFIX$$_STRING_MAX_LINE_LENGTH 127



int $$PREFIX$$_in_buffer[$$PREFIX$$_STRING_MAX_LENGTH+1];

int $$PREFIX$$_out_strings[$$PREFIX$$_STRING_MAX_LINES][$$PREFIX$$_STRING_MAX_LINE_LENGTH];

function int $$PREFIX$$_work(int string)
{
if (strlen(string) < $$PREFIX$$_STRING_MAX_LENGTH)
{
	strcpy(a:$$PREFIX$$_in_buffer, string);
	$$PREFIX$$_in_buffer[strlen(string)] = 0;
	return 1;
} else 
	log(s:"ERROR string too long!");
	return 0;
}
}

function int $$PREFIX$$_wrap(int pixelwidth);
{
int sptr = 0;
int wptr = 0;
int lines = 0;
while ((in_buffer[sptr]!=0)
{

}
}

*/