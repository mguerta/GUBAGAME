#include "so.h"
#include "menu.h"

void menuMain(void);
void pauseMenu(long varTime);
int moveCursor(int joye,infoDir *dirInfo);
void printMenu(frameMenu *menu,BMPimage *view,int flgAtu);
void printMenuRoll(frameMenu *menu,BMPimage *view);
void transition(frameMenu *menu);
void printDirInfo(infoDir *dirInfo,BMPimage *view);
void runPlay(unsigned int joyp);
void initPlay(char *file,char *strExec); 
void resumePlay(void);
int execPlayer(fileType player, char *fileName,int action);
int changePlayerType(int direction,int type);
char * getNameToPath(char *name);
int changeDir(char *path);
void resetDEMO(void);
int runDEMO(void);
void resetMP3(void);
void runMP3(void);
void resumeGameDefault(void);

#define TIMEFADE 50
#define GENESISPREVIEW "/usr/bin/g"
#define GENESISFULL "/usr/bin/genesis"
#define SMSPREVIEW "/usr/bin/smsp"
#define SMSFULL "/usr/bin/sms"

#define MP3PLAYER "/usr/bin/mplayer"
#define JPGPREVIEW "/usr/bin/mp3player"
//#define STARTGAME "/mnt/data/games/extras/Adventures of Batman and Robin, The (JU) [!].bin"
#define STARTGAME "/mnt/data/jogos/MegaDrive/Sonic 3.bin.gz"

#define SOUNDCURSOR "./res/menu/sounds_roll2_v2.wav"
int CORBKGPREVIEW=0x0e0d1b;
int XPREVIEW=357;
int YPREVIEW=84+70;

fontText fontsGl[]={"./res/menu/fontGRBBold17.tga",29,32,4};

Animate2D aniAppleSel={"./res/menu/apple",TRANSPARENT_COLOR,1,1,CACHE}; 
Animate2D aniAppleNormal={"./res/menu/appleSmall",TRANSPARENT_COLOR,1,1,CACHE};
Animate2D aniStandBySel={"./res/menu/standby",TRANSPARENT_COLOR,1,1,CACHE}; 
Animate2D aniStandByNormal={"./res/menu/standbySmall",TRANSPARENT_COLOR,1,1,CACHE};
Animate2D aniMusicSel={"./res/menu/music",TRANSPARENT_COLOR,1,1,CACHE}; 
Animate2D aniMusicNormal={"./res/menu/musicSmall",TRANSPARENT_COLOR,1,1,CACHE};
Animate2D aniGameSel={"./res/menu/game",TRANSPARENT_COLOR,1,1,CACHE}; 
Animate2D aniGameNormal={"./res/menu/gameSmall",TRANSPARENT_COLOR,1,1,CACHE};

Animate2D aniDEMO={"./res/menu/demo",TRANSPARENT_COLOR,400,2,CACHE};

Animate2D aniBkg={"./res/menu/Bar-color",SOLID_COLOR,1000,1,CACHE}; 
Animate2D aniSega={"./res/menu/sega",SOLID_COLOR,150,15,CACHE};
Animate2D aniFolder={"./res/menu/foldern",TRANSPARENT_COLOR,150,1,CACHE,BMPOPTIMIZE};
Animate2D aniBack={"./res/menu/backn",TRANSPARENT_COLOR,150,1,CACHE,BMPOPTIMIZE};
Animate2D aniFile={"./res/menu/file",TRANSPARENT_COLOR,150,1,CACHE,BMPOPTIMIZE};
Animate2D aniMP3={"./res/menu/mp3",TRANSPARENT_COLOR,150,1,CACHE,BMPOPTIMIZE};
Animate2D aniMP4={"./res/menu/mp4",TRANSPARENT_COLOR,150,1,CACHE,BMPOPTIMIZE};
Animate2D aniGEN={"./res/menu/gens",TRANSPARENT_COLOR,150,1,CACHE,BMPOPTIMIZE};
Animate2D aniSMS={"./res/menu/sms",TRANSPARENT_COLOR,150,1,CACHE,BMPOPTIMIZE};
Animate2D aniJPG={"./res/menu/jpg",TRANSPARENT_COLOR,150,1,CACHE,BMPOPTIMIZE};
Animate2D aniUSB={"./res/menu/usb",TRANSPARENT_COLOR,150,1,CACHE,BMPOPTIMIZE};
Animate2D aniFUSB={"./res/menu/fusb",TRANSPARENT_COLOR,150,1,CACHE,BMPOPTIMIZE};
Animate2D aniJOY={"./res/menu/joy",TRANSPARENT_COLOR,150,1,CACHE};

Animate2D *listTypesFiles[]={&aniFile,&aniGEN,&aniGEN,&aniSMS,&aniFile,&aniFile,&aniFile,&aniMP3,&aniFile,&aniFile,&aniMP4,&aniJPG,&aniFUSB,&aniFolder,&aniBack,&aniFile,&aniFile,&aniFile,&aniFile,&aniFile,&aniFile,&aniFile,&aniFile};


tsound musicBkg = {"./res/menu/APRE.WAV",REPEATMUSIC,CACHE};
tsound somCursor = {SOUNDCURSOR,NORMALMUSIC,CACHE};

