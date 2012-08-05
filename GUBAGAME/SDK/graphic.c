#include "so.h"
#include "graphic.h"
#include "math.h"


int xclip1=-1,yclip1=-1,xclip2=-1,yclip2=-1;

/*============================================================
 F: reset clip area
 =============================================================*/
void resetClipArea(void)
{
	xclip1=-1;
}

/*============================================================
 F: set clip area
 =============================================================*/

void setClipArea(int x1,int y1,int x2,int y2,BMPimage *view)
{
	if(x1<0) x1=0;
	if(y1<0) y1=0;
	if(x2<0) x2=0;
	if(y2<0) y2=0;
	if(x1>=view->resx) x1=view->resx-1;
	if(x2>=view->resx) x2=view->resx-1;
	if(y1>=view->resy) y1=view->resy-1;
	if(y2>=view->resy) y2=view->resy-1;
			
	
	xclip1=x1;yclip1=y1;xclip2=x2;yclip2=y2;
}

/*============================================================
 F: clean fast image
 =============================================================*/

void cleanImageFast(BMPimage *src,unsigned char color)
{int y,size;
 pixel *bs;


	if(src)
	{
		if(src->buf) 
		{
			bs=src->buf;
			size=sizeof(pixel)*src->resx;
			for(y=0;y<src->resy;y++)
			{
				memset(bs,color,size);
				bs+=src->resx;
			}
		}
	}
}

/*============================================================
 F: copy image
 =============================================================*/

void copyImage(BMPimage *src,BMPimage *dst)
{int y,size;
 pixel *bs,*bd;


	if((src) && (dst))
	{
		if((src->buf) && (dst->buf))
		{
			bs=src->buf;
			bd=dst->buf;
			//memcpy(dst->buf,src->buf,sizeof(pixel)*dst->resx*dst->resy);		{
			size=sizeof(pixel)*dst->resx;
			for(y=0;y<dst->resy;y++)
			{
				memcpy(bd,bs,size);
				bs+=dst->resx;
				bd+=dst->resx;
			}
		}
	}
}


/*============================================================
 F: copy image
 =============================================================*/

//void copyImageView(int xview,int yview,int resx,int resy,BMPimage *src,BMPimage *dst)
void copyImageView(int xview,int yview,BMPimage *src,BMPimage *dst)
{int y,size;
 pixel *bs,*bd;


	if((src) && (dst))
	{
		if((src->buf) && (dst->buf))
		{
			bs=src->buf;
			bd=dst->buf;
			//memcpy(dst->buf,src->buf,sizeof(pixel)*dst->resx*dst->resy);		{
			size=src->resx*sizeof(pixel);
			//bs+=dst->resx*yview+xview;
			bd+=dst->resx*yview+xview;
			for(y=0;y<src->resy;y++)
			{
				memcpy(bd,bs,size);
				bs+=src->resx;
				bd+=dst->resx;
			}
		}
	}
}


/*============================================================
 F: refresh area image
 =============================================================*/

void refreshAreaImage(int x1,int y1,int x2,int y2,BMPimage *src,BMPimage *dst)
{int y,size;
 pixel *bs,*bd;


	if((src) && (dst))
	{
		if((src->buf) && (dst->buf))
		{
			bs=src->buf;
			bd=dst->buf;
			
			size=((x2-x1)+1)*sizeof(pixel);
			bs+=src->resx*y1+x1;
			bd+=dst->resx*y1+x1;
			for(y=y1;y<y2;y++)
			{
				memcpy(bd,bs,size);
				bs+=src->resx;
				bd+=dst->resx;
			}
		}
	}
}

/*============================================================
 F: copy image
 =============================================================*/

void scrollImageView(int xview,int yview,BMPimage *src,BMPimage *dst)
{int y,size;
 pixel *bs,*bd;
 int resx,resy;

 int offy=0;

	resx=GetVideoWidth();
	resy=GetVideoHeight();

	if((src) && (dst))
	{
		if((src->buf) && (dst->buf))
		{
			bs=src->buf;
			bd=dst->buf;

			size=resx*sizeof(pixel);
			//	offy=(xview % 720)*(resy-y)/1500;
			offy=(xview - 2160/3)*(resy-(590-yview))/2160;
			bd=dst->buf+dst->resx*(resy-1);
			bs=src->buf+src->resx*(yview+resy-1)+xview;

			for(y=resy-1;y>0;y--)
			{
				if( ((yview+y)>186) && ((yview+y)<590) )
					offy=(xview - 2160/3)*(resy-y)/2160;
			
				memcpy(bd,bs-offy,size);

				bs-=src->resx;
				bd-=dst->resx;
			}
		}
	}
}


