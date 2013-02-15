#include "stdafx.h"
#include "fontimage.h"

unsigned char getI(unsigned int color)
{
	if (color == CLR_INVALID) 
		return 0;
	char R = (char) ((0x00FF0000 & color) >> 16);
	char G = (char) ((0x0000FF00 & color) >> 8);
	char B = (char) ((0x000000FF & color));
	return (char) ((0.2125 * R) + (0.7154 * G) + (0.0721 * B));
}

FontImage::FontImage(HDC screen, RECT* r ,char rchar)
{
	representchar = rchar;
	heigth = r->bottom - r->top;
	width = r->right - r->left;
	data = (unsigned char*) malloc(heigth * width);
	for (unsigned int j = 0; j<heigth;j++)
		for(unsigned int i = 0;i<width;i++)
		{
			int c =  GetPixel(screen,r->left+i,r->top+j);
			if (c == 0)
				data[j*width+i] = 0;
			else
				data[j*width+i] = getI(c);
		}
}

FontImage::FontImage(FontImage& source)
{
	representchar = source.representchar;
	heigth = source.heigth;
	width = source.width;
	data = (unsigned char*) malloc(heigth * width);
	memcpy(data,source.data, heigth * width);
}

FontImage::~FontImage()
{
	free(data);
}
void FontImage::SaveAsPCX(std::string path)
{

}

void FontImage::DrawBorder(int x1,int y1,int x2,int y2, unsigned char color)
{
	for (int i=x1; i<=x2;i++) 
		data[y1*width+i] = color;
	for (int i=y1; i<=y2;i++) 
		data[i*width+x2] = color;
	for (int i=x2; i>=x1;i--) 
		data[y2*width+i] = color;
	for (int i=y2; i>=y1;i--) 
		data[i*width+x1] = color;
}

void FontImage::CompressBright(int div, int offset)
{
	for (unsigned int i=0; i<width*heigth;i++)
		if (data[i])
			data[i] = offset - data[i] / div;
}

RLERep *FontImage::GetRLERep(RLEMode mode)
{
	if (mode == RLE_raw)
		return new RLERep(data,1,heigth * width);
	else if (mode == RLE_lines)
		return new RLERep(data,heigth,width);
	else return 0;
}

void DoNothing(int i) {}

void FontImage::DumpWithRLE(FILE * f)
{
	for (unsigned int i = 0; i < heigth; i++)
	{
		int rl = 0;
		unsigned char * c = &data[i*width];
		rl = packrow(c,f,width);
		DoNothing(rl);
	}
}


void FontImage::DrawTo(HDC dc, int x, int y)
{
	for (int j = 0; j < (int) heigth; j++)
		for (int i = 0; i < (int) width; i++)
		{
			int b = 0;
			unsigned char c = data[j*width+i];
			if (c < 80)
			{
				if (c != 0)
					b = 80;
			}
			else
				b = (111 - c) * 8;
			SetPixel(dc,x+i,y+j,RGB(b,b,b));
		}

}

char FontImage::GetCharRepresentation(void)
{
	return representchar;
}

int _cdecl qcompare(const void* c1,const void *c2) {return (*(unsigned char*)c1) - (*(unsigned char*)c2);}

std::string SortChars(std::string str)
{
	char buf[257];
	memmove(buf,str.data(),str.length());
	qsort(buf,str.length(),1,&qcompare);
	buf[str.length()] = 0;
	std::string s(buf);
	return s;
}