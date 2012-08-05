//#include "math.h"


#define COLOR_WHITE PIXEL(0xFF,0xFF,0xFF)
#define COLOR_RED PIXEL(0xFF,0,0)
#define COLOR_BLACK PIXEL(0,0,0)
#define COLOR_GREEN PIXEL(0,0xFF,0)
#define COLOR_BLUE PIXEL(0,0,0xFF)
#define COLOR_YELOW PIXEL(0xFF,0xFF,0)


#define TRANS_COLOR 0x1
#define SOLID_COLOR 0xFFFFFFFF
#define TRANSPARENT_COLOR PIXEL(0xe5,0x40,0xe3)  //ERRADO.....

#define BMPNORMAL 0
#define BMPOPTIMIZE 1

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

typedef struct s_tgaheader_t
{
	unsigned char   idLength;
	unsigned char   colorMapType;
	unsigned char   imageType;
	unsigned char   colorMapSpec[5];
	short  xOrigin;
	short  yOrigin;
	short  width;
	short  height;
	unsigned char   bpp;
	unsigned char   imageDesc;

}tgaheader_t;

typedef struct s_rgb_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}rgb_t;

#pragma pack(pop)   /* restore original alignment from stack */


typedef struct _sOptImage
{
//	int ds,dd;
//	int dx,dy;
	int ds,dd,size;
	int dx;

} sOptImage;


typedef struct _BMPimage
{
	int resx;
	int resy;
	pixel *buf;

	int resxOrg;
	int resyOrg;
	int offsetx;
	int offsety;

	int sizeOptImage;
	sOptImage *optImage;

} BMPimage;

typedef struct _boxArea
{
	int x1,y1,x2,y2;

} boxArea;

typedef struct _fontInfoChar
{
	pixel *bci;
	int min;
	int max;

} fontInfoChar;

typedef struct _fontText
{
	char *fileName;
	int sizew;
	int sizeh;
	int spacew;
	BMPimage bmp;

	fontInfoChar listChar[256];

} fontText;

int loadTGA(char *fileName,BMPimage *image);
void putViewImage(int xview,int yview,BMPimage *image);
void copyImage(BMPimage *src,BMPimage *dst);
void cloneImage(BMPimage *src,BMPimage *dst);
void freeImage(BMPimage *img);
void fadeOutViewImage(int xview,int yview,BMPimage *image);
void fadeInViewImage(int xview,int yview,BMPimage *image);
void printImageText(int x,int y,char *str,fontText *fonts,int cor,BMPimage *image);
int putImage(int xp,int yp,BMPimage *image,BMPimage *frame,pixel colorTrans);
void putImageSolid(int xp,int yp,BMPimage *image,BMPimage *frame);
void putMaskImage(int xp,int yp,BMPimage *image,BMPimage *frame,pixel colorTrans,BMPimage *mask,pixel colorMask);
//void copyImageView(int xview,int yview,int resx,int resy,BMPimage *src,BMPimage *dst);
void copyImageView(int xview,int yview,BMPimage *src,BMPimage *dst);
void scrollImageView(int xview,int yview,BMPimage *src,BMPimage *dst);

void drawLine(int x1,int y1,int x2,int y2,pixel color,BMPimage *image);
void drawPixel(int x,int y,pixel color,BMPimage *image);
void drawRetangle(int x1,int y1,int x2,int y2,pixel color,BMPimage *image);
void drawRetangleFast(int x1,int y1,int x2,int y2,pixel color,BMPimage *image);
pixel readPixel(int x,int y,BMPimage *image);

void putViewImageLevelColor(int xview,int yview,BMPimage *image,BMPimage *imageDst,int levelColor);
int printImageTextClip(int x,int y,char *str,fontText *fonts,int cor,BMPimage *image);
int printImageTextClipFase(int x,int y,char *str,fontText *fonts,int cor,BMPimage *image,boxArea *clipArea);

void cleanImageFast(BMPimage *src,unsigned char color);

void autoCutImage(BMPimage *image);

void getOptImage(BMPimage *image,BMPimage *view,pixel colorTrans);
void putOptImage(int xp,int yp,BMPimage *image,BMPimage *frame,pixel colorTrans);

void refreshAreaImage(int x1,int y1,int x2,int y2,BMPimage *src,BMPimage *dst);
int getCodeCar(char str);
void drawBOX(int x1,int y1,int x2,int y2,pixel color,int flagAlpha,BMPimage *image);

void resetClipArea(void);
void setClipArea(int x1,int y1,int x2,int y2,BMPimage *view);

void drawLineH(int x1,int y1,int x2,pixel color,BMPimage *image);
void drawLineV(int x1,int y1,int y2,pixel color,BMPimage *image);


