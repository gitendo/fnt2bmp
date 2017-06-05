#ifndef FNT2BMP_H
#define FNT2BMP_H

typedef signed char		s8;
typedef unsigned char	u8;
typedef signed short	s16;
typedef unsigned short	u16;
typedef signed int		s32;
typedef unsigned int	u32;

#define VERSION 		1.00
#define ERR_NOT_FOUND	1
#define ERR_MALLOC_FNT	2
#define ERR_MALLOC_BMP	3
#define ERR_UNK_OPTION	4

#define CHAR_WIDTH		8
#define CHAR_LENGTH		8
#define BMP_CHAR_LENGTH	64
#define MIN_BMP_WIDTH	8
#define MAX_BMP_WIDTH	2560

#pragma pack(push, 1)

typedef struct _BMPFILEHEADER {
	u16	bfType;
	u32	bfSize;
	u16	bfReserved1;
	u16	bfReserved2;
	u32	bfOffBits;
} BMPFILEHEADER;

typedef struct _BMPINFOHEADER {
	u32 biSize;
	u32 biWidth;
	u32 biHeight;
	u16 biPlanes;
	u16 biBitCount;
	u32 biCompression;
	u32 biSizeImage;
	u32 biXPelsPerMeter;
	u32 biYPelsPerMeter;
	u32 biClrUsed;
	u32 biClrImportant;
} BMPINFOHEADER;

typedef struct _RGBQUAD {
	u8 rgbBlue;
	u8 rgbGreen;
	u8 rgbRed;
	u8 rgbReserved;
} RGBQUAD;

#pragma pack(pop)

void usage(void);
void error(u8);
void release(void);

#endif /* FNT2BMP_H */
