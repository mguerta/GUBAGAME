#include "so.h"
#include "game.h"


/*============================================================
D: Data
=============================================================*/
void projection(void);
void unProjection(int x,int y,float *x1,float *z1);

void controlActorTest(gameLevel *gl,Actor *ac);
void controlActorObject(gameLevel *gl,Actor *ac);
void controlActorMain(gameLevel *gl,Actor *ac);

fontText fonts[]={"./res/fontgllb17.tga",29,32,4};

Animate3D aniLunchBox={"./res/lunchbox",COLOR_BLACK,1};
Animate3D aniGirlStand={"./res/girl/stand",COLOR_BLACK,1};
Animate3D aniGirlWalk={"./res/girl/walk",COLOR_BLACK,50};

Animate3D aniSofa={"./res/sofa",0x7098,1};

Animate3D aniMirror={"./res/mirror",0x7098,1};

Action lunchBoxStand={&aniLunchBox};
Action girlStand={&aniGirlStand};
Action girlWalk={&aniGirlWalk};

Action sofaStand={&aniSofa};
Action mirrorStand={&aniMirror};

Actor sofa={0,0,ANGLE_0,"",&controlActorObject,1,&sofaStand};
Actor mirror={0,0,ANGLE_0,"",&controlActorObject,1,&mirrorStand};

Actor lunch0={0,0,ANGLE_0,"Pao",&controlActorObject,1,&lunchBoxStand};
Actor lunch1={0,0,ANGLE_0,"Queijo",&controlActorObject,1,&lunchBoxStand};
Actor lunch2={0,0,ANGLE_0,"leite",&controlActorObject,1,&lunchBoxStand};
Actor lunch3={0,0,ANGLE_0,"Cha",&controlActorObject,1,&lunchBoxStand};
Actor girl2={588,375,ANGLE_45,"Joana",&controlActorTest,2,&girlStand,&girlWalk};
Actor girl1={450,360,ANGLE_0,"Claudia",&controlActorTest,2,&girlStand,&girlWalk};
Actor girl0={0,0,ANGLE_45,"Magali",&controlActorMain,2,&girlStand,&girlWalk};

Actor g2={150,360,ANGLE_0,"Claudia",&controlActorTest,2,&girlStand,&girlWalk};
Actor g3={250,360,ANGLE_0,"Claudia",&controlActorTest,2,&girlStand,&girlWalk};
Actor g4={350,360,ANGLE_0,"Claudia",&controlActorTest,2,&girlStand,&girlWalk};
Actor g5={450,360,ANGLE_0,"Claudia",&controlActorTest,2,&girlStand,&girlWalk};
Actor g6={550,360,ANGLE_0,"Claudia",&controlActorTest,2,&girlStand,&girlWalk};
Actor g7={650,360,ANGLE_0,"Claudia",&controlActorTest,2,&girlStand,&girlWalk};
Actor g8={750,360,ANGLE_0,"Claudia",&controlActorTest,2,&girlStand,&girlWalk};
Actor g9={850,360,ANGLE_0,"Claudia",&controlActorTest,2,&girlStand,&girlWalk};



char *rotes[NUMMAXROTES][NUMMAXROTES]=
{ "0",//0 - 0
  "0,1",//0 - 1
  "0,2",//0 - 2
  "0,2,3",//0 - 3
  "0,2,3,4",//0 - 4
  "1,0",//1 - 0
  "1",//1 - 1
  "1,0,2",//1 - 2
  "1,0,2,3",//1 - 3
  "1,0,2,3,4",//1 - 4
  "2,1,0",//2 - 0
  "2,0,1",//2 - 1
  "2",//2 - 2
  "2,3",//2 - 3
  "2,3,4",//2 - 4
  "3,2,1,0",//3 - 0
  "3,2,0,1",//3 - 1
  "3,2",//3 - 2
  "3",//3 - 3
  "3,4",//3 - 4
  "4,3,2,1,0",//4 - 0
  "4,3,2,0,1",//4 - 1
  "4,3,2",//4 - 2
  "4,3",//4 - 3
  "4",//4 - 4
};

pointRote listPoints[]={340,529,0xe540e3,  //0
					    217,440,0xe540e3,  //1
						543,433,0xe540e3,  //2
						367,281,0xe540e3,  //3
						977,297,0xe540e3,}; //4 