itemMenuRoll itemGame = {"JOGOS",&aniGameNormal};
itemMenuRoll itemMusic = {"MUSICAS",&aniMusicNormal};
itemMenuRoll itemPhotos = {"FOTOS",};
itemMenuRoll itemPower = {"DESLIGA",&aniStandByNormal};
itemMenuRoll itemVideo = {"VIDEOS"};

menuRoll menuRollTest = {5,5,0,&itemGame,&itemMusic,&itemPhotos,&itemPower,&itemVideo};

frameMenu menuTest=
{0,0,"./res/menu/Bar-color.tga",&musicBkg,&aniBkg,&menuRollTest
};

frameMenu *actualMenu;

char pathDir[MAXPATH]; 
char pathDirBKP[MAXPATH];
boxArea clipArea1={400-40+20,60+40,720-40,60+40+70-30};
boxArea clipArea2={400-40,360+40,720,360+40+70};
boxArea clipArea={80,42+70,80+250,42+380};
infoDir dirInfo={&pathDir,&clipArea,32};


int flgPlay=RUNMENU;
long timeResumePreview;

BMPimage framePlay;

int actualPlayer=TFARM;

int flgDoubleStart;

char actualExecNameFile[8000];
char actualExecFullNameFile[8000];
int flgScroll,idxScroll;
long timeScroll;
int actualPlayer;
int flgPENDRIVE=1;

long timeDEMO;
int flgDEMO;
int stsDEMO;


int	flgMP3;
int	stsMP3;
long timeMP3;

int flgGameStart;
long timeStartFull;


/*============================================================
 F: Entry Point 
 =============================================================*/
int Application(char *cmdLine)
{
	menuMain();
}

/*============================================================
 F: End Point 
 =============================================================*/

int stopApplication(void)
{

}

/*============================================================
 F: Menu loop 
 =============================================================*/
unsigned int joy=0;
void menuMain(void)
{//unsigned int joy=0; 

  char text[100];
  int fps=0,fpsg=0;
  int draw;
 // int i;

  
	
	SetSyncTimer(60);   //SET 60 FPS 

	actualMenu=&menuTest;

	actualMenu->view.buf=GetVideo();
	actualMenu->view.resx=GetVideoWidth();
	actualMenu->view.resy=GetVideoHeight();

	cloneImage(&actualMenu->view,&actualMenu->frame);
	//actualMenu->frame.buf=GetVideo();
	//actualMenu->frame.resx=GetVideoWidth();
	//actualMenu->frame.resy=GetVideoHeight();

	cloneImage(&actualMenu->view,&actualMenu->frameTransition);

/*	for(i=0;i<480;i+=2)
	{
		if(i&2)
		{
		 drawLine(0,i,719,i,COLOR_WHITE,&actualMenu->view); 
		 drawLine(0,i+1,719,i+1,COLOR_WHITE,&actualMenu->view); 
		}
		else
		{
		 drawLine(0,i,719,i,COLOR_BLACK,&actualMenu->view);
		 drawLine(0,i+1,719,i+1,COLOR_BLACK,&actualMenu->view);
		}
	}

	while(1) FlipVideo();  */

	strcpy(pathDir,getRootPath());
	dirInfo.tamList=getListFileDir(dirInfo.actualPath,&dirInfo.listGames);

	printMenu(actualMenu,&actualMenu->frame,0);
	actualMenu->transition=LOOPFADEOUT;
	transition(actualMenu);


	framePlay.buf=GetBufferPreview();
	framePlay.resx=320;
	framePlay.resy=224;

//	execPlayer(TFGEN,STARTGAME,PREVIEW);
	flgGameStart=1;
	timeResumePreview=getCurrentTime();

	resumeSound();
	startSound(actualMenu->music);

	do
	{
		actualMenu->view.buf=GetVideo();
	//	actualMenu->frame.buf=GetVideo();

		switch (flgPlay)
		{
			case RUNMENU:
			
			if( (actualPlayer!=TFMP3) && (actualPlayer!=TFMP4) )
			{

			//	runDEMO();

				if(getRenderAudio())
				{
#ifdef UNIQUINTERFACESOUND
				RenderAudioPreview();
#else
					RenderAudio();
#endif
				}
			}

			//if(actualPlayer==TFMP3)
			//	mp3_play();

			draw=getDrawFrame();
		

			if(draw) 
			{	
#ifdef SWAPVIDEO
			    printMenu(actualMenu,&actualMenu->view,1);
#else
				printMenu(actualMenu,&actualMenu->frame,1);
#endif

				//sprintf(text,"FPS MENU: %d/%d",fpsg,fps);
				//printImageText(actualMenu->xview+450,actualMenu->yview+400,text,&fontsGl[0],COLOR_RED |COLOR_GREEN,&actualMenu->frame);
				//printImageText(actualMenu->xview+450,actualMenu->yview+400,text,&fontsGl[0],COLOR_RED |COLOR_GREEN,&actualMenu->view);

#ifndef SWAPVIDEO
				transition(actualMenu);
#endif
			}

					
			if(draw) 
				FlipVideo();

		    WaitSyncTimer();

			joy=GetJoystick();
			moveCursor(joy,&dirInfo);

			if(getFPS(&fpsg,&fps))
				printf("FPS MENU %d/%d \n",fpsg,fps);

#ifdef SLOWMENU
			watchdog(8000);
#else
			if((actualPlayer==TFMP3) || (actualPlayer==TFMP4))
			{
//				if(actualPlayer==TFMP3) 
				runMP3();
			
                watchdog(11500); //13000);
			}
            else
			{
				if(!runDEMO())
					watchdog(6500);
			}
#endif 
			if(checkStandBy()) 
			{
				mute(0);
				execFile(GENESISFULL,"nada");
				cleanImageFast(&actualMenu->view,0);
			    tvOff();
				freeAudio();
				stopSound();
				flgPlay=STANDBY;
				setLowCPUConsume();

			}
				break;

			case RUNPLAY:
				runPlay(GetJoystick());
				if(checkStandBy()) 
				{
					mute(1);
					execFile(GENESISFULL,"nada");
					cleanImageFast(&actualMenu->view,0);
					tvOff();
					resumeVideo();
					
					flgPlay=STANDBY;
					setLowCPUConsume();
				}
				break;

			case STANDBY:
				if(runStandBy()) 
				{	setNormalCPUConsume();
					flgPlay=RUNMENU;

					changeDir(getRootPath());

					printMenu(actualMenu,&actualMenu->view,0);
					actualPlayer=TFARM;
					//execPlayer(TFGEN,STARTGAME,PREVIEW);
					flgGameStart=1;
					timeResumePreview=getCurrentTime();

					resumeSound();
					startSound(actualMenu->music);
					tvOn();
				}
				break;
		}

	}while(1);
}