/*
void scrollImageView(int xview,int yview,BMPimage *src,BMPimage *dst)
{int y,size;
 pixel *bs,*bd;
 int resx,resy;


	resx=GetVideoWidth();
	resy=GetVideoHeight();

	if((src) && (dst))
	{
		if((src->buf) && (dst->buf))
		{
			bs=src->buf;
			bd=dst->buf;

			size=resx*sizeof(pixel);

			bs+=src->resx*yview+xview;
			if((src->resx==dst->resx) && (src->resy==dst->resy))
				bd+=dst->resx*yview+xview;
			for(y=0;y<resy;y++)
			{

				memcpy(bd,bs,size);
				bs+=src->resx;
				bd+=dst->resx;
			}
		}
	}
}
*/

/*============================================================
 F: free image
 =============================================================*/

void freeImage(BMPimage *img)
{
	if(img)
	{
		if(img->buf) myfree(img->buf);
		img->buf=0;
	}
}
/*============================================================
 F: clone image
 =============================================================*/

void cloneImage(BMPimage *src,BMPimage *dst)
{
	if((src) && (dst))
	{
		if(src->buf)
		{
			if(dst->buf!=0) 
			{
				freeImage(dst);
				
			}

			dst->buf=mymalloc(sizeof(pixel)*src->resx*src->resy);
			if(dst->buf)
			{
				dst->resx=src->resx;
				dst->resy=src->resy;
				dst->resxOrg=src->resxOrg;
				dst->resyOrg=src->resyOrg;
				dst->offsetx=src->offsetx;
				dst->offsety=src->offsety;
			}

			memset(dst->buf,0,sizeof(pixel)*src->resx*src->resy);

		}
	}
}

/*============================================================
 F: Put image in Video view
 =============================================================*/

void putViewImage(int xview,int yview,BMPimage *image)
{int rvx,rvy,rx,ry,y,size;
 pixel *pd,*ps,*p;

	if(image)
	{	
		pd=GetVideo();
		ps=image->buf;
		rvx=GetVideoWidth();
		rvy=GetVideoHeight();
		rx=image->resx;
		ry=image->resy;

		p=ps+(yview)*rx+xview;
		if(rx<rvx)
			size=rx*sizeof(pixel);
		else
			size=rvx*sizeof(pixel);
		for(y=0;y<rvy;y++)
		{
			memcpy(pd,p,size);
			p+=rx;
			pd+=rvx;

		}

	}
}



/*============================================================
 F: Put image in Video view with level color
 =============================================================*/

void putViewImageLevelColor(int xview,int yview,BMPimage *image,BMPimage *imageDst,int levelColor)
{int rvx,rvy,rx,ry,x,y,sizex,sizey;
 pixel *pd,*ps,*p;

 unsigned char *bs,*bd,*bdd,*bss,c;

	if(image)
	{	
		pd=imageDst->buf; //GetVideo();
		ps=image->buf;
		rvx=imageDst->resx; //GetVideoWidth();
		rvy=imageDst->resy; //GetVideoHeight();
		rx=image->resx;
		ry=image->resy;

		p=ps+(yview)*rx+xview;
		if(rx<rvx)
			sizex=rx*sizeof(pixel);
		else
			sizex=rvx*sizeof(pixel);

		if(ry<rvy)
			sizey=ry;
		else
			sizey=rvy;
		
		bs=(unsigned char *)p;
		bd=(unsigned char *)pd;

		rx=rx*sizeof(pixel);
		rvx=rvx*sizeof(pixel);

		for(y=0;y<sizey;y++)
		{
			bss=bs;
			bdd=bd;
			for(x=0;x<sizex;x++)
			{
				c=*bss++;
				c=c>>levelColor;
				*bdd++=c;				
			}
			bs+=rx;
			bd+=rvx;
		}

	}
}



/*============================================================
 F: Print text in Video view
 =============================================================*/

