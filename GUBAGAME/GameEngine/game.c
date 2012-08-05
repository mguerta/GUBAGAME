#include "so.h"
#include "game.h"
#include "actor.h"
#include "math.h"


/*============================================================
D: Data
=============================================================*/

int runGame(gameLevel *gl,int draw);
int loadGame(gameLevel *gl,int draw);

void scrollScene(gameLevel *gl,BMPimage *view);
//void renderScene(gameLevel *gl,BMPimage *view);
void renderAllViews(gameLevel *gl,BMPimage *view);
void renderFireButton(gameLevel *gl,BMPimage *view,viewGame *vg);
void renderScene(gameLevel *gl,BMPimage *view,viewGame *vg);
void renderMainTopSceneView(gameLevel *gl,BMPimage *view,viewGame *vg);
void renderTopSceneView(gameLevel *gl,BMPimage *view,viewGame *vg);
void renderSceneView3D(gameLevel *gl,BMPimage *view,viewGame *vg);
void projectScene(gameLevel *gl,BMPimage *view);

void loadingGame(gameLevel *gl);


void projectQUAD(int xt,int yt,int zt,int lenx, int leny, gameLevel *gl,BMPimage *view,pixel COLOR);

void projection(void);
void unProjection(gameLevel *gl,int x,int y,float *x1,float *z1);


fontText fonts={"./res/fontgbrbold17.tga",29,32,4};


Animate3D aniAzulStand={"./res/idle",TRANSPARENT_COLOR,30,ANIPINGPONG,CACHE,25};
Animate3D aniAzulWalk={"./res/jog",TRANSPARENT_COLOR,15,ANINORMAL,CACHE,1};

Animate3D aniBolaStand={"./res/bola",TRANSPARENT_COLOR,10,ANIPINGPONG,CACHE,1,BMPOPTIMIZE};
Animate3D aniRobotAStand={"./res/robotA",TRANSPARENT_COLOR,40,ANIPINGPONG,CACHE,1};
Animate3D aniRobotBStand={"./res/robotB",TRANSPARENT_COLOR,40,ANIPINGPONG,CACHE,1};
Animate3D aniRobotCStand={"./res/robotC",TRANSPARENT_COLOR,40,ANIPINGPONG,CACHE,1};
Animate3D aniRobotDStand={"./res/robotD",TRANSPARENT_COLOR,40,ANIPINGPONG,CACHE,1};

Animate3D aniNinjaStand={"./res/idle",TRANSPARENT_COLOR,10,ANIPINGPONG,CACHE,1};
Animate3D aniNinjaWalk={"./res/jog",TRANSPARENT_COLOR,30,ANINORMAL,CACHE,2};
Animate3D aniNinjaPunch={"./res/idle",TRANSPARENT_COLOR,30,ANINORMAL,CACHE,5};
//Animate3D aniNinjaPunch={"./res/punch",TRANSPARENT_COLOR,30,ANINORMAL,CACHE,5};
Animate3D aniBalaStand={"./res/bala",TRANSPARENT_COLOR,10,ANINORMAL,CACHE,1,BMPOPTIMIZE};

Action ninjaStand={&aniNinjaStand};
Action ninjaWalk={&aniNinjaWalk};
Action ninjaPunch={&aniNinjaPunch};

Action azulStand={&aniAzulStand};
Action azulWalk={&aniAzulWalk};

Action bolaStand={&aniBolaStand};
Action bolaWalk={&aniBolaStand};

Action balaStand={&aniBalaStand};

Action robotAStand={&aniRobotAStand};
Action robotAWalk={&aniRobotAStand};
Action robotBStand={&aniRobotBStand};
Action robotBWalk={&aniRobotBStand};
Action robotCStand={&aniRobotCStand};
Action robotCWalk={&aniRobotCStand};
Action robotDStand={&aniRobotDStand};
Action robotDWalk={&aniRobotDStand};

//posicao 0,0,0,  centro do mundo.
Actor ninja0={ACTORMAIN,0,0,0,ANGLE_0,"NINJA0",CAMERAON,TAMQUAD/2,TAMQUAD/2,TAMQUAD/2,(void (*) (void *,void *))&controlActorMain,3,&ninjaStand,&ninjaWalk,&ninjaPunch};
Actor bola1={ACTORBAD,150,360,0,ANGLE_90,"BOLA1",CAMERAOFF,TAMQUAD/2,TAMQUAD/2,TAMQUAD/2,(void (*) (void *,void *))&controlActorTest,2,&bolaStand,&bolaWalk};
Actor bala={ACTORFIRE,10000,100000,0,ANGLE_90,"Bala",CAMERAOFF,TAMQUAD/8,TAMQUAD/8,TAMQUAD/8,(void (*) (void *,void *))&controlActorBala,1,&balaStand};