/*============================================================
 F: Play game or midia 
 =============================================================*/

void runPlay(unsigned int joyp)
{
	if(flgDEMO>0)
	{
		watchdog(900000);
		joy=joyp;
		runDEMO();
	}
	else
	{
#if USBJOY
	if((joyp & (BTN_A | BTN_B | BTN_C)) == (BTN_A | BTN_B | BTN_C ))
#else
	if((joyp & (BTN_X | BTN_Y | BTN_Z | BTN_START)) == (BTN_X | BTN_Y | BTN_Z | BTN_START ))
#endif
		resumePlay();
	else
		watchdog(900000);
	}
//	printf("Joy = %x\n",joy);
}

/*============================================================
 F: init play 
 =============================================================*/

void initPlay(char *file,char *strExec)
{
    flgPlay=RUNPLAY;
    freeAudio();
	mute(2);
    stopSound();
    saveVideo();

    execFile(GENESISFULL,"nada");


    actualMenu->transition=LOOPFADEIN;
    transition(actualMenu);

	cleanImageFast(&actualMenu->frame,0);

	//printImageText(40,220,"Pressione START+X+Y+Z para retornar ao menu.",&fontsGl[0],COLOR_RED |COLOR_GREEN,&actualMenu->view);

	printImageText(70,220,"Pressione Start+X+Y+Z para voltar ao menu",&fontsGl[0],COLOR_RED |COLOR_GREEN,&actualMenu->frame);

	actualMenu->transition=LOOPFADEOUT;
    transition(actualMenu);

	FlipVideo();
	WaitSyncTimer();

	pauseMenu(3000);

	actualMenu->transition=LOOPFADEIN;
    transition(actualMenu);

    tvOff();

    execFile(strExec,file);

    watchdog(100000);
    tvOn();


}

/*============================================================
 F: resume play 
 =============================================================*/

void resumePlay(void)
{ char strExecFile[4096];

	flgPlay=RUNMENU;

	mute(0);

	strcpy(strExecFile,dirInfo.actualPath);
	strcat(strExecFile,dirInfo.listGames[dirInfo.lastIdxExec].nameFile);
	tvOff();
	execFile(GENESISFULL,"nada");

	resumeVideo();
//	resumeSound();
	tvOn();

	
	printMenu(actualMenu,&actualMenu->frame,0);
	actualMenu->transition=LOOPFADEOUT;
	transition(actualMenu);
	//startSound(actualMenu->music);

	timeResumePreview=getCurrentTime();

	resetDEMO();

	resumeSound();
	startSound(actualMenu->music);
}


/*============================================================
 F: move cursor diretorio 
 =============================================================*/