void printImageText(int x,int y,char *str,fontText *fonts,int cor,BMPimage *image)
{pixel *bd,*bs,*bc,*bci,*bdd;
 int i,j,c;
 int cx,cy;
 int width,height;
 int max,min;
 int sizew,sizeh; 



	if(fonts->bmp.buf==0)
		loadTGA(fonts->fileName,&fonts->bmp);

	sizew=fonts->sizew;
	sizeh=fonts->sizeh;

	width=fonts->bmp.resx/sizew;
	height=fonts->bmp.resy/sizeh;

	bs=fonts->bmp.buf;
	
	while(*str)
	{
		c=getCodeCar(*str);
			
		if(fonts->listChar[c].bci==0)
		{
			cx = ((c % width) ); 
			cy = ((c / width) ); 
			cx*=sizew;
			cy*=sizeh;

			max=0;
			min=255;
			bci=bs+fonts->bmp.resx*cy+cx;

			bd=bci;
			for(i=0;i<sizeh;i++)
			{
				bc=bci;
				for(j=0;j<sizew;j++)
				{
					if(*bc!=COLOR_WHITE)	
					{
						if(j>max) max=j;
						if(j<min) min=j;
					}
					bc++;
				}
				bci+=fonts->bmp.resx;
			}
			bci=bd;
			if(c==0)
			{
				min=0;max=sizew/4;
			}
	

			fonts->listChar[c].bci=bci;
			fonts->listChar[c].min=min;
			fonts->listChar[c].max=max;
		}
		else
		{	bci=fonts->listChar[c].bci;
			min=fonts->listChar[c].min;
			max=fonts->listChar[c].max;
		}



		x=x-min;
		if(x<0) x=0;


		bdd=image->buf+image->resx*y;
		for(i=0;i<sizeh;i++)
		{
			//bd=image->buf+image->resx*(y+i)+x;
			bd=bdd+x;
			bc=bci;

			if(y+i>=image->resy) break;  //clip horizontal para baixo
			if(y+i>0)
			{

			for(j=0;j<sizew;j++)
			{
				
				if(*bc!=COLOR_WHITE)	
				{
					//*bd=((*bc^0xFFFF)|cor);
					
					if(*bc==COLOR_BLACK)
					{
						*bd=(*bc|cor);
					}
					else
					{
						*bd=((*bc &*bd)| cor); //suavisa os contornos.

					} 

				}
				

				bc++;
				bd++;
			}
			}

			bci+=fonts->bmp.resx;
			bdd+=image->resx;

		}
		
		x+=((max))+fonts->spacew; //+3;
		str++;
	}
} 


/*============================================================
 F: Print text in Video view Clip
 =============================================================*/

int printImageTextClip(int x,int y,char *str,fontText *fonts,int cor,BMPimage *image)
{pixel *bd,*bs,*bc,*bci,*bdd;
 int i,j,c,xx;
 int cx,cy;
 int width,height;
 int max,min;
 int sizew,sizeh; 
 int printIn=1;


 int x1c,y1c,x2c,y2c;

	if(xclip1>=0)
	{
		x1c=xclip1;y1c=yclip1;x2c=xclip2;y2c=yclip2;
	}
	else
	{
		x1c=0;y1c=0;x2c=image->resx-1;y2c=image->resy-1;
	}

	if(fonts->bmp.buf==0)
		loadTGA(fonts->fileName,&fonts->bmp);

	sizew=fonts->sizew;
	sizeh=fonts->sizeh;

	width=fonts->bmp.resx/sizew;
	height=fonts->bmp.resy/sizeh;

	bs=fonts->bmp.buf;
	
	while(*str)
	{
		c=getCodeCar(*str);
			
		if(fonts->listChar[c].bci==0)
		{
			cx = ((c % width) ); 
			cy = ((c / width) ); 
			cx*=sizew;
			cy*=sizeh;

			max=0;
			min=255;
			bci=bs+fonts->bmp.resx*cy+cx;

			bd=bci;
			for(i=0;i<sizeh;i++)
			{
				bc=bci;
				for(j=0;j<sizew;j++)
				{
					if(*bc!=COLOR_WHITE)	
					{
						if(j>max) max=j;
						if(j<min) min=j;
					}
					bc++;
				}
				bci+=fonts->bmp.resx;
			}
			bci=bd;
			if(c==0)
			{
				min=0;max=sizew/4;
			}
	

			fonts->listChar[c].bci=bci;
			fonts->listChar[c].min=min;
			fonts->listChar[c].max=max;
		}
		else
		{	bci=fonts->listChar[c].bci;
			min=fonts->listChar[c].min;
			max=fonts->listChar[c].max;
		}

		x=x-min;
		//if(x<0) x=0;

		if(y>=y2c) return 0;  //clip horizontal para baixo

		bdd=image->buf+image->resx*y;
		for(i=0;i<sizeh;i++)
		{
			xx=x;
			//bd=image->buf+image->resx*(y+i)+x;
			bd=bdd+x;
			bc=bci;

			
		//	if(y+i>=clipArea->y2) break;  //clip horizontal para baixo

			if(y+i>0)
			{

			for(j=0;j<sizew;j++)
			{
				
			
				if(xx>=x1c)
				{
				if(*bc!=COLOR_WHITE)	
				{
					if(xx>=x2c) break; 
					 
					//*bd=((*bc^0xFFFF)|cor);
					
					if(*bc==COLOR_BLACK)
					{
						
						*bd=(*bc|cor);
					}
					else
					{	
						*bd=((*bc &*bd)| cor); //suavisa os contornos.

					} 
	
				}
				}
				xx++;
				

				bc++;
				bd++;
			}
			}

			bci+=fonts->bmp.resx;
			bdd+=image->resx;

		}
		
		x+=((max))+fonts->spacew; //+3;
		if(x>=x2c-fonts->sizeh/2) 
		{	x=x2c;
			break; 
		}
		str++;
	}

	return x;
} 

/*============================================================
 F: Print text in Video view Clip
 =============================================================*/
int fase=0,ffase;
long aniFase;