Actor robotA={ACTORBAD,0,360,0,ANGLE_90,"BOLA1",CAMERAOFF,TAMQUAD/2,TAMQUAD/2,TAMQUAD/2,(void (*) (void *,void *))&controlActorTest,2,&robotAStand,&robotAWalk};
Actor robotB={ACTORBAD,15,360,0,ANGLE_180,"BOLA1",CAMERAOFF,TAMQUAD/2,TAMQUAD/2,TAMQUAD/2,(void (*) (void *,void *))&controlActorTest,2,&robotBStand,&robotBWalk};
Actor robotC={ACTORBAD,150,36,0,ANGLE_0,"BOLA1",CAMERAOFF,TAMQUAD/2,TAMQUAD/2,TAMQUAD/2,(void (*) (void *,void *))&controlActorTest,2,&robotCStand,&robotCWalk};
Actor robotD={ACTORBAD,50,60,0,ANGLE_45,"BOLA1",CAMERAOFF,TAMQUAD/2,TAMQUAD/2,TAMQUAD/2,(void (*) (void *,void *))&controlActorTest,2,&robotDStand,&robotDWalk};

Animate2D aniTest={"./res/tiledTeste",TRANSPARENT_COLOR,150,3,CACHE,BMPOPTIMIZE};

tiled t0={TILEDNORMAL,"./res/tiled0.tga",0,0};
tiled t1={TILEDNORMAL,"./res/tiled1.tga",10,0};
//tiled t1={TILEDANI,0,10,&aniTest};
tiled t2={TILEDNORMAL,"./res/tiledn.tga",0,0};


unsigned short map[1000*1000]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
scene scene1=
{
	55,55, //15,15, //25,25,  // precisa ser numero impar 
	160,80,
	ITAMQUAD,ITAMQUAD,
	(unsigned short *)&map,
	&t0,&t1,&t0,
	&t2,&t0,&t0,
	&t0,&t0,&t0,&t0,
	&t0,&t0,&t0,
	&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0,&t0
};

viewGame view3D=
{
	0,0,
	720,480,
	8,8,
	SOLID_COLOR,
	&renderSceneView3D,
    VIEWTOUCHENABLE
};

viewGame view2D=
{
	510,260,
	200,200,
	25,25,
	TRANS_COLOR,//SOLID_COLOR,
	&renderMainTopSceneView,
    VIEWTOUCHDISABLE
};
viewGame view2D2=
{
	20,400,
	60,60,
	0,0,
	TRANS_COLOR,
	&renderFireButton,
    VIEWTOUCHDISABLE
};

listSound listMusic=
{2,
 "./res/rio_de_hola.wav","./res/activated.wav"
};

gameLevel levelTest=
{&scene1,
&fonts,
"./res/fundo.tga",&listMusic,
0,0,MASKCOLOR,"./res/score.tga",
40,40,MASKCOLOR,"./res/lives.tga",
30,440,COLOR_WHITE,
2,&view3D,&view2D,0,0,0,

&ninja0,6,&ninja0,&bala,&robotA,&robotB,&robotC,&robotD};

gameLevel *actualLevel;

int flgRun;

int runStateGame;

 int ag1=AG1/*393 /*450 */,ag2=AG2; /*452 /*450*/
 int numTiled=0,numTotalTiled=0;
 //int xcel,ycel;

 int CENTRALXVIEW=0;
 int CENTRALYVIEW=0;

 int ylinha,xlinha;

/*============================================================
 F: Entry Point 
 =============================================================*/
int Application(char *cmdLine)
{
	gameMain();	
	return 0;
}


/*============================================================
 F: End Point 
 =============================================================*/

int stopApplication(void)
{

	return 0;
}

/*============================================================
 F: Game Initialization
 =============================================================*/

int gameInit(void)
{
   
    SetSyncTimer(60);   //SET 60 FPS 
    
	initProjOrtho3D();
    
	resetClipArea();
	
	actualLevel=setLevel(&levelTest);
    

	return 0;

}

/*============================================================
 F: Set level
 =============================================================*/

gameLevel * setLevel(gameLevel *gl)
{
    
    gl->xview=0;
    gl->yview=0;
    gl->view.buf=GetVideo();
    gl->view.resx=GetVideoWidth();
    gl->view.resy=GetVideoHeight();
    gl->idxLoadActor=0;
    gl->stateGame=0;
    cloneImage(&gl->view,&gl->frame);
    
    runStateGame=0;
	flgRun=1;
    

	return gl;
}

/*============================================================
 F: reset level
 =============================================================*/

void resetLevel(gameLevel *gl)
{
	if(gl)
	{	
		freeImage(&gl->background);
		freeImage(&gl->frame);
		gl->xview=0;
		gl->yview=0;

		freeActors(gl);
	}
}



/*============================================================
 F: Game loop Aberto 
 =============================================================*/