gameLevel levelTest=
{"./res/cenarionocomp.tga","./res/cenarionomask.tga","./res/rio_de_hola.wav",
0,VIDEO_HEIGHT-85-20,MASKCOLOR,"./res/score.tga",
40,40,MASKCOLOR,"./res/lives.tga",
30,440,COLOR_WHITE,
//&girl1,3,1,&sofa,3,&mirror,0,&girl1}; 
&girl1,8,0,&girl1,0,&g2,0,&g3,0,&g4,0,&g5,0,&g6,0,&g7,0,&g8};

gameLevel *actualLevel;

int flgRun;

 int ag1=393 /*450 */,ag2=452 /*450*/;


/*============================================================
 F: Game Initialization
 =============================================================*/

int gameInit(void)
{
	
	actualLevel=setLevel(&levelTest);
	flgRun=1;

     printImageText(0,0,"i",&fonts[0],COLOR_WHITE,&actualLevel->frame); // força carga do texto antes

	//fadeInViewImage(actualLevel->xview,actualLevel->yview,&actualLevel->frame);

	return 0;

}

/*============================================================
 F: Set level
 =============================================================*/

gameLevel * setLevel(gameLevel *gl)
{
	if(gl)
	{
		loadTGA(gl->fileMaskBackground,&gl->maskBackground);

		loadTGA(gl->fileBackground,&gl->background);

		loadTGA(gl->fileScore,&gl->score);

		loadTGA(gl->fileLives,&gl->lives);

		cloneImage(&gl->background,&gl->frame);

		copyImage(&gl->background,&gl->frame);

		setMusic(gl->music,REPEATMUSIC); // repeat background music
		gl->xview=0;
		gl->yview=gl->background.resy-GetVideoHeight()-1;
		actualLevel=gl;

		gl->view.buf=GetVideo();
		gl->view.resx=GetVideoWidth();
		gl->view.resy=GetVideoHeight();

		initActors(gl);
	}

	return gl;
}

/*============================================================
 F: reset level
 =============================================================*/

void resetLevel(gameLevel *gl)
{
	if(gl)
	{	
		freeImage(&gl->maskBackground);
		freeImage(&gl->background);
		freeImage(&gl->frame);
		gl->xview=0;
		gl->yview=0;

		freeActors(gl);
	}
}


/*============================================================
 F: Game loop TESTE BARRA DE CORES DO EDGARD
 =============================================================*/

void gameMain_OLD2(void)
{ unsigned int joy;
  tmouse *mo;
  char text[100],time[20];
  pixel color;
  int sound,i;
  float xf=0,zf=0,yf=0;
  int fps=0,fpsg=0;
  int xv,yv;
  int draw;
  
	SetSyncTimer(60);   //SET 60 FPS 

	actualLevel=&levelTest;
	loadTGA("./res/Bar-color.tga",&actualLevel->background);
	xv=actualLevel->xview;
    yv=actualLevel->yview;

	putViewImage(xv,yv,&actualLevel->background);
//	startMusicMenu("./res/Sonic_3_Kazan_16K.mp3",1);
	setMusic(actualLevel->music,REPEATMUSIC); 
	do
	{
	 //   mp3_play();
		RenderAudio();

		joy=GetJoystick();
		if(joy & BTN_UP) 
		{	loadTGA("./res/Bar-color.tga",&actualLevel->background);
			putViewImage(xv,yv,&actualLevel->background);
		}
		if(joy & BTN_DOWN) 
		{	loadTGA("./res/Bar-gray.tga",&actualLevel->background);
			putViewImage(xv,yv,&actualLevel->background);
		}

	
		FlipVideo();
        WaitSyncTimer();

	}while(1);
}

/*============================================================
 F: Game loop - PRIMEIRO PROTOTIPO DE JOGO PERSPECTIVA ISOMETRICA
 =============================================================*/