int printImageTextClipFase(int x,int y,char *str,fontText *fonts,int cor,BMPimage *image,boxArea *clipArea)
{pixel *bd,*bs,*bc,*bci;
 int i,j,c;
 int cx,cy;
 int width,height;
 int max,min;
 int sizew,sizeh; 
 int printIn=1;
 int yt;

 boxArea clip;

	if(clipArea==0)
	{
		clip.x1=0;
		clip.y1=0;
		clip.x2=image->resx-1;
		clip.y2=image->resy-1;
	}
	else
	{
		clip.x1=clipArea->x1;
		clip.y1=clipArea->y1;
		clip.x2=clipArea->x2;
		clip.y2=clipArea->y2;

	}

	if(fonts->bmp.buf==0)
		loadTGA(fonts->fileName,&fonts->bmp);

	sizew=fonts->sizew;
	sizeh=fonts->sizeh;

	width=fonts->bmp.resx/sizew;
	height=fonts->bmp.resy/sizeh;


	bs=fonts->bmp.buf;
	
	yt=y;

	while(*str)
	{
		c=getCodeCar(*str);
			
		if(fonts->listChar[c].bci==0)
		{
			cx = ((c % width) ); 
			cy = ((c / width) ); 
			cx*=sizew;
			cy*=sizeh;

			max=0;
			min=255;
			bci=bs+fonts->bmp.resx*cy+cx;

			bd=bci;
			for(i=0;i<sizeh;i++)
			{
				bc=bci;
				for(j=0;j<sizew;j++)
				{
					if(*bc!=COLOR_WHITE)	
					{
						if(j>max) max=j;
						if(j<min) min=j;
					}
					bc++;
				}
				bci+=fonts->bmp.resx;
			}
			bci=bd;
			if(c==0)
			{
				min=0;max=sizew/4;
			}
	

			fonts->listChar[c].bci=bci;
			fonts->listChar[c].min=min;
			fonts->listChar[c].max=max;
		}
		else
		{	bci=fonts->listChar[c].bci;
			min=fonts->listChar[c].min;
			max=fonts->listChar[c].max;
		}



		x=x-min;
		if(x<0) x=0;


		if(y>=clip.y2) return 0;  //clip horizontal para baixo

		ffase=fase+(x)*8;
		if(ffase>=ANGLE_360) ffase-=ANGLE_360;
		if(ffase>=ANGLE_360) ffase-=ANGLE_360;
		y=(int)(yt+(16*isen(ffase))/KDD_FIX);
		if(getCurrentTime()-aniFase>20)
		{
			aniFase=getCurrentTime();
			fase+=ANGLE_10;
			if(fase>=ANGLE_360) fase-=ANGLE_360;
		}

		for(i=0;i<sizeh;i++)
		{
			
			bd=image->buf+image->resx*(y+i)+x;
			bc=bci;

			
		//	if(y+i>=clipArea->y2) break;  //clip horizontal para baixo

			if(y+i>0)
			{

			for(j=0;j<sizew;j++)
			{
				
				if(*bc!=COLOR_WHITE)	
				{
					//*bd=((*bc^0xFFFF)|cor);
					
					if(*bc==COLOR_BLACK)
					{
						*bd=(*bc|cor);
					}
					else
					{
						*bd=((*bc &*bd)| cor); //suavisa os contornos.

					} 
	
				}
				

				bc++;
				bd++;
			}
			}

			bci+=fonts->bmp.resx;

		}
		
		x+=((max))+fonts->spacew; //+3;
		if(x>=clip.x2) break; 
		str++;
	}

	return x;
} 

/*============================================================
 F: get code caracter
 =============================================================*/

char acentuados[]="ÀÁÂÃÄÇÈÉÊËÌÍÎÏÒÓÔÕÖÙÚÛÜàáâãäçèéêëìíîïòóôõöùúûü";

int getCodeCar(char str)
{int c;
 char * pch;

	c=str-32;
	if(c>(127-32) || (c<0)) 
	{
		pch=strchr(acentuados,str);
		if(pch) 
		{
			c=(int)((pch-acentuados)+95);
		}
		else
			c='?'-32; 
	}
	return c;
}

/*============================================================
 F: fadeout image in Video view
 =============================================================*/

void fadeInViewImage(int xview,int yview,BMPimage *image)
{int tam,k,m;
 int rs,gs,bs,rd,gd,bd;
 pixel *ps,*pd;

 BMPimage clone;

	if(image)
	{	
		cloneImage(image,&clone);
		tam=image->resx*image->resy;

		memset(clone.buf,0,sizeof(pixel)*tam);

		for(k=0;k<32;k++)
		{
			putViewImage(xview,yview,&clone);	
			FlipVideo();
		//	t=1;
			//while(t--)
			WaitSyncTimer();

			ps=image->buf;
			pd=clone.buf;

			for(m=0;m<tam;m++) 
			{  
				rs=RED(*ps);
				gs=GREEN(*ps);
				bs=BLUE(*ps);
				
				rd=RED(*pd);
				gd=GREEN(*pd);
				bd=BLUE(*pd);

				{rd++; if(rd>rs) rd=rs;}
				{gd++; if(gd>gs) gd=gs;}
				{bd++; if(bd>bs) bd=bs;}
				
				*pd=PIXEL(rd,gd,bd);
				ps++;
				pd++;			
			}

		}

		freeImage(&clone);
	}
}