int Vfps=0,Vfpsg=0;
int gameMainOpen(void)
{

    unsigned int joy;
    tmouse *mo;

    int sound;
    int draw;
    
      

        actualLevel->view.buf=GetVideo();
        
        joy=GetJoystick();
        mo=getMouseInf();
        
        actualLevel->mouse=mo;
        actualLevel->key=joy;
        
   
        draw=getDrawFrame();
    
        switch (runStateGame) 
        {
            case 0:
                if(loadGame(actualLevel,draw)) 
                    runStateGame=1;
            break;
            case 1:
                runGame(actualLevel,draw);
            break;        
            default:
            break;
        }
        
    
        wipeView(&actualLevel->transiction);
              
        if(getRenderAudio())
            sound=SRenderAudio();
        
        if(draw) FlipVideo();
        WaitSyncTimer();  
        
        
        getFPS(&Vfpsg,&Vfps);
		
      
        return draw;
             
   
}


/*============================================================
 F: Screen entry game
 =============================================================*/

int loadGame(gameLevel *gl,int draw)
{int ret=0;
  
    int i;
    
    scene *sc;
    
        switch (gl->stateGame) 
        {
        
                
            case 0:
            
                loadTGA(gl->fileBackground,&gl->background); 
                   
                gl->stateGame++;
                setWipe(&gl->transiction,FADEOUT);
                
                break;
                
          
            case 1: loadingGame(gl); 
                    if(gl->transiction.type==IDLE)
                    {
                        if(initActorsOpen(gl))
                            gl->stateGame++;
                    }
                break;
                
            case 2:
                loadingGame(gl);
            	for(i=0;i<gl->listSounds->size;i++)
                {
                    SsetMusic(gl->listSounds->nameSounds[i],0);
                }
                
                SstartIDMusic(0,REPEATMUSIC);
                
                sc=gl->sceneLevel;
                
                
                for(i=0;i<sc->sizex*sc->sizey;i++)   // cria cenario randomicamente
                    *(sc->map+i)=myrand() & 31;
                
                
                gl->stateGame++;
                setWipe(&gl->transiction,FADEIN);
  
                break;
                
                
            case 3:
                if(gl->transiction.type==IDLE)
                {
              
                    setWipe(&gl->transiction,FADEOUT);
                    gl->stateGame=0;
                    ret=1;
                }
                break;
            default:
            break;
        }
   
       
    
    return ret;
    
}
/*============================================================
 F: loading status bar game
 =============================================================*/

void loadingGame(gameLevel *gl)
{ char text[100],time[100];
    
    copyImageView(0,0,&gl->background,&gl->view);


    getStringTime(getCurrentTime(),time);
    sprintf(text,"Carregando o jogo = %s ",time);
    printImageText(10,10,text,gl->fonts,COLOR_WHITE,&gl->view);
}

/*============================================================
 F: run game core
 =============================================================*/

int runGame(gameLevel *gl,int draw)
{ 
    char text[100],time[100];
    
    runActors(gl,0);
    
    posViewCameraByActor (gl,gl->actorMain); //TEM QUE ESTAR AQUI.
    
    if(draw)
    {
        
        //putViewImage(0,0,&actualLevel->background);
        cleanImageFast(&gl->view,0x4a);
        //   projectScene(actualLevel,&actualLevel->view);	
        
        renderAllViews(gl,&gl->view);
        
        getStringTime(getCurrentTime(),time);
        sprintf(text,"tempo = %s ",time);
        printImageText(10,10,text,gl->fonts,COLOR_WHITE,&gl->view);
        
        sprintf(text,"FPS %d/%d - xyz= %d,%d,%d - xyv= %d,%d ",Vfpsg,Vfps,gl->actorMain->xo,gl->actorMain->yo,gl->actorMain->zo,gl->actorMain->xp,gl->actorMain->yp);
        printImageText(10,40,text,gl->fonts,COLOR_WHITE,&gl->view);
        
        sprintf(text,"Actors = %d - mem = %d joy = %d,%d",gl->numActorsLive,getSizeMalloc(),view2D.mouse.x,view2D.mouse.y);      
        printImageText(10,70,text,gl->fonts,COLOR_WHITE,&gl->view);	
        
        
    }
  
    return 0;

}
/*============================================================
 F: Game loop - PRIMEIRO PROTOTIPO DE JOGO PERSPECTIVA ISOMETRICA
 =============================================================*/

void gameMain(void)
{
	gameInit();
	do
	{
		gameMainOpen();

	}while(flgRun);

	gameClose();
}

/*============================================================
 F: render all views
 =============================================================*/

void renderAllViews(gameLevel *gl,BMPimage *view)
{void (*viewControl) (void *,void *,void *);
 int i;
 boxArea v2d;
 viewGame *vg;
 int tamViewX;
 int tamViewY;
 int xof;
 int yof;

	for(i=0;i<actualLevel->numViewGame;i++)
	{

		vg=gl->viewList[i];
		vg->flgTouch=0;
		xof=vg->x;
		yof=vg->y;

	    tamViewX=vg->tamx;
		tamViewY=vg->tamy;
		v2d.x1=0;v2d.y1=0;v2d.x2=tamViewX-1;v2d.y2=tamViewY-1;

        if(vg->flgView)
        {
            if((gl->mouse->x>(v2d.x1+xof)) && (gl->mouse->x<(v2d.x2+xof)) && (gl->mouse->y>(v2d.y1+yof)) && (gl->mouse->y<(v2d.y2+yof)))
            {
                if(gl->mouse->status & MOUSE_DOWN)
                {
                    vg->flgTouch=1;
                }
            }
        }
		viewControl=gl->viewList[i]->control;
		(*viewControl)((void *)gl,(void *)view,vg);	
	}
}