int moveCursor(int joye,infoDir *dirInfo)
{int i;
 unsigned int joy=0;
 char strExecFile[4096];
 int ret=0;

		if(joye==dirInfo->joyLast)
		{
			if(getCurrentTime()-dirInfo->timeRepetJoy>dirInfo->timeRepeat)
			{
				dirInfo->timeRepetJoy=getCurrentTime();
				joy=joye;
				dirInfo->joyLast=joye;
				dirInfo->timeRepeat=30;
			}
		}
		else
		{	dirInfo->joyLast=joye;
			joy=joye;
			dirInfo->timeRepetJoy=getCurrentTime();
			dirInfo->timeRepeat=1000;
		}

		if(joy & BTN_LEFT) 
		{
			flgDoubleStart=0;

			if(dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].type==TYPE_FILE)
			{
				dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].subType=
					changePlayerType(BTN_LEFT,dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].subType);

			}
		}
		if(joy & BTN_RIGHT)
		{

			flgDoubleStart=0;
			if(dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].type==TYPE_FILE)
			{
				dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].subType=
					changePlayerType(BTN_RIGHT,dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].subType);;

			}
		}

		if(joy & BTN_UP) 
		{	

			flgDoubleStart=0;
			dirInfo->flgScroll=0;
			dirInfo->posCursor--;
			if(dirInfo->posCursor<0)
			{   dirInfo->posPage--;
				if(dirInfo->posPage<0) 
				{
					//dirInfo->posPage=0;	
					dirInfo->posPage=(dirInfo->tamList-dirInfo->numListView);
					dirInfo->posCursor=dirInfo->numListView-1;
					ret=1;
				}
				else
					dirInfo->posCursor=0;
			}
		}
		if(joy & BTN_DOWN) 
		{	

			flgDoubleStart=0;
			dirInfo->flgScroll=0;
			dirInfo->posCursor++;
			if(dirInfo->posCursor>dirInfo->numListView-1) 
			{
				dirInfo->posCursor--;
				dirInfo->posPage++;
				if((dirInfo->posPage+dirInfo->numListView)>dirInfo->tamList)
				{
					//dirInfo->posPage--;
					dirInfo->posPage=0;
					dirInfo->posCursor=0;
					ret=1;
				}	
			}

		}

		if(joy & (BTN_START | BTN_A | BTN_B | BTN_C | BTN_X | BTN_Y | BTN_Z))
		{

			dirInfo->flgScroll=0;
			if(dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].type & (TYPE_DIR | TYPE_BACKDIR))
			{
				if(dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].type == TYPE_BACKDIR)
				{
					i=strlen(dirInfo->actualPath)-1;
					while(i-->0) 
						if(dirInfo->actualPath[i]=='/') break;
					i++;
					dirInfo->actualPath[i]=0;

				}
				else
				{
					strcat(dirInfo->actualPath,dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].nameFile);
					strcat(dirInfo->actualPath,"/");
				}
				changeDir(dirInfo->actualPath);
			}
			else
			{
				strcpy(strExecFile,dirInfo->actualPath);
				strcat(strExecFile,dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].nameFile);


				if(flgDoubleStart==0)
				{
						flgDoubleStart=1;
						timeStartFull=getCurrentTime();
						execPlayer(dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].subType,strExecFile,PREVIEW);
				}
				else
				{
					if(getCurrentTime()-timeStartFull>400)
					{
						flgDoubleStart=0;
						timeStartFull=0;
						execPlayer(dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].subType,strExecFile,FULLSCREEN); 
					}
				}
				
				dirInfo->lastIdxExec=dirInfo->posPage+dirInfo->posCursor;

/*				flgDoubleStart=1-flgDoubleStart;
				dirInfo->lastIdxExec=dirInfo->posPage+dirInfo->posCursor;
				if(flgDoubleStart)
					execPlayer(dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].subType,strExecFile,PREVIEW);
				else
					execPlayer(dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].subType,strExecFile,FULLSCREEN); */

				timeResumePreview=0;
			}
		}
	/*	else
		{
			if(joye==0) 
			{
				if(!dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].type & (TYPE_DIR | TYPE_BACKDIR))
				{	if((dirInfo->posPage+dirInfo->posCursor)!=dirInfo->lastIdxExec)
					{
						//setSoundChannel(SOUNDCURSOR,7,0);

						strcpy(strExecFile,dirInfo->actualPath);
						strcat(strExecFile,dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].nameFile);

						dirInfo->lastIdxExec=dirInfo->posPage+dirInfo->posCursor;
						//execFile(XPREVIEW,YPREVIEW,GENESISPREVIEW,strExecFile);
						execPlayer(dirInfo->listGames[dirInfo->posPage+dirInfo->posCursor].subType,strExecFile,PREVIEW);
			

						timeResumePreview=0;
					}
				}
			}
		}  */

		if(timeResumePreview)
		{

			if(getCurrentTime()-timeResumePreview>1000)
			{
					timeResumePreview=0;

					if(flgGameStart)
					{
						flgGameStart=0;
						execPlayer(TFGEN,STARTGAME,PREVIEW);
					}
					else
					{

						strcpy(strExecFile,dirInfo->actualPath);
						strcat(strExecFile,dirInfo->listGames[dirInfo->lastIdxExec].nameFile);
						execPlayer(dirInfo->listGames[dirInfo->lastIdxExec].subType,strExecFile,PREVIEW);
						//flgDoubleStart=1-flgDoubleStart;
					}

			}
		}

		return ret;
}


/*============================================================
 F: Print dirInfo 
 =============================================================*/

void printDirInfo(infoDir *dirInfo,BMPimage *view)
{int i,j=0,color,x;
 long delta;

	for(i=dirInfo->posPage;i<dirInfo->tamList;i++)
	{	
		color=COLOR_BLACK;
		if(i==(dirInfo->posPage+dirInfo->posCursor)) 
		{	color=COLOR_RED | COLOR_GREEN;
		    if(dirInfo->flgScroll==0) dirInfo->idxScroll=0;
			x=printImageTextClip(dirInfo->viewClip->x1-dirInfo->idxScroll,dirInfo->viewClip->y1+j*dirInfo->interval,dirInfo->listGames[i].nameFile,&fontsGl[0],color,view,dirInfo->viewClip);

			switch(dirInfo->flgScroll)
			{
				case 0:
					if(x>=dirInfo->viewClip->x2)
						dirInfo->flgScroll=1;
					break;
				case 1:
					if(getCurrentTime()-dirInfo->timeScroll>1000)
					{
						dirInfo->timeScroll=getCurrentTime();
						dirInfo->idxScroll=0;
						dirInfo->flgScroll=2;
					}
					break;
				case 2:
					delta=getCurrentTime()-dirInfo->timeScroll;
					if(delta>16)
					{
						dirInfo->timeScroll=getCurrentTime();
						dirInfo->idxScroll+=delta/16;
						if(x<=dirInfo->viewClip->x1)
						{
							dirInfo->idxScroll=0;
							dirInfo->flgScroll=1;
						}
					}
					break;

			}

		}
		else
		{
			x=printImageTextClip(dirInfo->viewClip->x1,dirInfo->viewClip->y1+j*dirInfo->interval,dirInfo->listGames[i].nameFile,&fontsGl[0],color,view,dirInfo->viewClip);
		}
		
		if(!x)
		{  

			break;
		}
		else
		{

				printAnimate2D(dirInfo->viewClip->x1-26,dirInfo->viewClip->y1+j*dirInfo->interval+7,listTypesFiles[dirInfo->listGames[i].subType],view);
		}
		j++;
		
	}
	dirInfo->numListView=j;
}