/*============================================================
 F: fadein image in Video view
 =============================================================*/

void fadeOutViewImage(int xview,int yview,BMPimage *image)
{int tam,k,t,m;
 int rs,gs,bs,rd,gd,bd;
 pixel *ps,*pd;

 BMPimage clone;

	if(image)
	{	
		cloneImage(image,&clone);
		tam=image->resx*image->resy;

		copyImage(image,&clone);

		for(k=0;k<32;k++)
		{
			putViewImage(xview,yview,&clone);	
			FlipVideo();
			t=1;
			while(t--)	WaitSyncTimer();

			ps=image->buf;
			pd=clone.buf;

			for(m=0;m<tam;m++) 
			{  
				rs=RED(*ps);
				gs=GREEN(*ps);
				bs=BLUE(*ps);
				
				rd=RED(*pd);
				gd=GREEN(*pd);
				bd=BLUE(*pd);

				{rd--; if(rd<0) rd=0;}
				{gd--; if(gd<0) gd=0;}
				{bd--; if(bd<0) bd=0;}
				
				*pd=PIXEL(rd,gd,bd);
				ps++;
				pd++;			
			}

		}

		freeImage(&clone);
	}
}


/*============================================================
 F: Read a TGA file (no compress 24 bits, RGB)
 =============================================================*/

int loadTGA(char *fileName,BMPimage *image) 
{char *buf=0,*bf;
 tgaheader_t *tga;

	short int width, height;
	unsigned char headerLength = 0;					
	unsigned char imageType = 0;				
	unsigned char r,g,b,a=255,bits = 0;						
	int format= 0;					
	int lineWidth = 0;	
	int ret=0;
	
	int y,x;	

	pixel *p,*pp;

	unsigned char id;
	unsigned char length;

	if(readFile(fileName, &buf)>0)
	{
		tga=(tgaheader_t *)buf;

		imageType=tga->imageType;
		width=tga->width;
		height=tga->height;
		bits=tga->bpp;

		if(image->buf!=0) 
		{
			myfree(image->buf);
			image->buf=0;
		}
		p=mymalloc(sizeof(pixel)*width*height); //GetVideo();

		if(p)
		{
			image->resx=tga->width;
			image->resy=tga->height; 

			image->resxOrg=image->resx;
			image->resyOrg=image->resy;
			image->offsetx=0;
			image->offsety=0;
			image->buf=p;

			bf=buf+sizeof(tgaheader_t);

			if(imageType != 10) 
			{		
				if((bits == 24)||(bits == 32)) 
				{ 
					format = bits >> 3; 
					lineWidth = format * width;
					for(y = height-1; y >=0; y--) 
					{
						pp=p+y*width;
						for(x=0;x<lineWidth;x+=format)
						{
							b=*bf++;
							g=*bf++;
							r=*bf++;
							if(tga->imageDesc==8) a=*bf++;
							if(a==0)
							//if(a<0xff)
							 *pp++=TRANSPARENT_COLOR;
							else 
							*pp++=PIXEL(r,g,b);
							
						}
					}
				}
			}
			else
			{
				if((bits == 24)||(bits == 32)) 
				{ 
					format = bits >> 3; 
					lineWidth = format * width;
					y = height-1;
					x=0;

					while(y>=0)
					{
										
						id=*bf++;

						if(id & 0x80)
						{
							length = (unsigned char)(id - 127);
							b=*bf++;
							g=*bf++;
							r=*bf++;
							if(tga->imageDesc==8) a=*bf++;
					
							pp = p+x+(width * y);
							while(length > 0)
							{
								if(a==0) 
								//if(a<0xff) 
									*pp++=TRANSPARENT_COLOR;
								else 
								*pp++=PIXEL(r,g,b);
						
								x++;
								if(x>=width)
								{
									x=0;
									y--;
									if(y<0) 
										goto fim;
									pp = p+(width * y);
								}	
								--length;
							}
						}
						else
						{
							length = (unsigned char)(id + 1);

							pp = p+x+(width * y);
							while(length > 0)
							{
								b=*bf++;
								g=*bf++;
								r=*bf++;
								if(tga->imageDesc==8) a=*bf++;
								if(a==0)
								//if(a<0xff)
									*pp++=TRANSPARENT_COLOR;
								else 
									*pp++=PIXEL(r,g,b);
							
								x++;
								if(x>=width)
								{
									x=0;
									y--;
									if(y<0) 
										goto fim;
									pp = p+(width * y);
								}	
								--length;
							}
						}
					}
				}
			}
fim:			ret=1;
		}
	}

	myfree(buf); 

	return(ret);
}