/*============================================================
 F: render fire button
 =============================================================*/

void renderFireButton(gameLevel *gl,BMPimage *view,viewGame *vg)
{int x1,y1,x2,y2;
pixel color=COLOR_YELOW;

	x1=vg->x;
	y1=vg->y;
	x2=x1+vg->tamx-1;
	y2=y1+vg->tamy-1;

	gl->buttonFire=0;
	if(vg->flgTouch)
	{
		color=COLOR_RED;
		gl->buttonFire=1;

		gl->mouseView.status=0;
	}
	drawBOX (x1,y1,x2,y2,color,vg->flagAlpha,view); 

}

/*============================================================
 F: render main view Main
 =============================================================*/

void renderMainTopSceneView(gameLevel *gl,BMPimage *view,viewGame *vg)
{
	vg->cam.ag=gl->cam.ag;
	vg->cam.x=gl->cam.x;
	vg->cam.y=gl->cam.y;
	vg->cam.z=gl->cam.z;

	renderTopSceneView(gl,view,vg);
}

/*============================================================
 F: render Top scene view
 =============================================================*/

void renderTopSceneView(gameLevel *gl,BMPimage *view,viewGame *vg)
{ scene *sc;
  float xfc,yfc;
  boxArea v2d;
  int color;


  int numCelViewX=vg->celx;  // maximo igual = sc->sizex
  int numCelViewY=vg->cely;  // maximo igual = sc->sizey

  int tamViewX;
  int tamViewY;

  int xof=vg->x;
  int yof=vg->y;
  int xc,yc;
  int dx,dy;
  int x,y;
  int x1,y1,x2,y2;
  int xcel,ycel,xcel2,ycel2;

  float rx,ry,rsx,rsy;

  unsigned short *bp;

   Actor *ac;

   int xmax,ymax;

  	
//tamanho da janela em pixels
    tamViewX=vg->tamx;
    tamViewY=vg->tamy;
	v2d.x1=0;v2d.y1=0;v2d.x2=tamViewX-1;v2d.y2=tamViewY-1;

	//Define uma clip area do tamanho da area da janela posicionada na tela
	setClipArea(v2d.x1+xof,v2d.y1+yof,v2d.x2+xof+1,v2d.y2+yof+1,view);

	//Calcula o fator de escala de acordo com as dimensoes da janela 
	sc = gl->sceneLevel;

	rx=(float)(sc->realSizeTiledx/KDD_FIX);
	rx*=numCelViewX;
	rx=rx/tamViewX;

	ry=(float)(sc->realSizeTiledy/KDD_FIX);
	ry*=numCelViewY;
	ry=ry/tamViewY;


	rsy=(float)(sc->realSizeTiledy/KDD_FIX);
	rsy/=ry;	
	rsx=(float)(sc->realSizeTiledx/KDD_FIX);
	rsx/=rx;

	//calcula o valor das dimensoes maxima do cenario
 	xc=(sc->sizex-numCelViewX)*sc->realSizeTiledx;
	yc=(sc->sizey-numCelViewY)*sc->realSizeTiledy;
	xmax=xc>>(PDD_FIX+1);
	ymax=yc>>(PDD_FIX+1);

	//calcula posição do mouse no view
	if(vg->flgTouch)
	{

		xc=gl->mouse->x;
		yc=gl->mouse->y;

		xfc=(float)(xc-xof-tamViewX/2);
		yfc=(float)(yc-yof-tamViewY/2);
		xfc*=rx;
		yfc*=ry;
		xc=(int)(xfc+vg->xTopView);
		yc=(int)(yfc+vg->yTopView);
	
		vg->mouse.status=gl->mouse->status;
		vg->mouse.x=xc;
		vg->mouse.y=yc;

		gl->mouseView.status=vg->mouse.status;
		gl->mouseView.x=vg->mouse.x;
		gl->mouseView.y=vg->mouse.y;
		gl->mouseView.z=0;

	}


	//calcula foco da camera
    if(vg->flgTopView==0)
    {
		vg->flgTopView=1;
		vg->xTopView=vg->cam.x;
		vg->yTopView=vg->cam.y;
    }

	xfc=(float)(vg->cam.x-vg->xTopView);
	yfc=(float)(vg->cam.y-vg->yTopView);
	xfc/=rx;
	yfc/=ry;

	xc=(int)(xfc);
	yc=(int)(yfc);


	//reposiciona scroll da tela em funcao da area da camera 1/3 da tela centralizada
	if((xc>(tamViewX/3)) )
	{
		if(vg->xTopView<=xmax)
		{
			xfc=(float)(xc-(tamViewX/3));
			xfc*=rx;
			xc=(int)xfc;
			vg->xTopView+=xc;
		}
	}

	if( (xc<-(tamViewX/3)) )
	{
		if(vg->xTopView>=-xmax)
		{
			xfc=(float)(xc+(tamViewX/3));
			xfc*=rx;
			xc=(int)xfc;
			vg->xTopView+=xc;
		}
	}


	if((yc>(tamViewY/3)) )
	{
		if(vg->yTopView<=ymax)
		{
			yfc=(float)(yc-(tamViewY/3));
			yfc*=ry;
			yc=(int)yfc;
			vg->yTopView+=yc;
		}
	}

	if( (yc<-(tamViewY/3)) )
	{
		if(vg->yTopView>=-ymax)
		{
			yfc=(float)(yc+(tamViewY/3));
			yfc*=ry;
			yc=(int)yfc;
			vg->yTopView+=yc;
		}
	}


	//Desenha box com as dimensoes do cenario todo
	xc=(0-sc->sizex)*sc->realSizeTiledx;
	yc=(0-sc->sizey)*sc->realSizeTiledy;
	xc=xc>>(PDD_FIX+1);
	yc=yc>>(PDD_FIX+1);

	xfc=(float)(xc-vg->xTopView);
	yfc=(float)(yc-vg->yTopView);
	xfc/=rx;
	yfc/=ry;

	xc=(int)(xof+xfc+tamViewX/2);
	yc=(int)(yof+yfc+tamViewY/2);

	dx=(int)(rsx*sc->sizex);
	dy=(int)(rsy*sc->sizey);

	x1=xc;y1=yc;x2=xc+dx;y2=yc+dy;
//	drawRetangle (x1,y1,x2,y2,COLOR_BLUE,view); 

	drawBOX (x1,y1,x2,y2,PIXEL(0x7f,0x7f,0x7f),vg->flagAlpha,view); 

 // desenha os box com tamanho maior que zero
	xcel=((((vg->xTopView)<<(PDD_FIX))/sc->realSizeTiledx))+sc->sizex/2;
	ycel=((((vg->yTopView)<<(PDD_FIX))/sc->realSizeTiledy))+sc->sizey/2;

	xcel=xcel-numCelViewX/2-1;
	ycel=ycel-numCelViewY/2-1;

	if(xcel<0) xcel=0;
	if(ycel<0) ycel=0;
	
	xcel2=xcel+numCelViewX+3;
	ycel2=ycel+numCelViewY+3;

	if(xcel2>sc->sizex) xcel2=sc->sizex;
	if(ycel2>sc->sizey) ycel2=sc->sizey;

	for(y=ycel;y<ycel2;y++)
	{
		bp=sc->map+y*sc->sizex+xcel;
		for(x=xcel;x<xcel2;x++)
		{
			if(sc->listTiled[*bp]->sizez>0) 
			{

			xc=(x-sc->sizex/2)*sc->realSizeTiledx;
			yc=(y-sc->sizey/2)*sc->realSizeTiledy;
			xc=xc>>(PDD_FIX);
			yc=yc>>(PDD_FIX);

			xfc=(float)(xc-vg->xTopView);
			yfc=(float)(yc-vg->yTopView);
			xfc/=rx;
			yfc/=ry;

			xc=(int)(xof+xfc+tamViewX/2);
			yc=(int)(yof+yfc+tamViewY/2);

			dx=(int)((rsx/2.0)+0.5);//dx++;
			dy=(int)((rsy/2.0)+0.5);//dy++;

			x1=xc-dx;y1=yc-dy;x2=xc+dx;y2=yc+dy;

			if(!((x1>v2d.x2+xof) | (x2<v2d.x1+xof) | (y1>v2d.y2+yof) | (y2<v2d.y1+yof)))
			{
				//drawRetangle (x1,y1,x2,y2,COLOR_BLUE,view); 
				drawBOX (x1,y1,x2,y2,COLOR_BLACK,vg->flagAlpha,view);
			}
			}
			bp++;

		}
	}
 //desenha os atores
	x=gl->firstActor;
	while(x!=-1)
	{
		ac=gl->listActors[x];
		xfc=(float)(ac->xo-vg->xTopView);
		yfc=(float)(ac->yo-vg->yTopView);
		xfc/=rx;
		yfc/=ry;

		xc=(int)(xof+xfc+tamViewX/2);
		yc=(int)(yof+yfc+tamViewY/2);

		xfc=(float)ac->sizex;  
		xfc/=rx;
		yfc=(float)ac->sizey;
		yfc/=ry;

		dx=(int)xfc;
		dy=(int)yfc;
	
		x1=xc-dx;y1=yc-dy;x2=xc+dx;y2=yc+dy;

		if(!((x1>v2d.x2+xof) | (x2<v2d.x1+xof) | (y1>v2d.y2+yof) | (y2<v2d.y1+yof)))
		{

			color=COLOR_YELOW;
			if(ac==gl->actorMain) color=COLOR_GREEN;

			//drawRetangle (x1,y1,x2,y2,color,view); 
			drawBOX (x1,y1,x2,y2,color,vg->flagAlpha,view); 

			dx=(int)(fcos(gl->listActors[x]->ago)*dx*2);
			dy=(int)(fsen(gl->listActors[x]->ago)*dy*2);

			drawLine(xc,yc,xc+dx,yc-dy,COLOR_BLUE,&actualLevel->view); 
		}

		x=ac->nextActor;  
	}

	//desenha as bordas da janela 
	drawRetangle (v2d.x1+xof,v2d.y1+yof,v2d.x2+xof,v2d.y2+yof,COLOR_WHITE,view); 

	//reset clip area 
	resetClipArea();


}

