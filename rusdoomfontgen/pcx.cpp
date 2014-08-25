#include "stdafx.h"
#include "pcx.h"
#include <string.h>
#include <string>

const int DUMP = 0;
const int RUN =	1;

const int MinRun = 3;
const int MaxRun = 128;
const int MaxDat = 128;

static LONG putSize;
static char buf[256];	/* [TBD] should be 128?  on stack?*/

#define GetByte()		(*source++)
#define PutByte(c)		{ putc (c, dest); ++putSize; }

const unsigned char magic_constant = 0x0c;

#define RLECONVBUFFERLENGTH 2048
unsigned char rleconvbuffer[RLECONVBUFFERLENGTH];

int encodeSeq(char c,unsigned char l, FILE* out)
{
	if ((l == 1) && (0xC0 != (0xC0 & c)))
	{
		fwrite(&c,1,1,out);
		return 1;
	}
	else
	{
		l = l | 0xC0;
		fwrite(&l,1,1,out);
		fwrite(&c,1,1,out);
		return 2;
	}

}

int encodeLinePCX(char* line, int len, FILE* out)
{
	unsigned char runcount = 1;
	int total = 0;
	char cur = *line;
	int i;
	for (int srcIndex = 1; srcIndex < len; srcIndex++)
	{
		char next = *(++line);
        if (next == cur)     /* There is a "run" in the data, encode it */
                {
                runcount++;
                if (runcount == 63)
                        {
                        if (! (i = encodeSeq(cur, runcount, out)))
                                return 0;
                        total += i;
                        runcount = 0;
                        }
                }
        else                /* No "run"  -  this != last */
                {
                if (runcount)
                        {
                        if (! (i = encodeSeq(cur, runcount, out)))
                                return 0;
                        total += i;
                        }
                cur = next;
                runcount = 1;
                }
        }        /* endloop */
  if (runcount)        /* finish up */
        {
        if (! (i = encodeSeq(cur, runcount, out)))
                return (0);
        return (total + i);
        }
  return (total);
}

void encodePCX(int w, int h, char* data, FILE* out)
{
	for (int i = 0; i < h; i++)
	{
		encodeLinePCX(data+i*w,w,out);
	}
}



void decodeLine(int w, char* line, FILE* in)
{
	int i = 0;
	unsigned char c1;
	unsigned char c2;
	while(i < w)
	{
		fread(&c1,1,1,in);
		if ((c1 & 0xC0) == 0xC0)
		{
			fread(&c2,1,1,in);
			c1 = c1 & 0x3F;
			for (int j = 0; j<c1;j++)
			{
				*line = (char) c2;
				line++;
			}
			i+=c1;
		}
		else
		{
			*line = (char) c1;
			line++;
			i+=1;
		}

	}
}

void decodePCX(int w, int h, char* data, FILE* in)
{
	for (int i =0; i<h;i++)
	{
		decodeLine(w,data,in);
		data += w;
	}
}

void SaveasPCX256_DOOM(int w, int h, char* data, std::string path, int encoding)
{
	PCXHEAD header;
	header.Identifier = 0x0A;
	header.Version = 5;
	header.Encoding = encoding?1:0;
	header.BitsPerPixel = 8;
	header.XStart = 0;
	header.YStart = 0;
	header.XEnd = (unsigned short int) w-1;
	header.YEnd = (unsigned short int) h-1;
	header.HorzRes = 72;
	header.VertRes = 72;
	memcpy(header.Palette,EGA_PCX_PAL,sizeof(EGA_PCX_PAL));
	//memset(&(header.Palette[0]),0,48);      
	header.Reserved1 = 0;
	header.NumBitPlanes = 1;
	header.BytesPerLine = (unsigned short int) w;
	header.PaletteType = 1;
	header.HorzScreenSize = 0;
	header.VertScreenSize = 0;
	memset(&(header.Reserved2[0]),0,54);

	FILE *f = fopen(path.c_str(),"wb");
	fwrite(&header,sizeof(PCXHEAD),1,f);
	if (!encoding)
		fwrite(&data,1,w * h,f);
	else
		encodePCX(w,h,data,f);

	fwrite(&magic_constant,1,1,f);
	fwrite(doompal,1,768,f);
	fclose(f);
}

void LoadasPCX256_DOOM(std::string path, int* width, int* height, char** data, char** pal)
{
	char * tdata;
	char * tpal;
	PCXHEAD header;
	FILE *f = fopen(path.c_str(),"rb");
	fread(&header, sizeof(PCXHEAD),1, f);

	if (header.BitsPerPixel != 8) throw new std::runtime_error("Unsupported PCX file. Only 256-pallette images are alailable.");

	*width = header.XEnd - header.XStart + 1;
	*height = header.YEnd - header.YStart + 1;

	tdata = new char[*width * *height];
	*data = tdata;

	tpal = new char[768];
	*pal = tpal;

	if (header.Encoding == 0)
	{
		fread(tdata,1, *width * *height, f);
	}
	else
	{
		decodePCX(*width, *height,tdata,f);
	}
	unsigned char magic = 0;
	fread(&magic,1, 1, f);
	if (magic != magic_constant) throw new std::runtime_error("Bad PCX file.");
	fread(tpal,1, 768, f);
	fclose(f);
}