void gameMain(void)
{ unsigned int joy;
  tmouse *mo;
  char text[100],time[20];
  pixel color;
  int sound,i;
  float xf=0,zf=0,yf=0;
  int fps=0,fpsg=0;
  int xv,yv;
  int draw;
  

	SetSyncTimer(60);   //SET 60 FPS 
	gameInit();

//	loadAnimate3D(&aniGirlWalk);
//	loadAnimate3D(&aniGirlStand);

	do
	{
		  joy=GetJoystick();
		  mo=getMouseInf();

		  actualLevel->mouse=mo;
		  actualLevel->key=joy;

		  draw=getDrawFrame();
		//  copyImage(&actualLevel->background,&actualLevel->frame);
		  xv=actualLevel->xview;
		  yv=actualLevel->yview;
		 // if(draw) copyImageView(xv,yv,720,480,&actualLevel->background,&actualLevel->frame);
		  if(draw) copyImageView(xv,yv,&actualLevel->background,&actualLevel->frame);
		  
		  //printImageText(10,10,"TEXTO NO FUNDO",&fonts[0],COLOR_RED,&actualLevel->frame);

		  mo->x=actualLevel->actorMain->xw;
		  mo->y=actualLevel->actorMain->yw;
		  calcViewCamera(actualLevel,mo);

		  runActors(actualLevel,draw);

		  if(draw)
		  {
			putImage(xv+actualLevel->xlives,yv+actualLevel->ylives,&actualLevel->lives,&actualLevel->frame,actualLevel->colorTransLives);
			putImage(xv+actualLevel->xscore,yv+actualLevel->yscore,&actualLevel->score,&actualLevel->frame,actualLevel->colorTransScore);

	  	    sprintf(text,"%d/%d",fpsg,fps);
		    printImageText(xv+560,yv+425-10,text,&fonts[0],COLOR_WHITE,&actualLevel->frame);
		
		  }
		  if(draw) putViewImage(xv,yv,&actualLevel->frame);

		//  putImage(actualLevel->xlives,actualLevel->ylives,&actualLevel->lives,&actualLevel->view,actualLevel->colorTransLives);
		//  putImage(actualLevel->xscore,actualLevel->yscore,&actualLevel->score,&actualLevel->view,actualLevel->colorTransScore);
		//  color=readPixel(mo->x,mo->y,&actualLevel->view);

		  getStringTime(getCurrentTime(),&time[0]);
//		  sprintf(text,"Time %s X = %d Y= %d S= %d ag1 =%d ag2 =%d",time,mo->x+actualLevel->xview,mo->y+actualLevel->yview,mo->status,ag1,ag2);
//		  printImageText(10,actualLevel->view.resy-50,text,&fonts[0],COLOR_WHITE,&actualLevel->view);

		//  sprintf(text,"%s",time);
		//  printImageText(actualLevel->xTime,actualLevel->yTime,text,&fonts[0],actualLevel->colorTime,&actualLevel->view);
		 // sprintf(text,"color= %x xf= %f zf=%f",color,xf,zf);
		 // printImageText(10,actualLevel->view.resy-30,text,&fonts[0],COLOR_WHITE,&actualLevel->view);
		 // printAnimate3D(10,400,0,&aniGirlStand,&actualLevel->view);	

		 // projection();

		 // unProjection(girl1.xo,girl1.yo,&xf,&zf);

		   if(getRenderAudio())
            sound=RenderAudio();

		  // i=sound/4096;
		  //drawRetangle(actualLevel->view.resx/2-i,actualLevel->view.resy/2-i,actualLevel->view.resx/2+i,actualLevel->view.resy/2+i,COLOR_GREEN,&actualLevel->view); 

		  if(draw) FlipVideo();
	      WaitSyncTimer();


  		  if(joy==BTN_EXIT) 
			  break;

		  if(joy==BTN_UP) ag1++;
		  if(joy==BTN_DOWN) ag1--;

		  if(joy==BTN_RIGHT) ag2++;
		  if(joy==BTN_LEFT) ag2--;

		  if(getFPS(&fpsg,&fps))
			printf("FPS %d/%d \n",fpsg,fps);

		

		  watchdog(1000);

	}while(flgRun);


//	freeAnimate3D(&aniGirlWalk);
//	freeAnimate3D(&aniGirlStand);

	gameClose();
}



/*============================================================
 F: unProjection
 =============================================================*/

void unProjection(int x,int y,float *x1,float *z1)
{float xt,yt,zt;
 float xf,yf;

    xf=x;
	yf=y;

 	zt=(yf*fcos(ag2)-xf*fsen(ag1)*fsen(ag2))/(fcos(ag2)*fsen(ag1)*fcos(ag2)-fsen(ag2)*fsen(ag1)*fsen(ag2));
	xt=(xf+zt*fsen(ag2))/fcos(ag2);	

	*x1=xt;
	*z1=zt;
}

/*============================================================
 F: Projection
 =============================================================*/