/*============================================================
 F: render main view 3D Main
 =============================================================*/

void renderSceneView3D(gameLevel *gl,BMPimage *view,viewGame *vg)
{    float xf=0,zf=0,yf=0;

   int xv=0,yv=0;


	vg->cam.ag=gl->cam.ag;
	vg->cam.x=gl->cam.x;
	vg->cam.y=gl->cam.y;
	vg->cam.z=gl->cam.z;

	xv=gl->mouse->x;
	yv=gl->mouse->y;

	if(gl->mouse->status & MOUSE_DOWN)
	{
		unProjection(gl,xv,yv,&xf,&yf);

		xv=(int)xf;
		yv=(int)yf;

		gl->mouseView.x=xv;
		gl->mouseView.y=yv;
		gl->mouseView.z=0;

	}
	gl->mouseView.status=gl->mouse->status;

//	renderScene(gl,view);
	renderScene(gl,view,vg);
}

/*============================================================
 F: render scene	
 =============================================================*/



void renderScene(gameLevel *gl,BMPimage *view,viewGame *vg)
{ int x,y,z,xc,yc;
  unsigned short *bp,*bpp,i;
  BMPimage *td;
  int xf,yf;
  int mx,my,mtx,mty;
  int sizetx,sizety;

  int xcel,ycel,zcel;

  int xv,xvv,yv;
  scene *sc;

  int CELVIEWX,CELVIEWY;



  	    int xl,yl,xvl,yvl;
	    unsigned short *bpl;

//  float xff,yff;

  
		CELVIEWX = vg->celx;
		CELVIEWY = vg->cely;

		sc = gl->sceneLevel;

		sizetx=sc->sizeTiledx/2;
		sizety=sc->sizeTiledy/2;
//===================================================

		xcel=((((gl->cam.x)<<(PDD_FIX))/sc->realSizeTiledx))+sc->sizex/2;
		ycel=((((gl->cam.y)<<(PDD_FIX))/sc->realSizeTiledy))+sc->sizey/2;

		zcel=projZOrtho3D(gl->cam.z)/sc->sizeTiledy;

		xc=xcel-CELVIEWX+zcel;
		yc=ycel-CELVIEWY+zcel;

		if(xc<0) xc=0;
		if(yc<0) yc=0;

		mtx=sc->sizex-xc;
		if(mtx>CELVIEWX*2) mtx=CELVIEWX*2;
		mty=sc->sizey-yc;
		if(mty>CELVIEWY*2) mty=CELVIEWY*2;

		bpp=sc->map+yc*sc->sizex+xc;

		xv=xc;
		yv=yc;

		x=(int)((xc*sc->realSizeTiledx-sc->realSizeTiledx/2-(sc->sizex/2)*sc->realSizeTiledx))>>PDD_FIX;
		z=(int)((yc*sc->realSizeTiledx+sc->realSizeTiledx/2-(sc->sizey/2)*sc->realSizeTiledx))>>PDD_FIX;

		projOrtho3D(x,z,0,&xf,&yf);

		calcPosView(gl,xf,yf,&x,&y);

//======================================================== DESENHA O CHAO PRIMEIRO
	    xl=x;
		yl=y;
		xvl=xv;
		yvl=yv;
	    bpl=bpp;
		
		numTiled=0;numTotalTiled=0;
		xlinha=0;

		for(my=0;my<mty;my++)
		{
			bp=bpp;	

			xc=x;yc=y;
			xvv=xv;
			for(mx=0;mx<mtx;mx++)
			{	
				i=*bp++;

				if(sc->listTiled[i]->type==TILEDNORMAL)
				{
					td=&sc->listTiled[i]->bmp;
					if(td->buf==0)
					{
						loadTGA(sc->listTiled[i]->nameFile,td);
						getOptImage(td,view,TRANSPARENT_COLOR);
					}
				}
				else
				{	

					if(sc->listTiled[i]->ani->lastCurrentTime==0)
						loadAnimate2D(sc->listTiled[i]->ani,view);
					td=&sc->listTiled[i]->ani->frame[sc->listTiled[i]->ani->actualFrame];
				}	

				numTotalTiled++;
	
				if((xc>=gl->view.resx) || (yc>gl->view.resy+4*td->resyOrg)) 
					break;
				if((xc>=-td->resxOrg) && (xc<gl->view.resx+td->resxOrg) && (yc>=0) && (yc<=gl->view.resy+td->resyOrg+td->offsety) )
				{
				
					if(sc->listTiled[i]->sizez<=0)
					{
						
						if(sc->listTiled[i]->type==TILEDNORMAL)
						{
							if(td->optImage)
							{
								putOptImage(xc,yc-td->resyOrg,td,view,TRANSPARENT_COLOR);

							}
							else
							{
								putImage(xc,yc-td->resyOrg,td,view,TRANSPARENT_COLOR);

							}
						}
						else
						{
						//printAnimate3D(xc,yc,0,sc->listTiled[i]->ani,view,1);
							printAnimate2D(xc,yc-td->resyOrg,sc->listTiled[i]->ani,view);

						}

					}
					numTiled++;
				}

				xc+=sizetx;
				yc+=sizety;
				xvv++;

			}

			x-=sizetx;
			y+=sizety;
			yv++;
		
			bpp+=sc->sizex;
		}

//======================================================== DESENHA O RESTO AGORA
		x=xl;
		y=yl;
		xv=xvl;
		yv=yvl;
	    bpp=bpl;

		xlinha=0;
	
		for(my=0;my<mty;my++)
		{
			bp=bpp;	

			xc=x;yc=y;
			xvv=xv;
			for(mx=0;mx<mtx;mx++)
			{	
				i=*bp++;

				if(sc->listTiled[i]->type==TILEDNORMAL)
				{
					td=&sc->listTiled[i]->bmp;
			
				}
				else
				{	

					td=&sc->listTiled[i]->ani->frame[sc->listTiled[i]->ani->actualFrame];
				}	

				numTotalTiled++;

	
				if((xc>=gl->view.resx) || (yc>gl->view.resy+4*td->resyOrg)) 
					break;
				if((xc>=-td->resxOrg) && (xc<gl->view.resx+td->resxOrg) && (yc>=0) && (yc<=gl->view.resy+td->resyOrg+100) )
				{

					drawActorByCel(gl,xvv,yv);

				
					if(sc->listTiled[i]->sizez>0)
					{

						if(sc->listTiled[i]->type==TILEDNORMAL)
						{
							if(td->optImage)
							{
								putOptImage(xc,yc-td->resyOrg,td,view,TRANSPARENT_COLOR);

							}						
							else
							{
								putImage(xc,yc-td->resyOrg,td,view,TRANSPARENT_COLOR);

							}
						}
						else	
						{
							//printAnimate3D(xc,yc,0,sc->listTiled[i]->ani,view,1);
							printAnimate2D(xc,yc-td->resyOrg,sc->listTiled[i]->ani,view);

						}
					}
				
					numTiled++;
				}

				xc+=sizetx;
				yc+=sizety;
				xvv++;

			}

			x-=sizetx;
			y+=sizety;
			yv++;
		
			bpp+=sc->sizex;
		}

//	drawActors(actualLevel);

}


