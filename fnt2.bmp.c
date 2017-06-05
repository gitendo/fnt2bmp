#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fnt2bmp.h"

// globals
u8		*bmiData = NULL, *fnt = NULL;

// haelp!
void usage(void) {
	printf("syntax: fnt2bmp [options] file[.fnt]\n\n");
	printf("options:\n");
	printf("\t-h# - font height in px (8 - default, 14 or 16)\n");
	printf("\t-w# - bitmap width (must be multiple of 8 or will be rounded up)\n");
	printf("\n");
	exit(EXIT_FAILURE);
}

// error handler
void error(u8 msg) {
	switch(msg) {
		case ERR_NOT_FOUND	:	printf("Input file not found!\n"); break;
		case ERR_MALLOC_FNT	:	printf("Font memory allocation failed!\n"); break;
		case ERR_MALLOC_BMP	:	printf("Bitmap memory allocation failed!\n"); break;
		case ERR_UNK_OPTION :	printf("Invalid option!\n"); break;
		default				:	printf("Undefined error code!\n"); break;
	}

	release();

	exit(EXIT_FAILURE);
}

// release all allocated memory before exit
void release(void) {
	if(bmiData != NULL)
		free(bmiData);
	if(fnt != NULL)
		free(fnt);
}

int main (int argc, char *argv[]) {

	char 	fn[FILENAME_MAX];
	char 	*pos;
	BMPFILEHEADER bfh = {0};
	BMPINFOHEADER bih = {0};
	FILE	*fp;
	RGBQUAD bmiColors[256] = {0};

	u8		*dst = NULL, *src, *lut[16];
	u8		arg, b, height = 0, fname = 0, j, row;
	u16		cols = 0, width = 256, padding = 0;
	u32		dsize = 0, fsize, i;

	printf("\nfnt2bmp v%.2f - vga font to bitmap converter\n", VERSION);
	printf("programmed by: tmk, email: tmk@tuta.io\n");
	printf("bugs & updates: https://github.com/gitendo/fnt2bmp/\n\n");

	if(argc < 2)
		usage();

	for(arg = 1; arg < argc; arg++) {
		if((argv[arg][0] == '-') || (argv[arg][0] == '/')) {
			switch(tolower(argv[arg][1])) {
				case 'w':
					width = atoi(&argv[arg][2]);
					break;
				case 'h':
					height = atoi(&argv[arg][2]);
					break;
				default: 
					error(ERR_UNK_OPTION); 
					break;
			}
		} else {
			fname = arg;
		}
	}

	fp = fopen(argv[fname], "rb");
	if (!fp)
		error(ERR_NOT_FOUND);
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	fnt = malloc(fsize);
	if (!fnt)
		error(ERR_MALLOC_FNT);
	fread(fnt, 1, fsize, fp);
	fclose(fp);

	// header check
	if(*(u16*) fnt == 0x55AA) { // bios legacy?
		fsize -= 0x08;
		padding = 0x08;
	} else if(*(u32*) fnt == 0x0D9033EB) { // ? .com files
		fsize -= 0x63;
		padding = 0x63;
	} else if(*(u32*) fnt == 0x05A247EB) { // font mania .com files
		fsize -= 0x05A2;
		padding = 0x05A2;
	} else {
		padding = 0;
	}

	// check if width parameter is multiple of 8, round up if not
	if(width % 8)
		width = (width + (8 - 1)) & ~(8 - 1);

	// allowed width 8-2560
	if(width < MIN_BMP_WIDTH || width > MAX_BMP_WIDTH)
		width = 256;

	cols = width / CHAR_WIDTH;

	while(dsize < fsize / CHAR_LENGTH)
		dsize += cols;
	dsize = (dsize + cols) * BMP_CHAR_LENGTH;

	bmiData = malloc(dsize);
	if (!bmiData)
		error(ERR_MALLOC_BMP);
	memset(bmiData, 0, dsize);

	switch(height) {
		case 8:
			break;
		case 14:
			break;
		case 16:
			break;
		default:
			height = 8;
			break;
	}
	
	memset(lut, 0, sizeof(lut));

	row = 0;
	// loop through all chars
	for(i = 0; i < fsize / height; i++) {
		// we need new lut values for every row
		if(!(i % cols)) {
			lut[0] = &bmiData[width * height * row];
			for(j = 1; j < height; j++)
				lut[j] = lut[0] + (j * width);
			row++;
		} else {
			// update lut to another char
			for(j = 0; j < height; j++)
				lut[j] = lut[j] + CHAR_LENGTH;
		}
		// each char consists of 8, 14 or 16 bytes
		for(j = 0; j < height; j++) {
			src = &fnt[padding + (i * height) + j];
			dst = lut[j];
			// reverse bits in byte to avoid "mirrored" characters
			b = ((*src * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
			for(b; b; b >>= 1) {
				// "pixelize" each bit set
				if(b & 1)
					*dst = 1;
				dst++;
			}
		}
	}

	// depending on width parameter and chars used more memory might be allocated than needed
	// this checks against address of last pixel put and end of bitmap buffer to strip an empty row
	if((bmiData + dsize) - dst > cols * BMP_CHAR_LENGTH)
		dsize -= cols * BMP_CHAR_LENGTH;
	
	pos = strrchr(argv[fname], '.');
	if(pos != NULL)
		*pos = 0;
	strcpy(fn, argv[fname]);
	strcat(fn, ".bmp");
	fp = fopen(fn, "wb");

    bih.biSize = sizeof(BMPINFOHEADER);
    bih.biBitCount = 8;
    bih.biClrImportant = 0;
    bih.biClrUsed = 2;
    bih.biCompression = 0;
    bih.biHeight = -(dsize / (cols * 8));
    bih.biWidth = cols * 8;
    bih.biPlanes = 1;
    bih.biSizeImage = dsize;

	bfh.bfType = 'B'+('M' << 8);
	bfh.bfOffBits = sizeof(BMPINFOHEADER) + sizeof(BMPFILEHEADER) + sizeof(RGBQUAD) * 256;
	bfh.bfSize = bfh.bfOffBits + bih.biSizeImage;

	bmiColors[0].rgbBlue = 255;
	bmiColors[0].rgbGreen = 255;
	bmiColors[0].rgbRed = 255;

	fwrite(&bfh, 1, sizeof(bfh), fp);
	fwrite(&bih, 1, sizeof(bih), fp);
	fwrite(bmiColors, 1, sizeof(bmiColors), fp);
	fwrite(bmiData, 1, dsize, fp);
	fclose(fp);

	release();

	return 0;
}