#define TAMQUAD 500
void projection(void)
{coord3D plane[4]=
{0,0,0,
TAMQUAD,0,0,
0,0,-TAMQUAD,
TAMQUAD,0,-TAMQUAD};

 int x[4];
 int y[4];
 float xf,yf;
 int i;


	for(i=0;i<4;i++)
	{
		xf=plane[i].x*fcos(ag2)-plane[i].z*fsen(ag2);
		yf=plane[i].x*fsen(ag1)*fsen(ag2)+ plane[i].y*fcos(ag1)+plane[i].z*fsen(ag1)*fcos(ag2);

		x[i]=actualLevel->mouse->x+xf;y[i]=actualLevel->mouse->y+yf;
	}

	drawLine(x[0],y[0],x[1],y[1],COLOR_WHITE,&actualLevel->view); 
	drawLine(x[0],y[0],x[2],y[2],COLOR_WHITE,&actualLevel->view);
	drawLine(x[2],y[2],x[3],y[3],COLOR_WHITE,&actualLevel->view);
	drawLine(x[1],y[1],x[3],y[3],COLOR_WHITE,&actualLevel->view);


	/*for(i=0;i<4;i++)
	{

		xf=(plane[i].x-plane[i].z)*fcos(350);
		yf=plane[i].y+(plane[i].x+plane[i].z)*fsen(350);

		x[i]=actualLevel->mouse->x+xf;y[i]=actualLevel->mouse->y+yf;
	}

	drawLine(x[0],y[0],x[1],y[1],COLOR_RED,&actualLevel->view); 
	drawLine(x[0],y[0],x[2],y[2],COLOR_RED,&actualLevel->view);
	drawLine(x[2],y[2],x[3],y[3],COLOR_RED,&actualLevel->view);
	drawLine(x[1],y[1],x[3],y[3],COLOR_RED,&actualLevel->view); */

	//x=(x−z)cos(30) y=y+(x+z)sin(30)  
}

/*============================================================
 F: Camera position
 =============================================================*/

void calcViewCamera(gameLevel *gl,tmouse *mo)
{ int viewx=300,viewy=100,pview=8;

	if(mo->x<viewx) 
    {
		gl->xview-=pview;
		if(gl->xview<0) gl->xview=0;
		
	}
	
	if(mo->x>GetVideoWidth()-viewx)
	{
		gl->xview+=pview;
		if((gl->xview+GetVideoWidth())>=gl->background.resx) gl->xview=gl->background.resx-GetVideoWidth()-1;
	}

	if(mo->y<viewy) 
	{
		gl->yview-=pview;
		if(gl->yview<0) gl->yview=0;
	}
	
	if(mo->y>GetVideoHeight()-viewy)
	{
		gl->yview+=pview;
		if((gl->yview+GetVideoHeight())>=gl->background.resy) gl->yview=gl->background.resy-GetVideoHeight()-1;
	}
}

/*============================================================
 F: Stop game
 =============================================================*/

void stopGame(void)
{
	flgRun=0;
}

/*============================================================
 F: Close game
 =============================================================*/

void gameClose(void)
{
	fadeOutViewImage(actualLevel->xview,actualLevel->yview,&actualLevel->frame);
	resetLevel(actualLevel);

	freeImage(&fonts[0].bmp);

	freeAudio();
}


/*============================================================
 F: Get string time
 =============================================================*/

void getStringTime(long time,char *str)
{int sec,min;

	min=time/(60*60);
	sec=time/60-min*60;
	
	sprintf(str,"%d:%02d",min,sec);
}


/*============================================================
 F: Init Actors
 =============================================================*/

void initActors(gameLevel *gl)
{int i,j;
 pointRote *lp;

	if(!gl) return;

	for(i=0;i<gl->numActors;i++)
	{
		gl->listActors[i]=gl->listActorsPosition[i].ac;
		gl->listActors[i]->actualPoint=gl->listActorsPosition[i].p;
		lp=&listPoints[gl->listActors[i]->actualPoint];
		//gl->listActors[i]->xo=lp->x;
		//gl->listActors[i]->yo=lp->y;

		gl->listActors[i]->actualAction=0;
		gl->listActors[i]->state=0;
		gl->listActors[i]->sel=0;

		
		for(j=0;j<gl->listActors[i]->numActions;j++)
		{
			loadAnimate3D(gl->listActors[i]->actions[j]->ani);
		}
	}
}


/*============================================================
 F: Run Actors
 =============================================================*/