/*============================================================
 F: calc pos view
 =============================================================*/

void calcPosView(gameLevel *gl,int x,int y,int *xp, int *yp)
{
		*xp=x-gl->xview+gl->view.resx/2+CENTRALXVIEW;
		*yp=y-gl->yview+gl->view.resy/2+gl->sceneLevel->sizeTiledy/2+CENTRALYVIEW;
}


/*============================================================
 F: project scene	
 =============================================================*/

void projectScene(gameLevel *gl,BMPimage *view)
{scene *sc;
 
	sc = gl->sceneLevel;

	projectQUAD((int)(gl->actorMain->xcel*FTAMQUAD-FTAMQUAD/2-(gl->sceneLevel->sizex/2)*FTAMQUAD),(int)(gl->actorMain->ycel*FTAMQUAD+FTAMQUAD/2-(gl->sceneLevel->sizey/2)*FTAMQUAD),0,TAMQUAD,TAMQUAD,gl,view,COLOR_WHITE);
	projectQUAD((gl->actorMain->xo-TAMQUAD/2),(gl->actorMain->yo+TAMQUAD/2),gl->actorMain->zo,TAMQUAD,TAMQUAD,gl,&gl->view,COLOR_GREEN);
	projectQUAD((gl->actorMain->xo-TAMQUAD/2),(gl->actorMain->yo+TAMQUAD/2),0,TAMQUAD,TAMQUAD,gl,&gl->view,COLOR_GREEN);

} 