/*============================================================
 F: Print Menu 
 =============================================================*/

void printMenu(frameMenu *menu,BMPimage *view,int flgAtu)
{int delta,x;
 char patht[MAXPATH]; 
 int flgAtuDir=0;
 int bar,ret;
 

	if(flgAtu==0)
	{
		printAnimate2D(menu->xview,menu->yview,menu->aniBackground,view);
	
		printDirInfo(&dirInfo,view);

		x=printImageTextClip(clipArea1.x1-idxScroll,clipArea1.y1,actualExecNameFile,&fontsGl[0], COLOR_BLUE ,view,&clipArea1);	

		if(strlen(actualExecNameFile)>0)
			printAnimate2D(clipArea1.x1-25,clipArea1.y1+8,listTypesFiles[actualPlayer],view);


	}
	else
	{
		refreshAreaImage(clipArea2.x1,clipArea2.y1,clipArea2.x2,clipArea2.y2,&menu->aniBackground->frame[0],view);
		
		x=0;
		ret=checkPENDRIVE();
		if(ret)
		{
				if(ret==2)
				{
					execPlayer(actualPlayer,actualExecFullNameFile,PREVIEW);
				}
				else
				{
				if(flgPENDRIVE)
				{
					if(strcmp(dirInfo.actualPath,getRootPath())==0)
					{
						flgAtuDir=1;
						flgPENDRIVE=0;
					}
				}

			//	flgPENDRIVE=0;
				printAnimate2D(clipArea2.x1+x,clipArea2.y1,&aniUSB,view);
				x+=64;
				}
		}
		else
		{
			if(flgPENDRIVE==0)
			{
				strcpy(patht,getRootPath());
				strcat(patht,"usb");
				if(strstr(dirInfo.actualPath,patht))
				{
					flgAtuDir=1;
					//timeResumePreview=0;
					//execPlayer(TFGEN,STARTGAME,PREVIEW);
					flgGameStart=1;
					timeResumePreview=getCurrentTime();
				}

				if(strcmp(dirInfo.actualPath,getRootPath())==0) 
				{
					flgAtuDir=1;
				}
			}
			flgPENDRIVE=1;
		}

		if(checkJOY())
		{
			printAnimate2D(clipArea2.x1+x,clipArea2.y1,&aniJOY,view);
			x+=64;
		}

		if(flgDEMO)
		{
			printAnimate2D(clipArea2.x1+x,clipArea2.y1,&aniDEMO,view);
		}

		if(flgAtuDir)
		{
			changeDir(getRootPath());
		}

		refreshAreaImage(clipArea.x1-44,clipArea.y1,clipArea.x2+14,clipArea.y2+10,&menu->aniBackground->frame[0],view);
		printDirInfo(&dirInfo,view);

		refreshAreaImage(clipArea1.x1-25,clipArea1.y1,clipArea1.x2,clipArea1.y2,&menu->aniBackground->frame[0],view);
		x=printImageTextClip(clipArea1.x1-idxScroll,clipArea1.y1,actualExecNameFile,&fontsGl[0], COLOR_BLUE ,view,&clipArea1);	

		if(strlen(actualExecNameFile)>0)
			printAnimate2D(clipArea1.x1-25,clipArea1.y1+8,listTypesFiles[actualPlayer],view);

		switch(flgScroll)
		{
			case 0:
				if(x>=clipArea1.x2)
					flgScroll=1;
				break;
			case 1:
				if(getCurrentTime()-timeScroll>1000)
				{
					timeScroll=getCurrentTime();
					idxScroll=0;
					flgScroll=2;
				}
				break;
			case 2:
				delta=getCurrentTime()-timeScroll;
				if(delta>16)
				{
					timeScroll=getCurrentTime();
					idxScroll+=delta/16;
					if(x<=clipArea1.x1)
					{
						idxScroll=0;
						flgScroll=1;
					}
				}
				break;
		}

		if(actualPlayer==TFMP3) 
		{
//			refreshAreaImage(XPREVIEW,YPREVIEW,XPREVIEW+320,YPREVIEW+240,&menu->aniBackground->frame[0],&actualMenu->view);
		    if(stsMP3==0)
			{
				printAnimate2D(XPREVIEW,YPREVIEW,&aniSega,&actualMenu->view);
				if(getLenMPlayer()>0)
				bar=((getPosMPlayer())*320)/(getLenMPlayer());
				else bar=0;
				drawBOX(XPREVIEW,YPREVIEW+240-20,XPREVIEW+320,YPREVIEW+242,COLOR_BLACK,&actualMenu->view); 
				drawBOX(XPREVIEW,YPREVIEW+240-18,XPREVIEW+bar,YPREVIEW+240,COLOR_RED,&actualMenu->view); 
				sprintf(patht,"Play : %d : %d",getLenMPlayer(),getPosMPlayer());
				printImageText(XPREVIEW+10,YPREVIEW+212,patht,&fontsGl[0],COLOR_WHITE,&actualMenu->view);

			}
			else
			{
				if(stsMP3==1)
				{
					stsMP3=2;
					printAnimate2D(XPREVIEW,YPREVIEW,&aniSega,&actualMenu->view);
					if(getLenMPlayer()>0)
					bar=((getPosMPlayer())*320)/(getLenMPlayer());
					else bar=0;
					drawBOX(XPREVIEW,YPREVIEW+240-20,XPREVIEW+320,YPREVIEW+242,COLOR_BLACK,&actualMenu->view); 
					drawBOX(XPREVIEW,YPREVIEW+240-18,XPREVIEW+bar,YPREVIEW+240,COLOR_RED,&actualMenu->view); 
					sprintf(patht,"Pause: %d : %d",getLenMPlayer(),getPosMPlayer());
					printImageText(XPREVIEW+10,YPREVIEW+212,patht,&fontsGl[0],COLOR_WHITE,&actualMenu->view);
				}
			}

	}

		if(actualPlayer==TFMP4) 
		{
		    if(stsMP3==0)
			{
				if(getLenMPlayer()>0)
				bar=((getPosMPlayer())*320)/(getLenMPlayer());
				else bar=0;
				drawBOX(XPREVIEW,YPREVIEW+240-20,XPREVIEW+320,YPREVIEW+242,COLOR_BLACK,&actualMenu->view); 
				drawBOX(XPREVIEW,YPREVIEW+240-18,XPREVIEW+bar,YPREVIEW+240,COLOR_RED,&actualMenu->view); 
				sprintf(patht,"Play : %d : %d",getLenMPlayer(),getPosMPlayer());
				printImageText(XPREVIEW+10,YPREVIEW+212,patht,&fontsGl[0],COLOR_WHITE,&actualMenu->view);
			}
			else
			{
				//if(stsMP3==1)
				{
				//	stsMP3=2;


					if(getLenMPlayer()>0)
					bar=((getPosMPlayer())*320)/(getLenMPlayer());
					else bar=0;
					drawBOX(XPREVIEW,YPREVIEW+240-20,XPREVIEW+320,YPREVIEW+242,COLOR_BLACK,&actualMenu->view); 
					drawBOX(XPREVIEW,YPREVIEW+240-18,XPREVIEW+bar,YPREVIEW+240,COLOR_RED,&actualMenu->view); 
					sprintf(patht,"Pause: %d : %d",getLenMPlayer(),getPosMPlayer());
					printImageText(XPREVIEW+10,YPREVIEW+212,patht,&fontsGl[0],COLOR_WHITE,&actualMenu->view);
				}
			}
		}


#ifdef UNIQUINTERFACE
		putImageSolid(357,162,&framePlay,view);
#endif


	}


}