void runActors(gameLevel *gl,int draw)
{int i;
 Actor *ac;
 void (*control) (void *,void *);
 Animate3D *ani;

	if(!gl) return;

	for(i=0;i<gl->numActors;i++)
	{
		ac=gl->listActors[i];
		if(ac)
		{
			if(draw)
			{
				ani=ac->actions[ac->actualAction]->ani;
				printAnimate3D(ac->xo-ani->frame[ani->actualFrame]->resx/2,ac->yo-ani->frame[ani->actualFrame]->resy/2,ac->ago,ani,&gl->frame);
			}
			//printMaskAnimate3D(ac->xo,ac->yo,ac->ago,ani,&gl->frame,&gl->maskBackground,listPoints[ac->actualPoint].color);
			//printMaskAnimate3D(ac->xo-ani->frame[ani->actualFrame]->resx/2,ac->yo-ani->frame[ani->actualFrame]->resy/2,ac->ago,ani,&gl->frame,&gl->maskBackground,listPoints[ac->actualPoint].color);
			

			if(ac->control)
			{
				control=ac->control;

				(*control)((void *)gl,(void *)ac);	
			}
			//selActors(gl,ac);
		}
	}
}

/*============================================================
 F: Sel Actors
 =============================================================*/

void selActors(gameLevel *gl,Actor *ac)
{int x,y,x1,y1,x2,y2;
 Animate3D *ani;
 BMPimage *fr;

	if((!gl) || (!ac)) return;

	x=gl->mouse->x+gl->xview;
	y=gl->mouse->y+gl->yview;

	ani=ac->actions[ac->actualAction]->ani;
	fr=ani->frame[ani->actualFrame];

	x1=ac->xo-fr->resx/2;y1=ac->yo-fr->resy/2;
	x2=x1+fr->resx;y2=y1+fr->resy;

	if((x>x1) && (x<x2) && (y>y1) && (y<y2))
	{
		drawRetangle(x1,y1,x2,y2,COLOR_GREEN,&gl->frame); 
		ac->sel=1;
	}
	else
	{
		ac->sel=0;
	}

}


/*============================================================
 F: free Actors
 =============================================================*/

void freeActors(gameLevel *gl)
{int i,j;

	if(!gl) return;

	for(i=0;i<gl->numActors;i++)
	{
		for(j=0;j<gl->listActors[i]->numActions;j++)
		{
			freeAnimate3D(gl->listActors[i]->actions[j]->ani);
		}
	}
}

/*============================================================
 F: walk in line
 =============================================================*/

void walkInLine(Actor *ac) 
{int dx,dy,ag;
 float x,y;
 int i;

	dx=ac->xd-ac->xo;
	dy=ac->yd-ac->yo;

	ag=get_vetor_arctan_table(dx,-dy);
			 
	ac->ago=ag+ANGLE_22_5;
	if(ac->ago>=ANGLE_360) ac->ago=ac->ago-ANGLE_360;

	for(i=0;i<ac->vel;i++)
	{
		if((ac->xo==ac->xd) && (ac->yo==ac->yd)) return ;

		x=fcos(ag)*MOVERESOLUTION;
		y=fsen(ag)*MOVERESOLUTION;

		ac->xw+=x;
		ac->yw-=y;
		ac->xo=ac->xw;
		ac->yo=ac->yw;
	}		
}

/*============================================================
 F: Turn actor
 =============================================================*/

int turnActor(Actor *am) 
{int dag,dag1,dag2;

	if(am->ago!=am->agd) 
	{

		dag1=am->ago-am->agd;
		if(dag1<0) dag1=-dag1;
		dag2=ANGLE_360-dag1;
		if(dag2<0) dag2=ANGLE_360+dag2;

		if(dag1<=dag2)
		{
			dag=am->ago-am->agd;
			if(dag<0)
			{
				if(dag>-VELOCITTURN ) am->ago-=dag;
				else		 am->ago+=VELOCITTURN ;		
			}
			else
			{
				if(dag<VELOCITTURN )  am->ago-=dag;
				else		 am->ago-=VELOCITTURN ;
			} 
		}
		else
		{
			dag=am->ago-am->agd;
			if(dag<0)
			{
				dag=ANGLE_360+dag;
				if(dag<VELOCITTURN ) am->ago-=dag;
				else		 am->ago-=VELOCITTURN ;	
				if(am->ago<0) am->ago=ANGLE_360+am->ago;
			}
			else
			{
				dag=ANGLE_360-dag;
				if(dag<VELOCITTURN )  am->ago+=dag;
				else		 am->ago+=VELOCITTURN ;
				if(am->ago>=ANGLE_360) am->ago=am->ago-ANGLE_360;
			} 

		}
	}
	if(am->ago==am->agd) 
		return 1;
	else
		return 0;
}