/*============================================================
 F: Auto Cut image 
 =============================================================*/

void autoCutImage(BMPimage *image)
{int x,y,xmin,xmax,ymin,ymax,resx,resy;
 pixel c,*p,*pp;

 pixel *buf;
 
	if(image->buf)
	{	
		xmax=0;ymax=0;
		xmin=image->resx+1;	ymin=image->resy+1;

		p=image->buf;
		for(y=0;y<image->resy;y++)
		{
			for(x=0;x<image->resx;x++)
			{
				c=*p++;
				if(c!=TRANSPARENT_COLOR) 
				{	if(x<xmin) xmin=x;
					if(x>xmax) xmax=x;
					if(y<ymin) ymin=y;
					if(y>ymax) ymax=y;
				}

			}
		}
	}

	resx=xmax-xmin;
	resy=ymax-ymin;

	buf=mymalloc(sizeof(pixel)*resx*resy);
	pp=buf;
	
	for(y=ymin;y<ymax;y++)
	{
		p=image->buf+xmin+y*image->resx;
		for(x=xmin;x<xmax;x++)
		{
			*pp++=*p++;
		}
	}

	myfree(image->buf);
	image->buf=buf;
	image->resx=resx;
	image->resy=resy;
	image->offsetx=xmin;
	image->offsety=ymin;
//	image->resxOrg=image->resx;    // se fizer isto nao ajusta corretamente a animacao
//	image->resyOrg=image->resy;


//	image->offsetx=0;
//	image->offsety=0;
	
//	drawRetangle(0,0,resx-1,resy-1,0xffffff,image); 
}

/*============================================================
 F: Put image in buffer image no transparent color
 =============================================================*/

void putImageSolid(int xp,int yp,BMPimage *image,BMPimage *frame)
{int rvx,rvy,rx,ry,y;
 pixel *pd,*ps,*p;
 int sizex,sizey;


	if(image)
	{	
		pd=frame->buf;
		ps=image->buf;
		rvx=frame->resx;
		rvy=frame->resy;
		rx=image->resx;
		ry=image->resy;

		p=pd+(yp)*rvx+xp;
		if(rx<rvx)
			sizex=rx*sizeof(pixel);
		else
			sizex=rvx*sizeof(pixel);

		if(ry<rvy)
			sizey=ry;
		else
			sizey=rvy;

		for(y=0;y<sizey;y++)
		{
			memcpy(p,ps,sizex);
			ps+=rx;
			p+=rvx;

		}
	}

}


/*============================================================
 F: Put image in buffer image
 =============================================================*/

int putImage(int xp,int yp,BMPimage *image,BMPimage *frame,pixel colorTrans)
{int rvx,rvy,rx,ry,x,y,xt,yt;
 pixel *pd,*ps,*p,*pp,c;
 int count=0;

	if(image)
	{	
		pd=frame->buf;
		ps=image->buf;
		rvx=frame->resx;
		rvy=frame->resy;
		rx=image->resx;
		ry=image->resy;


		pd=pd+(yp)*rvx+xp;
		yt=yp+ry;
		xt=xp+rx;

		if(yt<0) 
			return count;
		if(xt<0)
			return count;
		
		for(y=yp;y<yt;y++)
		{
			if(y>=0)
			{
				if(y>=rvy) return count;

				p=pd;
				pp=ps;

				for(x=xp;x<xt;x++)
				{
					if(x>=0)
					{
						if(x>=rvx) break;

						c=*pp;
						if(c!=colorTrans) 
						{
							*p=c;
						}
						count++;
					}

					p++;
					pp++;
				}
			}
			pd+=rvx;
			ps+=rx;
		}
	}

	return count;
}


/*============================================================
 F: Put image in buffer image
 =============================================================*/

void putMaskImage(int xp,int yp,BMPimage *image,BMPimage *frame,pixel colorTrans,BMPimage *mask,pixel colorMask)
{int rvx,rvy,rx,ry,x,y;
 pixel *pd,*ps,*p,*pp,c,*pmask,*pm;

	if(image)
	{	
		pd=frame->buf;
		pmask=mask->buf;
		ps=image->buf;
		rvx=frame->resx;
		rvy=frame->resy;
		rx=image->resx;
		ry=image->resy;

		for(y=0;y<ry;y++)
		{
			if((yp+y)>=0)
			{
				if((yp+y)>=rvy) return;

				p=pd+(y+yp)*rvx+xp;
				pm=pmask+(y+yp)*rvx+xp;

				pp=ps+(y)*rx;

				for(x=0;x<rx;x++)
				{
					if((xp+x)>=0)
					{
						if((xp+x)>=rvx) break;
						
						if(*pm!=colorMask)
						{
							c=*pp;
							if(c!=colorTrans) *p=c;
						}
					}

					p++;
					pp++;
					pm++;
				}
			}
		}

	}
}