static FILE *PutDump (FILE *dest, int nn)
{
	int i;

	PutByte (nn-1);
	for (i = 0; i < nn; i++)
		PutByte (buf[i]);
	return dest;
}

static FILE *PutRun (FILE *dest, int nn, int cc)
{
	PutByte (-(nn-1));
	PutByte (cc);
	return dest;
}

#define OutDump(nn)		dest = PutDump (dest, nn)
#define OutRun(nn,cc)	dest = PutRun (dest, nn, cc)

LONG packrow (BYTE *pSource, FILE *dest, LONG rowSize)
{
	BYTE *source;
	char c, lastc = '\0';
	int mode = DUMP;
	short nbuf = 0;				/* number of chars in buffer */
	short rstart = 0;			/* buffer index current run starts */

	source = pSource;
	putSize = 0;
	buf[0] = lastc = c = GetByte();	/* so have valid lastc */
	nbuf = 1;	rowSize--;		/* since one byte eaten. */

	for (; rowSize; --rowSize)
	{
		buf[nbuf++] = c = GetByte();
		switch (mode)
		{
		case DUMP:
			/* If the buffer is full, write the length byte, then the data */
			if (nbuf > MaxDat)
			{
				OutDump (nbuf - 1);
				buf[0] = c;
				nbuf = 1;	rstart = 0;
				break;
			}

			if (c == lastc)
			{
				if (nbuf-rstart >= MinRun)
				{
					if (rstart > 0)	OutDump(rstart);
					mode = RUN;
				}
				else if (rstart == 0)
					mode = RUN;		/* no dump in progress, so can't
									lose by making these 2 a run.*/
			}
			else	rstart = nbuf-1;		/* first of run */
			break;

		case RUN:
			if ( (c != lastc) || (nbuf-rstart > MaxRun))
			{ /* output run */
				OutRun(nbuf-1-rstart, lastc);
				buf[0] = c;
				nbuf = 1; rstart = 0;
				mode = DUMP;
			}
			break;
		}

		lastc = c;
	}

	switch (mode)
	{
	case DUMP: OutDump(nbuf); break;
	case RUN: OutRun(nbuf-rstart,lastc); break;
	}
	return putSize;
}


	RLERep::RLERep(unsigned char* data, int h, int w)
	{



		int BufferSize = w * 2;
		unsigned char* buf = 0;

		rledata = (unsigned char*) malloc(w * h * 2);

		if (BufferSize>RLECONVBUFFERLENGTH)
			buf = (unsigned char*) malloc(BufferSize *2);
		else
			buf = rleconvbuffer;
		int rledatalen = 0;

	for (int i = 0;i<h;i++)
	{
	int index = 0;        /* Index into uncompressed data buffer  */
    int scanindex = 0;    /* Index into compressed data buffer    */
    char runcount = 0;                  /* Length of encoded pixel run          */
    char runvalue = 0;                  /* Value of encoded pixel run           */
    while (index < BufferSize && index < h * w)
    {
        /*
        ** Get the run count of the next pixel value run.
        **
        ** Pixel value runs are encoded until a different pixel value
        ** is encountered, the end of the scan line is reached, or 63
        ** pixel values have been counted.
        */
        for (runcount = 1, runvalue = data[i*w+index];
             runvalue == data[i*w+index + runcount] &&
             index + runcount < BufferSize &&
             runcount < 63;
             runcount++);

        /*
        ** Encode the run into a one or two-byte code.
        **
        ** Multiple pixel runs are stored in two-byte codes.  If a single
        ** pixel run has a value of less than 64 then it is stored in a
        ** one-byte code.  If a single pixel run has a value of 64 to 255
        ** then it is stored in a two-byte code.
        */
        if (runcount > 1)                   /* Multiple pixel run */
        {
            buf[scanindex++] = runcount | 0xC0;
            buf[scanindex++] = runvalue;       
        }
        else                                /* Single pixel run   */
        {
            if (data[i*w+index] < 64)  /* Value is 0 to 63   */
            {
                buf[scanindex++] = runvalue;
            }
            else                            /* Value is 64 to 255 */
            {
                buf[scanindex++] = runcount | 0xC0;
                buf[scanindex++] = runvalue;       
            }
        }
        index += runcount;  /* Jump ahead to next pixel run value */
    }
	memmove(rledata+rledatalen,buf,index);
	rledatalen+=index;
	}
	if (BufferSize>RLECONVBUFFERLENGTH)
		delete[] buf;
	rledatacount = rledatalen;
}

	RLERep::~RLERep()
	{
		delete[] rledata;
	}
	unsigned char* RLERep::GetRLEData(void)
	{
		return rledata;
	}
	unsigned int RLERep::GetRLEDataCount(void)
	{
		return rledatacount;
	}
	