/*============================================================
 F: Control Actor Main
 =============================================================*/

void controlActorMain(gameLevel *gl,Actor *ac)
{int xc,yc,xa,ya,p;
 Animate3D *ani;


	switch(ac->state)
	{
		case 0:
			ac->actualAction=STANDACTION;
			break;

		case 1:
			 xa=ac->xo;ya=ac->yo;

			 xc=listPoints[ac->ps].x;
			 yc=listPoints[ac->ps].y;
			 ac->xd=xc;
			 ac->yd=yc;

			 ac->agd=get_vetor_arctan_table((xc-xa),-(yc-ya))+ANGLE_22_5;
			 if(ac->agd>=ANGLE_360) ac->agd=ac->agd-ANGLE_360;
			 ac->xw=xa;
			 ac->yw=ya;
		//	 ac->actualAction=STANDACTION;
			 ac->state=2;

		case 2:
			if(turnActor(ac))
			{
				ac->ago=ac->agd;
				ac->state=3;
			}
			else break;
		case 3:
			 
			if((ac->xo==ac->xd) && (ac->yo==ac->yd)) 
			{
				ac->actualPoint=ac->ps;
				if(ac->pd==ac->ps)
				{
					ac->state=0;
				}
				else
				{
					if(strlen(ac->rote)>0)
					{
						ac->ps=getNextInt(&ac->rote);
						//ac->state=1;

						xc=listPoints[ac->ps].x;
						yc=listPoints[ac->ps].y;
						ac->xd=xc;
						ac->yd=yc;
						ac->xw=ac->xo;
						ac->yw=ac->yo;
						ac->vel=VELOCITWALK;
						ac->actualAction=WALKACTION;
						walkInLine(ac);
					}
					else
						ac->state=0;
				} 
			}
			else 
			{	ac->vel=VELOCITWALK;
				ac->actualAction=WALKACTION;
				walkInLine(ac);
			}

			break;
	}
		
}

/*============================================================
 F: Control Actor Object
 =============================================================*/

void controlActorObject(gameLevel *gl,Actor *ac)
{int xc,yc,xa,ya,ag;
 Animate3D *ani;

 Actor *am;


	selActors(gl,ac);

	if(ac->name)
	{
		ani=ac->actions[ac->actualAction]->ani;

		printImageText(ac->xo-ani->frame[ani->actualFrame]->resx/2,ac->yo-ani->frame[ani->actualFrame]->resy/2-16,ac->name,&fonts[0],COLOR_BLUE,&gl->frame);
	}

	switch(ac->state)
	{
		case 0:
			if((ac->sel) && (gl->mouse->status==2))
			{
				ac->actualAction=STANDACTION;
				ac->state=1;
				setSoundChannel("./res/beep.wav",9,0);
			}
			break;
		case 1:
			if(gl->mouse->status==0) 
				ac->state=2;
			break;
		case 2:
			
			 am=gl->actorMain;
			 if(am->state==0)
			 {
				am->rote=rotes[am->actualPoint][ac->actualPoint];
 				am->pd=ac->actualPoint;
				am->ps=getNextInt(&am->rote);
				if(strlen(am->rote)>0) 
					 am->ps=getNextInt(&am->rote);
				am->state=1;
			 }
			 else
			 {
				if(recalculateRote(gl,am,ac))
				{
					am->rote=rotes[am->ps][ac->actualPoint];
 					am->pd=ac->actualPoint;
					am->ps=getNextInt(&am->rote);
					if(strlen(am->rote)>0) 
					 am->ps=getNextInt(&am->rote);
					am->state=1;
				}
				else
				{
					am->rote=rotes[am->actualPoint][ac->actualPoint];
 					am->pd=ac->actualPoint;
					while(strlen(am->rote)>0)
					{
						if(am->ps==getNextInt(&am->rote)) break;
					}

				}
			 }
			 ac->state=0; 


			break;

	}
		
}

/*============================================================
 F: Recalcute rote
 =============================================================*/