/*============================================================
 F: project actor	
 =============================================================*/

void projectActor(gameLevel *gl,Actor *ac,pixel color)
{
	projectQUAD((ac->xo-ac->sizex),(ac->yo+ac->sizey),0,ac->sizex*2,ac->sizey*2,gl,&gl->view,color);
}


/*============================================================
 F: Projection
 =============================================================*/

void projectQUAD(int xt,int yt,int zt,int lenx, int leny, gameLevel *gl,BMPimage *view,pixel COLOR)
{coord3D plane[4]=
{0,0,0,
lenx,0,0,
0,0,-leny,
lenx,0,-leny};

 int x[4];
 int y[4];
 float xf,yf;
 int i;

 	for(i=0;i<4;i++)
	{
		plane[i].x+=xt;
		plane[i].z+=yt;
		plane[i].y+=zt;
	}

	for(i=0;i<4;i++)
	{
		xf=plane[i].x*fcos(ag2)-plane[i].z*fsen(ag2);
		yf=plane[i].x*fsen(ag1)*fsen(ag2)+ plane[i].y*fcos(ag1)+plane[i].z*fsen(ag1)*fcos(ag2);
 
		x[i]=(int)(xf-gl->xview+gl->view.resx/2+CENTRALXVIEW);
		y[i]=(int)(yf-gl->yview+gl->view.resy/2+CENTRALYVIEW);
	}

	drawLine(x[0],y[0],x[1],y[1],COLOR_WHITE,view); 
	drawLine(x[0],y[0],x[2],y[2],COLOR,view);
	drawLine(x[2],y[2],x[3],y[3],COLOR,view);
	drawLine(x[1],y[1],x[3],y[3],COLOR_RED,view);

}