/*============================================================
 F: pause 
 =============================================================*/

void pauseMenu(long varTime)
{long delta;
 long time;

	time=getCurrentTime();

	do
	{
		if(getCurrentTime()-time>varTime)
		{
				break;
		}
	}while(1);

}

/*============================================================
 F: Transition Menu 
 =============================================================*/

void transition(frameMenu *menu)
{ long delta;

	switch(menu->transition)
	{
		case NOTRANSITION:
			//putViewImage(0,0,&menu->frame);
			//putImage(0,0,&menu->frame,&menu->view,CORBKGPREVIEW);
			refreshAreaImage(clipArea.x1-44,clipArea.y1,clipArea.x2+14,clipArea.y2+10,&menu->frame,&menu->view);

			refreshAreaImage(clipArea1.x1-25,clipArea1.y1,clipArea1.x2,clipArea1.y2,&menu->frame,&menu->view);
			refreshAreaImage(clipArea2.x1,clipArea2.y1,clipArea2.x2,clipArea2.y2,&menu->frame,&menu->view);

			break;
		case FADEOUT:
				menu->timeTransition=getCurrentTime();
				menu->varTransition=16;
				menu->transition=RUNFADEOUT;

		case RUNFADEOUT:
			if(getCurrentTime()-menu->timeTransition>TIMEFADE)
			{
				menu->varTransition/=2;
				menu->timeTransition=getCurrentTime();
				if(menu->varTransition<=1) menu->transition=NOTRANSITION;
			}
			putViewImageLevelColor(0,0,&menu->frame,&menu->frameTransition,menu->varTransition);
			putViewImage(0,0,&menu->frameTransition);
			break;

		case LOOPFADEIN:
			menu->varTransition=1;
			menu->timeTransition=0;
			while(menu->varTransition<16)
			{
				delta=getCurrentTime()-menu->timeTransition;
				if(delta>TIMEFADE)
				{
					if(getDrawFrame());
					{ 
						putViewImageLevelColor(0,0,&menu->frame,&menu->frameTransition,menu->varTransition);
						putViewImage(0,0,&menu->frameTransition);
					}

					menu->varTransition*=2;
					menu->timeTransition=getCurrentTime();

					FlipVideo();
					WaitSyncTimer();
				}

			}
			menu->transition=NOTRANSITION;
			break;

		case LOOPFADEOUT:
			menu->varTransition=32;
			menu->timeTransition=0;
			while(menu->varTransition>=1)
			{
				delta=getCurrentTime()-menu->timeTransition;
				if(delta>TIMEFADE)
				{
					
					menu->varTransition/=2;
					menu->timeTransition=getCurrentTime();

					if(getDrawFrame());
					{
						putViewImageLevelColor(0,0,&menu->frame,&menu->frameTransition,menu->varTransition);
						putViewImage(0,0,&menu->frameTransition);
					}

					FlipVideo();
					WaitSyncTimer();
				}
			}

	
			menu->transition=NOTRANSITION;
			break;
	}
}
/*============================================================
 F: select player  
 ============================================================*/