int recalculateRote(gameLevel *gl,Actor *am,Actor *ac)
{char *newRote;
 int p;

	newRote=rotes[am->pd][ac->actualPoint];
	while(strlen(newRote)>0)
	{
		p=getNextInt(&newRote);
		if(am->actualPoint==p) return 1;
	}

	return 0;
}


/*============================================================
 F: Control Actor test
 =============================================================*/
/*
void controlActorTest(gameLevel *gl,Actor *ac)
{int xc,yc,xa,ya;
 Animate3D *ani;

	selActors(gl,ac);

	if(ac->name)
	{
		ani=ac->actions[ac->actualAction]->ani;

		printImageText(ac->xo-ani->frame[ani->actualFrame]->resx/2,ac->yo-ani->frame[ani->actualFrame]->resy/2-8,ac->name,&fonts[0],COLOR_RED,&gl->frame);
	}
	switch(ac->state)
	{
		case 0:
			if((ac->sel) && (gl->mouse->status==2))
			{
				ac->actualAction=STANDACTION;
				ac->state=1;
				setSoundChannel("./res/activated.wav",8,0);
			}
			break;
		case 1:
			if(gl->mouse->status==0) 
				ac->state=2;
			break;
		case 2:
			 xa=ac->xo;ya=ac->yo;
			 xc=gl->mouse->x+gl->xview;
			 yc=gl->mouse->y+gl->yview;
			 drawLine(xa,ya,xc,yc,COLOR_WHITE,&gl->frame); 
			 ac->ago=get_vetor_arctan_table((xc-xa),-(yc-ya))+ANGLE_22_5;
			 if(ac->ago>=ANGLE_360) ac->ago=ac->ago-ANGLE_360;
			 ac->xd=xc;
			 ac->yd=yc;
			 
			if(gl->mouse->status==2)
			{
				ac->xw=xa;
				ac->yw=ya;
				ac->vel=VELOCITWALK;
				ac->actualAction=WALKACTION;
				ac->state=3;
				setSoundChannel("./res/beep.wav",9,0);
			}  

			if(gl->mouse->status==1)
			{	
				ac->state=0;
			}
			break;
		case 3:
			walkInLine(ac); 
			//setSoundChannel("./res/footstep.wav",7,0);
			if((ac->xo==ac->xd) && (ac->yo==ac->yd)) 
			{
				ac->actualAction=STANDACTION;
				ac->state=0;
			}
			else 
			{
				if(gl->mouse->status==2)
				{
					xa=ac->xo;ya=ac->yo;
					xc=gl->mouse->x+gl->xview;
					yc=gl->mouse->y+gl->yview;
					drawLine(xa,ya,xc,yc,COLOR_WHITE,&gl->frame); 
					ac->ago=get_vetor_arctan_table((xc-xa),-(yc-ya))+ANGLE_22_5;
					if(ac->ago>=ANGLE_360) ac->ago=ac->ago-ANGLE_360;
					ac->xd=xc;
					ac->yd=yc;
				} 
			}
			break;
	}
		
} */


void controlActorTest(gameLevel *gl,Actor *ac)
{int xc,yc,xa,ya;
 Animate3D *ani;


	switch(ac->state)
	{
		case 0:
		
			 xa=ac->xo;ya=ac->yo;
			 xc=xa+(getCurrentTime())&0x3ff;
			 yc=ya+(getCurrentTime()/2)&0x1ff;
			 ac->ago=get_vetor_arctan_table((xc-xa),-(yc-ya))+ANGLE_22_5;
			 if(ac->ago>=ANGLE_360) ac->ago=ac->ago-ANGLE_360;
			 ac->xd=xc;
			 ac->yd=yc;
			 
			ac->xw=xa;
			ac->yw=ya;
			ac->vel=VELOCITWALK;
			ac->actualAction=WALKACTION;
			ac->state=1;
			break;
		case 1:
			if(turnActor(ac))
			{
				ac->vel=VELOCITWALK;
				ac->actualAction=WALKACTION;
				ac->state=2;
				//setSoundChannel("./res/activated.wav",8,0);

			}
			break;
		case 2:
			walkInLine(ac); 
			//setSoundChannel("./res/footstep.wav",7,0);
			if((ac->xo==ac->xd) && (ac->yo==ac->yd)) 
			{
				ac->actualAction=STANDACTION;
				ac->state=0;
			}
		
			break;
	}
		
}