/*============================================================
 F: unProjection
 =============================================================*/

void unProjection(gameLevel *gl,int x,int y,float *x1,float *z1)
{float xu,yu;
 float xp,yp;
 float K1,K2,K3,K4,A,B;

    xp=(float)(x+gl->xview-gl->view.resx/2-CENTRALXVIEW);
	yp=(float)(y+gl->yview-gl->view.resy/2-CENTRALYVIEW);

	K1=fcos(ag2);
	K2=fsen(ag2);
	K3=fsen(ag1)*fsen(ag2);
	K4=fsen(ag1)*fcos(ag2);

	A=K3/K1;
	B=((K2*K3)/K1)+K4;

	yu=(yp-xp*A)/B;
	xu=(xp+yu*K2)/K1;


	*x1=xu;
	*z1=yu;
}

/*============================================================
 F: Projection
 =============================================================*/

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

		x[i]=(int)(actualLevel->mouse->x+xf);y[i]=(int)(actualLevel->mouse->y+yf);
	}

	drawLine(x[0],y[0],x[1],y[1],COLOR_WHITE,&actualLevel->view); 
	drawLine(x[0],y[0],x[2],y[2],COLOR_WHITE,&actualLevel->view);
	drawLine(x[2],y[2],x[3],y[3],COLOR_WHITE,&actualLevel->view);
	drawLine(x[1],y[1],x[3],y[3],COLOR_WHITE,&actualLevel->view);
 
}

/*============================================================
 F: Ajust Camera to actor view
 =============================================================*/

void ajustActorCamera(gameLevel *gl,Actor *ac)
{int ag;

		ag=ac->ago-ANGLE_45;
		if(ag<0) ag+=ANGLE_360;

		if(ag<=ANGLE_180) 
		{	CENTRALYVIEW+=2;
			if(CENTRALYVIEW>=100) CENTRALYVIEW=100;
		}
		else 
		{	CENTRALYVIEW-=2;
			if(CENTRALYVIEW<0) CENTRALYVIEW=0;
		}		 

		if((ag>ANGLE_90) && (ag<ANGLE_270))
		{	CENTRALXVIEW+=2;
			if(CENTRALXVIEW>=100) CENTRALXVIEW=100;
		}
		else 
		{	
			if((ag==ANGLE_90) || (ag==ANGLE_270))
			{
				if(CENTRALXVIEW<0) CENTRALXVIEW+=2;
				if(CENTRALXVIEW>0) CENTRALXVIEW-=2;	
			}
			else
			{	
				CENTRALXVIEW-=2;
				if(CENTRALXVIEW<=-100) CENTRALXVIEW=-100;
			}	
		}	
}


/*============================================================
 F: Camera position
 =============================================================*/

#define PASSPVIEW 128
void calcViewCamera(gameLevel *gl,Actor *ac)
{ 
	gl->yview=ac->yp;
	gl->xview=ac->xp;

	gl->cam.x=ac->xo;
	gl->cam.y=ac->yo;
	gl->cam.z=ac->zo;
	gl->cam.ag=ac->ago;


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
//	fadeOutViewImage(actualLevel->xview,actualLevel->yview,&actualLevel->frame);
	resetLevel(actualLevel);

	freeImage(&fonts.bmp);

	SfreeAudio();
}


/*============================================================
 F: Get string time
 =============================================================*/

void getStringTime(long time,char *str)
{int sec,min;

	sec=time/1000;
	min=sec/60;
	sec-=min*60;
	
	sprintf(str,"%d:%02d",min,sec);
}