/*============================================================
 F: Draw Retangle Fast (no clip)
 =============================================================*/

void drawRetangleFast(int x1,int y1,int x2,int y2,pixel color,BMPimage *image) 
{pixel *bp1;
 pixel *bp2;
 int x,y,resx,resy;

	resx=image->resx;
	resy=image->resy;

	if(x1<0) x1=0;
	if(x2<0) x2=0;
	if(y1<0) y1=0;
	if(y2<0) y2=0;

	if(x1>=resx) x1=resx-1;
	if(x2>=resx) x2=resx-1;
	if(y1>=resy) y1=resy-1;
	if(y2>=resy) y2=resy-1;

	bp1=image->buf+resx*y1+x1;
	bp2=image->buf+resx*y2+x1;
	
    for(x=x1;x<=x2;x++)
	{
		*bp1++=color;
		*bp2++=color;
	}

	bp1=image->buf+resx*y1+x1;
	bp2=image->buf+resx*y1+x2;

	
	for(y=y1;y<=y2;y++)
	{
		
		*bp1=color;
		*bp2=color;
		bp1+=resx;
		bp2+=resx;
	}
}

/*============================================================
 F: Draw Retangle
 =============================================================*/

void drawRetangle(int x1,int y1,int x2,int y2,pixel color,BMPimage *image) 
{

	drawLineH(x1,y1,x2,color,image);
	drawLineH(x1,y2,x2,color,image);
	drawLineV(x1,y1,y2,color,image);
	drawLineV(x2,y1,y2,color,image);
}

/*============================================================
 F: Draw box 
 =============================================================*/

void drawBOX(int x1,int y1,int x2,int y2,pixel color,int flagAlpha,BMPimage *image) 
{
 pixel *bd,*bdd;
 int x,y;

 int x1c,y1c,x2c,y2c;

	if(xclip1>=0)
	{
		x1c=xclip1;y1c=yclip1;x2c=xclip2;y2c=yclip2;
	}
	else
	{
		x1c=0;y1c=0;x2c=image->resx;y2c=image->resy;
	}

	if((y2<y1c) | (y1>y2c)) return;
	if((x1>x2c) | (x2<x1c)) return;
	if(x1<x1c) x1=x1c;
	if(x2>x2c) x2=x2c;
	if(y1<y1c) y1=y1c;
	if(y2>y2c) y2=y2c;

	bdd=image->buf+image->resx*y1+x1;

	if(flagAlpha==TRANS_COLOR)
	{
		for(y=y1;y<y2;y++)
		{
			bd=bdd;
			if(y & 1)
			{
				for(x=x1;x<x2;x++)
				{
					if(x & 1)
						*bd=color;
					bd++;
				}
				bdd+=image->resx;
			}
			else
			{
				for(x=x1;x<x2;x++)
				{
					if(!(x & 1))
						*bd=color;
					bd++;
				}
				bdd+=image->resx;
			}
		}		
	}
	else
	{
		for(y=y1;y<y2;y++)
		{
			bd=bdd;
			for(x=x1;x<x2;x++)
			{
				*bd++=color;
			}
			bdd+=image->resx;
		}
	}
}


/*============================================================
 F: Draw line horizontal
 =============================================================*/

void drawLineH(int x1,int y1,int x2,pixel color,BMPimage *image) 
{int x1c,y1c,x2c,y2c,i;
 pixel *p;

	if(xclip1>=0)
	{
		x1c=xclip1;y1c=yclip1;x2c=xclip2;y2c=yclip2;
	}
	else
	{
		x1c=0;y1c=0;x2c=image->resx;y2c=image->resy;
	}

	if((y1<y1c) | (y1>y2c)) return;
	if((x1>x2c) | (x2<x1c)) return;

	if(x1<x1c) x1=x1c;
	if(x2>x2c) x2=x2c;

	p=image->buf+y1*image->resx+x1;

	for(i=x1;i<=x2;i++) *p++=color;
}

/*============================================================
 F: Draw line vertical
 =============================================================*/

void drawLineV(int x1,int y1,int y2,pixel color,BMPimage *image) 
{int x1c,y1c,x2c,y2c,i;
 pixel *p;

	if(xclip1>=0)
	{
		x1c=xclip1;y1c=yclip1;x2c=xclip2;y2c=yclip2;
	}
	else
	{
		x1c=0;y1c=0;x2c=image->resx;y2c=image->resy;
	}

	if((x1<x1c) | (x1>x2c)) return;
	if((y1>y2c) | (y2<y1c)) return;

	if(y1<y1c) y1=y1c;
	if(y2>y2c) y2=y2c;

	p=image->buf+y1*image->resx+x1;

	for(i=y1;i<=y2;i++)
	{	*p=color;
		p+=image->resx;
	}
}

/*============================================================
 F: Draw line
 =============================================================*/

