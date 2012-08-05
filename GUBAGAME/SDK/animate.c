#include "so.h"
#include "graphic.h"
#include "math.h"
#include "animate.h"

/*============================================================
 F: Load Animated3D
 =============================================================*/

void loadAnimate3D(Animate3D *ani,BMPimage *view)
{char nameFile[200];
 int i,j;
 char *angles[ANGLES3D]={"000","045","090","135","180","225","270","315"};
 
	if(ani)
	{

		if(ani->lastCurrentTime!=0) return; // Animaçao ja foi inicializada por outro ator

		for(j=0;j<ANGLES3D;j++)
		{
			i=0;
			if(j==0) 
				ani->numFrame=0;
			do
			{
				//sprintf(nameFile,"%s/%s/%04d.tga",ani->fileFrame,angles[j],i);	
				//sprintf(nameFile,"%s/%s/%s-%s-%04d.tga",ani->fileFrame,angles[j],&ani->fileFrame[getIndexLastBar(ani->fileFrame)],angles[j],i);
				sprintf(nameFile,"%s_%s_%04d.tga",&ani->fileFrame[getIndexLastBar(ani->fileFrame)],angles[j],i);
				if(loadTGA(nameFile,&ani->frame[j][i]))	
				{	
					autoCutImage(&ani->frame[j][i]);
					if(ani->flgOpt)
						getOptImage(&ani->frame[j][i],view,ani->colorTrans);

					if(ani->cache==NOCACHE)
					{
						myfree(ani->frame[j][i].buf);
						ani->frame[j][i].buf=0;
					}

					if(j==0)
					{
						ani->actualFrame=0;
						ani->lastCurrentTime=getCurrentTime();
						//ani->numFrame++;
						ani->numFrame+=ani->passFrame;

					}
				}
				else break;

				//i++;
				i+=ani->passFrame;

			}while(i<NUMMAXFRAMES);
		}
	}
}

/*============================================================
 F: print Animated3D
 =============================================================*/

void printAnimate3D(int x,int y,int angle,Animate3D *ani,BMPimage *view,int ctrAni)
{BMPimage *fr;

 char nameFile[200];
 char *angles[ANGLES3D]={"000","045","090","135","180","225","270","315"};
 

	if(ani)
	{

	
		fr=&ani->frame[angle/ANGLERESOLUTION][ani->actualFrame];

		if(ani->cache==NOCACHE)
		{
			sprintf(nameFile,"%s_%s_%04d.tga",&ani->fileFrame[getIndexLastBar(ani->fileFrame)],angles[angle/ANGLERESOLUTION],ani->actualFrame);

			if(loadTGA(nameFile,fr))
			{
				autoCutImage(fr);
				if(ani->flgOpt)
					getOptImage(fr,view,ani->colorTrans);
			}
		}
		else
		{
			if(fr->buf==0)  // para animacoes 3d sem os frames para cada angulo (apenas o zero)
				fr=&ani->frame[0][ani->actualFrame];
		}


		x=x+fr->offsetx-fr->resxOrg/2;  // Tem um problema aqui A imagem tem que estar centralizada
		y=y+fr->offsety-fr->resyOrg;

		if(ani->flgOpt)
			putOptImage(x,y,fr,view,ani->colorTrans);
		else
			putImage(x,y,fr,view,ani->colorTrans);
		
	
		if(ani->cache==NOCACHE)
		{
			myfree(fr->buf);
			fr->buf=0;
		}
		/* -----------------------------------------------------
		Aqui nao é o melhor lugar para isto mudar para actor */
		
		if(ctrAni)
		{
		if(getCurrentTime()-ani->lastCurrentTime>ani->delayFrame)
		{
			if(ani->type!=ANIPINGPONG) ani->direction=0;

			ani->lastCurrentTime=getCurrentTime();
			if(ani->direction==0) //ani->actualFrame++;
				ani->actualFrame+=ani->passFrame;
			else	
				//ani->actualFrame--;
				ani->actualFrame-=ani->passFrame;

			if(ani->actualFrame>=ani->numFrame) 
			{
				if(ani->type==ANIPINGPONG)
				{
					ani->direction=1;
				//	ani->actualFrame--;
					ani->actualFrame-=ani->passFrame;
				}
				else
					ani->actualFrame=0;
			}

			if(ani->actualFrame<0)
			{
				ani->actualFrame=0;
				ani->direction=0;

			}
		}
		}
		/* -------------------------------------------------------- */
		
	}
}

