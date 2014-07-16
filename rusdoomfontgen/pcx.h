#pragma once

#ifndef __PCX_H__
#define __PCX_H__
#include "stdafx.h"
#include "doompal.h"

typedef struct _PcxHeader
{
  unsigned char	Identifier;        /* PCX Id Number (Always 0x0A) */
  unsigned char	Version;           /* Version Number */
  unsigned char	Encoding;          /* Encoding Format */
  unsigned char	BitsPerPixel;      /* Bits per Pixel */
  unsigned short int	XStart;            /* Left of image */
  unsigned short int	YStart;            /* Top of Image */
  unsigned short int	XEnd;              /* Right of Image */
  unsigned short int	YEnd;              /* Bottom of image */
  unsigned short int	HorzRes;           /* Horizontal Resolution */
  unsigned short int	VertRes;           /* Vertical Resolution */
  unsigned char	Palette[48];       /* 16-Color EGA Palette */
  unsigned char	Reserved1;         /* Reserved (Always 0) */
  unsigned char	NumBitPlanes;      /* Number of Bit Planes */
  unsigned short int	BytesPerLine;      /* Bytes per Scan-line */
  unsigned short int	PaletteType;       /* Palette Type */
  unsigned short int	HorzScreenSize;    /* Horizontal Screen Size */
  unsigned short int	VertScreenSize;    /* Vertical Screen Size */
  unsigned char	Reserved2[54];     /* Reserved (Always 0) */
} PCXHEAD;

enum RLEType {RLE_PCX, RLE_Lines};

class RLERep
{
public:
	RLERep(unsigned char* data, int h, int w);
	unsigned char* GetRLEData(void);
	unsigned int GetRLEDataCount(void);
	~RLERep();
private:
	unsigned char * rledata;
	unsigned int rledatacount;
};

void SaveasPCX256_DOOM(int w, int h, char* data, std::string path, int encoding = 1);

void LoadasPCX256_DOOM(std::string path, int* width, int* height, char** data, char** pal);

LONG packrow (BYTE *pSource, FILE *dest, LONG rowSize);
#endif