int execPlayer(fileType player, char *fileName,int action)
{

	flgScroll=0;
	idxScroll=0;
	strcpy(actualExecFullNameFile,fileName);
	strcpy(actualExecNameFile,getNameToPath(fileName));


	switch (player)
	{

		case TFGEN :
			if(action==PREVIEW)
			{
				execFile(GENESISPREVIEW,fileName);
				if(actualPlayer==TFSMS)
				{
					putViewImage(0,0,&actualMenu->frame); // refresh view
					resumeMute();
				}
				if(actualPlayer==TFJPG)
					putViewImage(0,0,&actualMenu->frame); // refresh view
				if((actualPlayer==TFMP3) || (actualPlayer==TFMP4))
				{
					//printMenu(actualMenu,&actualMenu->frame,0);
					putViewImage(0,0,&actualMenu->frame); // refresh view
					resumeSound();
					startSound(actualMenu->music);
				}
				actualPlayer=TFGEN;
			}
			else
				initPlay(fileName,GENESISFULL);
		   break;
		
		case TFSMS :
			if(action==PREVIEW)
			{
				execFile(SMSPREVIEW,fileName);
				if(actualPlayer==TFSMS)
				{
					putViewImage(0,0,&actualMenu->frame); // refresh view
					//resumeMute();
				}
				if(actualPlayer==TFJPG)
					putViewImage(0,0,&actualMenu->frame); // refresh view
				if((actualPlayer==TFMP3) || (actualPlayer==TFMP4))
				{
					putViewImage(0,0,&actualMenu->frame); // refresh view
					resumeSound();
					startSound(actualMenu->music);
				}
				actualPlayer=TFSMS;
			}
			else
				initPlay(fileName,SMSFULL);
		   break;


		case TFMP3 :
			//if(action==FULLSCREEN)

			 if((actualPlayer==TFMP3) && (flgMP3<2))
				 if(stsMP3>0) stsMP3=0;
				 else		  stsMP3=1;
			 else
			{	
				freeAudio();
				mute(3);
				stopSound();	
				if(actualPlayer!=TFMP3)
					putViewImage(0,0,&actualMenu->frame); // refresh view
				execFile(MP3PLAYER,fileName); 
			//	startMusicMenu(fileName,0);

				resetMP3();
				actualPlayer=TFMP3;
			}
			
		   break;

		   case TFMP4 :
			//if(action==FULLSCREEN)
		    if((actualPlayer==TFMP4) && (flgMP3<2))
				 if(stsMP3>0) stsMP3=0;
				 else		  stsMP3=1;
			 else
			{	
				freeAudio();
				mute(4);
				stopSound();					
				putViewImage(0,0,&actualMenu->frame); // refresh view
				execFile(MP3PLAYER,fileName);
				
				resetMP3();
				actualPlayer=TFMP4;
			}
			
		   break;

		case TFJPG :
			//if(action==FULLSCREEN)
			{
				//execFileNoKill(JPGPREVIEW,fileName);
				execFile(JPGPREVIEW,fileName);
				if(actualPlayer==TFSMS)
				{
					putViewImage(0,0,&actualMenu->frame); // refresh view
					resumeMute();
				}
				if((actualPlayer==TFMP3) || (actualPlayer==TFMP4))
				{
					//printMenu(actualMenu,&actualMenu->frame,0);
					putViewImage(0,0,&actualMenu->frame); // refresh view
					resumeSound();
					startSound(actualMenu->music);
				}
				actualPlayer=TFJPG;
			}
			break;

	}

	return 0;

}


/*============================================================
 F: get name full path	  
 ============================================================*/

char * getNameToPath(char *name)
{char *bp;
	
	bp=name;
	while(*bp!=0) bp++;

	while((bp>name)) 
	{
		if(*bp=='\\') break;
		if(*bp=='/') break;
		bp--;
	}
	bp++;
	

	return bp;
}


/*============================================================
 F: change player type  
 ============================================================*/

#define TAMLISTPLAYERS 6
int listPlayers[TAMLISTPLAYERS]={TFARM,TFGEN,TFSMS,TFMP3,TFMP4,TFJPG};

int changePlayerType(int direction,int type)
{int i;

	for(i=0;i<TAMLISTPLAYERS;i++)
	{
		if(listPlayers[i]==type) break;
	}
	
	if(direction==BTN_LEFT) i--;
	if(direction==BTN_RIGHT) i++;

	if(i<0) i=TAMLISTPLAYERS-1;
	if(i>=TAMLISTPLAYERS) i=0;

	return listPlayers[i];
	
}