/*============================================================
 F: print Animated3D Mask
 =============================================================*/

void printMaskAnimate3D(int x,int y,int angle,Animate3D *ani,BMPimage *view,BMPimage *mask,pixel colorMask)
{
	if(ani)
	{
		putMaskImage(x,y,&ani->frame[angle/ANGLERESOLUTION][ani->actualFrame],view,ani->colorTrans,mask,colorMask);

		if(getCurrentTime()-ani->lastCurrentTime>ani->delayFrame)
		{
			ani->lastCurrentTime=getCurrentTime();
			ani->actualFrame++;
			if(ani->actualFrame>=ani->numFrame) ani->actualFrame=0;
		}
	}
}

/*============================================================
 F: free Animated3D
 =============================================================*/

void freeAnimate3D(Animate3D *ani)
{int i,j;

	if(ani)
	{
		for(j=0;j<ANGLES3D;j++)
		{
			for(i=0;i<ani->numFrame;i++)
			{
				if(ani->frame[j][i].buf)
				{
					myfree(ani->frame[j][i].buf);
					ani->frame[j][i].buf=0;
				}
			}
		}
	}
}


/*============================================================
 F: Load Animated
 =============================================================*/

void loadAnimate2D(Animate2D *ani,BMPimage *view)
{char nameFile[200];
 int i=0,p=0,size;

	if(ani)
	{
		if(ani->numFrame==0) 
		{	p=1;
			size=NUMMAXFRAMES;
		}
		else
			size=ani->numFrame;
		do
		{
			sprintf(nameFile,"%s%d.tga",ani->fileFrame,i);			
			if(loadTGA(nameFile,&ani->frame[i]))	
			{		
				if(ani->flgOpt)
					getOptImage(&ani->frame[i],view,ani->colorTrans);

				ani->actualFrame=0;
				ani->lastCurrentTime=getCurrentTime();
				ani->numFrame+=p;
			}
			else break;

			i++;

		}while(i<size);
	}
}

/*============================================================
 F: print Animated
 =============================================================*/

void printAnimate2D(int x,int y,Animate2D *ani,BMPimage *view)
{char nameFile[200];
 int idxFrame=0;

	if(ani)
	{

		if((getCurrentTime()-ani->lastCurrentTime>ani->delayFrame) || (ani->lastCurrentTime==0))
		{

			if(ani->lastCurrentTime==0) 
				ani->lastCurrentTime=getCurrentTime();
			else
			{
				ani->lastCurrentTime=getCurrentTime();
				ani->actualFrame++;
				if(ani->actualFrame>=ani->numFrame) ani->actualFrame=0;
			}

			
			if(ani->flgCache)
			{
				if(ani->frame[ani->actualFrame].buf==0)
				{
					sprintf(nameFile,"%s%d.tga",ani->fileFrame,ani->actualFrame);
					loadTGA(nameFile,&ani->frame[ani->actualFrame]);
					if(ani->flgOpt)
						getOptImage(&ani->frame[ani->actualFrame],view,ani->colorTrans);
				}

			}
			else
			{
				sprintf(nameFile,"%s%d.tga",ani->fileFrame,ani->actualFrame);
				if(ani->numFrame<=1)
				{
					if(ani->frame[0].buf==0) 
					{
						loadTGA(nameFile,&ani->frame[0]);
						if(ani->flgOpt)
							getOptImage(&ani->frame[0],view,ani->colorTrans);
					}
				}
				else
				{
					loadTGA(nameFile,&ani->frame[0]);
					if(ani->flgOpt)
						getOptImage(&ani->frame[0],view,ani->colorTrans);
				}
			}


		}

		if(ani->flgCache)
				idxFrame=ani->actualFrame;


		if(ani->colorTrans==SOLID_COLOR)
			putImageSolid(x,y,&ani->frame[idxFrame],view);
		else
		{
			if(ani->flgOpt)
				putOptImage(x,y,&ani->frame[idxFrame],view,ani->colorTrans);
			else
				putImage(x,y,&ani->frame[idxFrame],view,ani->colorTrans);

		}
	}
}

/*============================================================
 F: free Animated
 =============================================================*/

void freeAnimate2D(Animate2D *ani)
{int i;

	if(ani)
	{
		for(i=0;i<ani->numFrame;i++)
		{
			if(ani->frame[i].buf)
			{
				myfree(ani->frame[i].buf);
				ani->frame[i].buf=0;
			}
		}
	}
}

