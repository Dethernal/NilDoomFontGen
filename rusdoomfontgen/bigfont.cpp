#include "stdafx.h"
#include "bigfont.h"
#include <math.h>
//#include "rusdoomfontgen.h"

BigFont::BigFont(HDC modeldc, HFONT fnt, std::string chars, FilterStack *filters,int fkerning)
{
	SIZE size_r;
	ABCFLOAT correctw;
	TEXTMETRICW correctmetrics;
	kerning = fkerning;
	chars = SortChars(chars);
	int tx = 0;
	int ty = 0;
	HDC dc;
	RECT r;
	RECT origr;
	HBITMAP bmp;
	const char Wchar = 'W';
	HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
	charset = chars;

	dc = CreateCompatibleDC(modeldc);
	bmp = CreateCompatibleBitmap(modeldc,100,100);
	SelectObject(dc,fnt);
	SelectObject(dc,bmp);
	HPEN pen = CreatePen(PS_SOLID,1,RGB(255,0,0));
	SelectObject(dc, pen);
	SetBkColor(dc,RGB(0,0,0));
	SetBkMode(dc,OPAQUE);
	SetTextColor(dc,RGB(224,224,224));
	SelectObject(dc, brush);

	origr.top = 0;
	origr.bottom = 0;
	origr.left = 0;
	origr.right = 0;
	DrawTextA(dc,&Wchar,1,&origr,DT_CALCRECT | DT_TOP | DT_LEFT | DT_SINGLELINE | DT_NOCLIP);
	int paddingX = origr.right + 5;
	int paddingY = origr.bottom + 5;

	GetTextMetricsW(dc,&correctmetrics);

	imagedc = CreateCompatibleDC(modeldc);
	imagebmp = CreateCompatibleBitmap(modeldc, paddingX * 16, paddingY * 16);
	SelectObject(imagedc, imagebmp);
	imagerect.left = 0;
	imagerect.right = paddingX * 16;
	imagerect.top = 0;
	imagerect.bottom = paddingY * 16;
	FillRect(imagedc, &r, brush);
	images = (FontImage**) malloc((chars.length()) * sizeof (FontImage*));
	
	
	for (unsigned int i = 0; i< chars.length();i++)
	{
		r.top = 1;
		r.left = 1;
		r.right = 100;
		r.bottom = 100;

		origr.top = 0;
		origr.bottom = 0;
		origr.left = 0;
		origr.right = 0;
		std::string sub = charset.substr(i,1);
		const char *c = sub.data();
	
		DrawTextA(dc,c,1,&origr,DT_CALCRECT | DT_TOP | DT_LEFT | DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX);

		if (correctmetrics.tmItalic)
			origr.right += correctmetrics.tmAveCharWidth;

		origr.right +=1;
		origr.bottom +=1;
		DrawTextA(dc,c,1,&r,DT_TOP | DT_LEFT | DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX);
//		
//		or.right += correctmetrics.tmOverhang;
		images[i] = new FontImage(dc,&origr, *c);
		BitBlt(imagedc,tx * paddingX, ty * paddingY, origr.right - origr.left + 3,origr.bottom - origr.top + 3,dc,1, 1,SRCCOPY);
		tx++;
		if (tx>15) {tx = 0; ty++;}
		FillRect(dc,&r,brush);
	}
	DeleteDC(dc);
	DeleteObject(pen);
	DeleteObject(brush);
	DeleteObject(bmp);

	for (unsigned int i = 0; i< charset.length();i++)
	{
		images[i]->CompressBright(8,111);
		if (filters) 
			filters->Process(*images[i]);
		//images[i]->DrawBorder(0,0,images[i]->GetWidth(),images[i]->GetHeigth(),255);
	}
}
BigFont::~BigFont()
{
	for (unsigned int i = 0; i<charset.length();i++)
		delete images[i];
	free(images);
}
void BigFont::ExportToLMP(std::string path)
{
	const char* fon2 = "FON2";
	FILE *file = fopen(path.c_str(),"wb");
	fwrite(fon2,1,4,file); // FON2
	unsigned short int h = (images[0])->GetHeigth(); // Height
	fwrite(&h,sizeof(unsigned short int),1,file);
	unsigned char c = charset[0];
	fwrite(&c,sizeof(unsigned char),1,file); // First char
	c = charset[charset.length()-1];
	fwrite(&c,sizeof(unsigned char),1,file); // last char
	c = 0;
	fwrite(&c,sizeof(unsigned char),1,file); // wariable width
	fwrite(&c,sizeof(unsigned char),1,file); // shading
	c = 255;
	fwrite(&c,sizeof(unsigned char),1,file); // palette size
		c = (kerning != 0);
	fwrite(&c,sizeof(unsigned char),1,file); // flags, kerning is present
	if (c)
	{	
		h = kerning;
		fwrite(&h,sizeof(unsigned short int),1,file); // kerning
	}
	for (unsigned int i=charset[0],j = 0; i<=(unsigned char)charset[charset.length()-1]; i++,j++)
	{
		unsigned char cc = 0;
		while ((cc = (unsigned char)charset[j]) != i)
		{
			h = 0;
			fwrite(&h,sizeof(unsigned short int),1,file);
			i++;
		}
			h = (images[j])->GetWidth();
			fwrite(&h,sizeof(unsigned short int),1,file);
	}
	fwrite(doompal,sizeof(unsigned char),768,file);
	for (unsigned int i=0;i<charset.length();i++)
	{
		images[i]->DumpWithRLE(file);
	}
	fclose(file);
}

void BigFont::DrawChar(char c, HDC dc, int x, int y)
{
	for (FontImage** img = images; img < images + charset.length(); img++)
		if ((*img)->GetCharRepresentation() == c)
		{
			(*img)->DrawTo(dc,x,y);
			break;
		}
}

void BigFont::DrawPreviewTo(HDC dst, int x, int y, unsigned int mode)
{
	RECT r = {0,0,512,640};
	HBRUSH  b= CreateSolidBrush(0);
	HPEN pen = CreatePen(PS_SOLID,1,RGB(255,0,0));
	FillRect(dst,&r, (HBRUSH) b);
	HGDIOBJ oldpen = SelectObject(dst,pen);
	DeleteObject(b);
	int ax = 0;
	int ay = 0;
	int pos = 0;
	for (char c = charset[0]; pos < charset.length();++pos)
	{
		c = charset[pos];
		FontImage* img = images[pos];
		MoveToEx(dst,x + ax*30, y + ay*(img->GetHeigth() + 5),0);
		DrawChar(c, dst, x + ax*30,  y + ay*(img->GetHeigth()+ 5));
		
		LineTo(dst,x + ax*30 + img->GetWidth(),y + ay*(img->GetHeigth() + 5));
		LineTo(dst,x + ax*30 + img->GetWidth(),y + ay*(img->GetHeigth() + 5) + img->GetHeigth());
		LineTo(dst,x + ax*30,y + ay*(img->GetHeigth() + 5) + img->GetHeigth());
		LineTo(dst,x + ax*30,y + ay*(img->GetHeigth() + 5));
		ax++;
		if (ax > 16)
		{
			ax = 0;
			ay++;
		}
	}
	SelectObject(dst,oldpen);
	DeleteObject(pen);
	//BitBlt(dst,0,0,imagerect.right,imagerect.bottom,imagedc,x,y, mode);
}