/*============================================================
 F: change dir
 =============================================================*/

int changeDir(char *path)
{int tam;

		strcpy(pathDirBKP,pathDir);
		strcpy(pathDir,path);
		tam=getListFileDir(dirInfo.actualPath,&dirInfo.listGames);

		if(tam>0)
		{
			flgDoubleStart=0;
			dirInfo.flgScroll=0;
			dirInfo.posCursor=0;
			dirInfo.posPage=0;
			dirInfo.lastIdxExec=0;
			dirInfo.tamList=tam; //getListFileDir(dirInfo.actualPath,&dirInfo.listGames);
		}
		else
			strcpy(pathDir,pathDirBKP);

		return tam;
}

/*============================================================
 F: reset Demo
 =============================================================*/

void resetDEMO(void)
{
	flgDEMO=0;
	stsDEMO=0;
	timeDEMO=getCurrentTime();
}


/*============================================================
 F: run Demo
 =============================================================*/

#define SLEEPTIMEDEMO 60000 //60000
#define TIMEDEMO 60000 //60000

int runDEMO(void)
{int ret=0;

	if(joy!=0) 
	{
		if(flgDEMO==4) 
		{	resumePlay();
			ret=1;
		}
		resetDEMO();
	}

	switch(flgDEMO)
	{
		case 0:
			if(timeDEMO==0) 
			{
				resetDEMO();
				break;
			}
			if((getCurrentTime()-timeDEMO)>SLEEPTIMEDEMO)
			{
				timeDEMO=getCurrentTime();
				flgDEMO=1;
				if(changeDir(getDEMOPath())>0)
				{
					moveCursor(BTN_DOWN,&dirInfo);
					moveCursor(BTN_START,&dirInfo);
				}
				else
					flgDEMO=0;

			}
			break;

		case 1:
		/*	if(!moveCursor(BTN_DOWN,&dirInfo))
				flgDEMO=2;
			else
				flgDEMO=1; */
			do
			{
				dirInfo.joyLast=0;
				moveCursor(BTN_DOWN,&dirInfo);
				if((dirInfo.listGames[dirInfo.posPage+dirInfo.posCursor].subType==TFGEN)
					//|| (dirInfo.listGames[dirInfo.posPage+dirInfo.posCursor].subType==TFSMS)
					)
					break;
			}while(1);
			flgDEMO=2;
			break;
		case 2:
			joy=BTN_START;
			moveCursor(BTN_START,&dirInfo);
			flgDEMO=3;
			timeDEMO=getCurrentTime();
			break;
		case 3:
			if((getCurrentTime()-timeDEMO)>TIMEDEMO)
			{
				timeDEMO=getCurrentTime();
				//flgDEMO=1;
				flgDEMO=4;
				//joy=BTN_START;
				moveCursor(BTN_START,&dirInfo);
			}
			break;

		case 4:
			if((getCurrentTime()-timeDEMO)>TIMEDEMO)
			{
				resumePlay();
				flgDEMO=1;
				ret=1;
			}
			break;
	}

	return ret;
}


/*============================================================
 F: reset play mplayer
 =============================================================*/

void resetMP3(void)
{
	flgMP3=0;
	stsMP3=0;
	timeMP3=getCurrentTime();
	setPause(stsMP3);
	resetVarMPLayer();
}


/*============================================================
 F: run Demo
 =============================================================*/

#define SLEEPTIMEMP3 500
#define TIMEMP3 2000
#define RESUMETIMEMP3 5000

void runMP3(void)
{
	if((flgMP3>0) && (flgMP3<3))
	{
			if((joy & BTN_DOWN) || (joy & BTN_UP)) flgMP3=3;
			setPause(stsMP3);
	}
	switch(flgMP3)
	{
		case 0: 
			if((getCurrentTime()-timeMP3)>SLEEPTIMEMP3)
			{
				flgMP3=1;
			}
			break;

		case 1:
			if(getEndMPlayer())
			{
				flgMP3=2;
				timeMP3=getCurrentTime();
			}
			break;
		case 2:
			if((getCurrentTime()-timeMP3)>TIMEMP3)
			{
				do
				{
					dirInfo.joyLast=0;
					moveCursor(BTN_DOWN,&dirInfo);
					if(dirInfo.listGames[dirInfo.posPage+dirInfo.posCursor].subType==TFMP3) break;
				}while(1);
				moveCursor(BTN_START,&dirInfo);
			    resetMP3();
			}
			break;

		case 3:
			if(getEndMPlayer())
			{
				flgMP3=4;
				timeMP3=getCurrentTime();
			}
			break;

		case 4:
			if((getCurrentTime()-timeMP3)>RESUMETIMEMP3)
			{
				resumeGameDefault();
				flgMP3=5;
			}
			break;

	}
	

}
/*============================================================
 F: Resume Game Default
 =============================================================*/

void resumeGameDefault(void)
{
		resumeMute();
		resetDEMO();
		putViewImage(0,0,&actualMenu->frame); // refresh view
		resumeSound();
		startSound(actualMenu->music);
		//execPlayer(TFGEN,STARTGAME,PREVIEW);
		flgGameStart=1;
		timeResumePreview=getCurrentTime();
}