void drawLine(int x1,int y1,int x2,int y2,pixel color,BMPimage *image) 
{short int dx,dy,dxabs,dyabs,i,px,py,sdx,sdy,x,y;
 int mx,my;


	if(!image) return;
			
	mx=(x2-x1)+1;if(mx<0) mx=-mx;
	my=(y2-y1)+1;if(my<0) my=-my;


	dx=x2-x1;dy=y2-y1;
	sdx=1;sdy=1;
	dxabs=dx;
	dyabs=dy;
	if(dx<0)
	{	 sdx=-1;
		 dxabs=-dx;
	}

	if(dy<0)
	{	sdy=-1;
		dyabs=-dy;
	}

	x=0;y=0;
	px=x1;py=y1;

	if (dxabs >= dyabs)
	{	
		for (i=0;i<=dxabs;i++)
		{	y+=dyabs;
			if (y>=dxabs)
			{	y-=dxabs;
				py+=sdy;
			}
			drawPixel(px,py,color,image);
			px+=sdx;

		}
	}
	else
	{	
		for (i=0;i<=dyabs;i++)
		{	x+=dxabs;
			if (x>=dyabs)
			{	x-=dyabs;
				px+=sdx;
				
			}
			drawPixel(px,py,color,image);
			py+=sdy;
		}
	}
}

/*============================================================
 F: Draw pixel
 =============================================================*/

void drawPixel(int x,int y,pixel color,BMPimage *image) 
{pixel *p;

	if(!image) return;

	if(xclip1>=0)
	{
		if((y<yclip1) || (y>=yclip2) ||(x<xclip1) || (x>=xclip2) ) return;
	}
	else
	{
		if((y<0) || (y>=image->resy) ||(x<0) || (x>=image->resx) ) return;
	}

	p=image->buf+y*image->resx+x;

	*p=color;
}

/*============================================================
 F: read pixel
 =============================================================*/

pixel readPixel(int x,int y,BMPimage *image) 
{pixel *p;

	if(!image) return 0;

	if((y<0) || (y>=image->resy) ||(x<0) || (x>=image->resx) ) return 0;

	p=image->buf+y*image->resx+x;

	return (*p);
}


/*============================================================
 F: Get opt image to specific view
 =============================================================*/

void getOptImage(BMPimage *image,BMPimage *view,pixel colorTrans)
{int x,y,count=0;
 pixel c,*p,*pp;
 sOptImage *so;
 int ds,dss=0,dd,ddd=0;
 int size=0,sizet;
 int flg=0;

	if(image->buf)
	{	
		image->sizeOptImage=image->resy;
		image->optImage=mymalloc(sizeof(sOptImage)*image->sizeOptImage);

		so=image->optImage;
		
		pp=image->buf;
		for(y=0;y<image->resy;y++)
		{
			dd=ddd;
			ds=dss;
			size=0;
			p=pp;
			flg=0;
			for(x=0;x<image->resx;x++)
			{
				c=*p++;
				
				switch(flg)
				{
					case 0:
					if(c!=colorTrans) 
					{ 
						so->ds=ds;
						so->dd=dd;
						so->dx=x;
						size=1;
						flg=1;
					}
					ds++;
					dd++;
					break;
				
					case 1:
					if(c==colorTrans) 
					{	flg=2;
						sizet=1;
					}
					else size++;
					break;
	
					case 2:
					if(c!=colorTrans) 
					{
						flg=1;
						size+=sizet;

						//myfree(image->optImage);
						//image->optImage=0;
						//image->sizeOptImage=0;
						//return;
					}
					break;
				}
			}
			so->size=size;
			so++;

			ddd+=view->resx;
			dss+=image->resx;
			pp+=image->resx;
			
		}
	}

}


/*============================================================
 F: Put image Optimize in buffer image
 =============================================================*/

void putOptImage(int xp,int yp,BMPimage *image,BMPimage *frame,pixel colorTrans)
{int rvx,rvy,size,tx;
 pixel *pd,*ps,*pdd,*pss,c;
 sOptImage *so;

	if(image)
	{	
		pd=frame->buf;
		ps=image->buf;
		rvx=frame->resx;
		rvy=frame->resy;

		pd=pd+(yp)*rvx+xp;

		c=image->sizeOptImage;

		so=image->optImage;
		
		while(c--)
		{
			if(yp>0)
			{
				if(yp>=rvy) return;
				
				pdd=pd+so->dd;
				pss=ps+so->ds;
				tx=xp+so->dx;

				if(tx<rvx) 
				{
					if(tx>=0)
					{
						size=so->size;
						if(tx+size>rvx) 
							size-=tx+size-rvx;
						
					}
					else
					{	size=so->size;
						tx=-tx;
						size-=tx;
						pdd+=tx;
						pss+=tx;
					}

					if(size>0)
							memcpy(pdd,pss,size*sizeof(pixel));
				}
			}
			yp++;
			so++;
		};
	}
}
