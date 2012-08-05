#include <android/log.h>#include <time.h>#include <stdio.h>#include <stdlib.h>#include <unistd.h>#include <math.h>#include <fcntl.h> #include "so.h"    // Windows specific functions and definitions#include "sound.h"//#include "game.h"#include "dirent.h"#include <unistd.h>#include <zip.h>#define SND_BUFSIZE  256#define SND_HEADERS  8#define ROOTPATH "/mnt/data/"#define DEMOPATH "/mnt/data/jogos/MegaDrive/"struct zip* APKArchive;int HandleKeys(int Key,int Down);void PlayBuffers(void);#define  LOG_TAG    "libplasma"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
 double firstTime=0,nowTime;// Main screen buffer pixel *ScrBuffer;  char kbp[10000];  int kbfd = 0;  unsigned long maskJoy=0,mJoy2=0;// Joystick state extern unsigned int JoyState;int flgRenderAudioPreview=0;unsigned short silenc[2000];// Filename char Filename[256];// Audio buffer sample SndBuffer[SND_BUFSIZE]; sample *SndWPtr,*SndRPtr; int sizeActualSndBuffer; int CurHdr;// Timer variables int TimerID; int TimerReady;tmouse mouse; long currentTime=0; long timeJoyRecovery=0; long timeUSBRecovery=0; static long int screensize = 0; static int x = 40, y =0;int szline;long long tempo,now,tempof,tempop;long dif,diff;int fpsc,cf,fpsg,cfg; int fps;
 int ffps=0,fvps=0,rffps,rfvps;
 long timeFPS=0;int statusDraw;int countSound;long memoBusy=0;int flagMute=0;typedef struct _mapMalloc{	void *p;	long size;} mapMalloc;int idxListMalloc;mapMalloc listMalloc[10000];extern unsigned char *fb;extern float xTouch,yTouch,xiTouch,yiTouch;extern int typeTouch; int comp(const void *s1, const void *s2);int VVIDEO_WIDTH = VIDEO_WIDTH;  
int VVIDEO_HEIGHT = VIDEO_HEIGHT;

void setActualView(int w,int h)
{
	VVIDEO_WIDTH=w;
	VVIDEO_HEIGHT=h;
}//void mute(void)void mute(int i){}void resumeMute(void){	flagMute=0;}//int Application(const char *CmdLine)int mainAndroid(void){  	SndWPtr     = SndBuffer+1;  	SndRPtr     = SndBuffer;  	memset(SndBuffer,0,SND_BUFSIZE*sizeof(sample));	statusDraw=0;	countSound=0;	//Application(argv[1]);}/** PlayBuffers() ********************************************//** This internal function plays ready audio buffers.       **//*************************************************************/ void PlayBuffers(void){ }/*============================================================ F: Get mouse information =============================================================*/tmouse * getMouseInf(void){	mouse.x=xTouch;
	mouse.y=yTouch;	mouse.status=0;	if(typeTouch!=0)		mouse.status=MOUSE_DOWN;	else		mouse.status=0;	return &mouse; }/*============================================================ F: Set mouse information =============================================================*/void setMousePosition(tmouse * mo){	mouse.x=mo->x;	mouse.y=mo->y;}/** HandleKeys() *********************************************//** Handle key presses/releases. Returns 1 if a key was     **//** recognized, 0 otherwise.                                **//*************************************************************/ int HandleKeys(int Key,int Down){  /* Done */  return(0);}//** GetVideo() *****************************************************//** Get address of the video buffer that can be written by your   **//** program, i.e. currently INACTIVE video buffer. The other two  **//** functions return active (front) and inactive (back) buffers.  **//*******************************************************************pixel *GetVideo(void) { return((pixel *)fb); }pixel *GetFrontVideo(void) { return((pixel *)fb); }pixel *GetBackVideo(void) { return((pixel *)fb); }//** GetVideoWidth()/GetVideoHeight() *******************************//** Get video buffer dimensions.                                  **//*******************************************************************int GetVideoWidth(void) { return(VVIDEO_WIDTH); }int GetVideoHeight(void) { return(VVIDEO_HEIGHT); }//** FlipVideo() ****************************************************//** Switch displayed video buffer from VBUF0 to VBUF1 and back.   **//** Returns the address of the INACTIVE video buffer that can be  **//** modified by your program. The ACTIVE video buffer cannot be   **//** written to.                                                   **//*******************************************************************int fr=0;pixel *FlipVideo(void){    return((pixel *)fb);//    return 0;}//** RequestFlip() **************************************************//** Request a video buffer flip. The FlipReady() function will    **//** return 1 as soon as the video buffers have been flipped. Use  **//** RequestFlip()/FlipRead() functions instead of the FlipVideo() **//** function when you wish to do computations while waiting for   **//** the buffers to be flipped.                                    **//*******************************************************************void RequestFlip(void) { FlipVideo(); }//** FlipReady() ****************************************************//** This function will return new buffer address as soon as the   **//** video buffers are flipped after a call to RequestFlip(). It   **//** returns 0 otherwise.                                          **//*******************************************************************pixel *FlipReady(void) { return(ScrBuffer); }//** GetJoystick() **************************************************//** Get the state of joypad buttons (1 means "pressed"). Refer to **//** the BTN_* #defines for the button mapping information. Notice **//** that on Windows this function automatically calls ProcessMsgs **//** to process Windows messages.                                  **//*******************************************************************unsigned int GetJoystick(void){/*int xp,yp,xpi,ypi;  xp=xTouch;
  yp=yTouch;  xpi=xiTouch;  ypi=yiTouch;    if((xp-xpi)<0) JoyState=BTN_UP;
    else           JoyState=BTN_DOWN;       // tombado
    
    if((yp-ypi)<-100) JoyState=BTN_RIGHT;
    if((yp-ypi)>100) JoyState=BTN_LEFT; 	if((xp+yp)==0) JoyState=0; */	    return(JoyState);}//** WaitJoystick() *************************************************//** Wait until one or more of the buttons given in the Mask have  **//** been pressed. Returns the bitmask of pressed buttons. This    **//** function will also eliminate jitter. Refer to BTN_* #defines  **//** for the button mapping information.                           **//*******************************************************************unsigned int WaitJoystick(unsigned int Mask){  return(0);}//** GetAudio() *****************************************************//** Get base address of the audio buffer.                         **//*******************************************************************sample *GetAudio(void) { return(SndBuffer); }//** GetTotalAudio() ************************************************//** Get the total length of the audio buffer, in samples.         **//*******************************************************************unsigned int GetTotalAudio(void) { return(sizeof(SndBuffer)); }//** GetFreeAudio() *************************************************//** Get the amount of free space in the audio buffer, in samples. **//*******************************************************************unsigned int GetFreeAudio(void){  int J;  J=(int)(SndRPtr-SndWPtr);  return(J>0? J:J+SND_BUFSIZE);}//** WriteAudio() ***************************************************//** Write up to a given number of samples into the audio buffer.  **//** Returns the number of samples written.                        **//*******************************************************************unsigned int WriteAudio(sample *Data,unsigned int Length){    unsigned int J;
/*
  // Can't write more than the free space
  J=GetFreeAudio();
  if(J<Length) Length=J;

  // Copy data
  for(J=0;J<Length;J++)
  {
    *SndWPtr++=Data[J];
    if(SndWPtr-SndBuffer>=SND_BUFSIZE) SndWPtr=SndBuffer;
  }

  // Enqueue buffers ready to play
  PlayBuffers(); */

	sizeActualSndBuffer=0;

	for(J=0;J<Length;J++) SndBuffer[J]=Data[J];

	sizeActualSndBuffer=Length;

  // Done
  return(Length);}//** WaitSyncTimer() ************************************************//** Wait for the timer to become ready.                           **//*******************************************************************void WaitSyncTimer(void){  TimerReady=0;}//** SetSyncTimer() *************************************************//** Set synchronization timer to a given frequency in Hz.         **//*******************************************************************int SetSyncTimer(int Hz){  /* Return allocated timer ID */  return(TimerID);}/*=================================================F: Test file exist===================================================*/int testFileExist(char *nameFile) {FILE *arq; int ret=0;  arq=fopen(nameFile,"rb");  if(arq)  {	ret=1;	fclose(arq);  }  return ret; }/*=================================================F: Read a file ===================================================*/long readFile(char *nameFile, char **buf) {

  struct zip_file* arq;
  struct zip_stat st;

  
  long size=-1;
  char *b;
  char nameF[8192];
    


    strcpy(nameF,"res/drawable-hdpi");
    strcat(nameF,&nameFile[getIndexLastBar(nameFile)]);  //eliminar ./res/ (6)  // 18 para 3D
	
     LOGE("Name file %s",nameF);
//  arq=fopen(nameFile,"rb");
 // arq=fopen(nameF,"rb");
	 arq = zip_fopen(APKArchive, nameF, 0);
  if(arq)
  {

  	  zip_stat(APKArchive, nameF, 0, &st);
	  LOGE("Achou %s - %d",nameF,st.size);

	  size = st.size;

   // zip_fread(arq, void *, size_t);

	b=mymalloc(size);
	if(b)
	{
	
		size=zip_fread(arq, b, size);

		LOGE("Leu bytes %d",size);
	}
	else
		LOGE("Memoria insuficiente %d",size);
	*buf=b; 
	zip_fclose(arq);
  }
  else
	 LOGE("Nao Achou %s",nameF);

  return size ;
}
/*=================================================F: Read a file limit size===================================================*/long readFileChunk(char *nameFile, char **buf,int seek,int size) {  FILE *arq;  char *b;  int rsize=-1;  arq=fopen(nameFile,"rb");  if(arq)  {	fseek(arq, seek, SEEK_SET); 	b=*buf;	if(b==0) 		b=malloc(size);	if(b)	{		rsize=(long)fread(b,1,size,arq);	}	*buf=b;	fclose(arq);  }  return rsize ;}/*=================================================F: Get size malloc==================================================*/long  getSizeMalloc(void){	return memoBusy;}/*=================================================F: Malloc===================================================*/void * mymalloc(long size){int i; void *ret;	memoBusy+=size;	ret= malloc((size_t)size);	for (i=0;i<10000;i++)	{		if(listMalloc[i].p==0)		{			listMalloc[i].p=ret;			listMalloc[i].size=size;			break;		}	}	return ret;}/*=================================================F: FreeMalloc===================================================*/void myfree(void *p){int i;		for (i=0;i<10000;i++)	{		if(listMalloc[i].p==p)		{			memoBusy-=listMalloc[i].size;			listMalloc[i].p=0;			listMalloc[i].size=0;			break;		}	}	if(p) free(p);	p=0;}/*=================================================F: Get current time mileseconds===================================================*/long getCurrentTime(void){struct timeval tv;	//currentTime=direct_clock_get_millis();	gettimeofday(&tv, NULL);
    nowTime= tv.tv_sec*1000. + tv.tv_usec/1000.;	if(firstTime==0)	{		firstTime=nowTime;	}	currentTime=nowTime-firstTime;	//LOGE("tempo %d",currentTime);	return currentTime;}int drawr=0;
#define PULAFRAME 0int getFPS(int * FPSG,int * FPS){	if(currentTime<=timeFPS+1020)
	{
		ffps++;
		//if((drawr & PULAFRAME)==PULAFRAME) 
		if(drawr)
			fvps++;
	}
	else
	{
				
		rffps=ffps;
		rfvps=fvps;
		ffps=0;
		fvps=0;
		timeFPS=currentTime;

	}

	*FPSG=rfvps;
	*FPS=rffps;


	return 1;}int getStaticFPS(int * FPSG,int * FPS){	*FPSG=rfvps;
	*FPS=rffps;	return 1;}void setDrawFrame(int d){	drawr=d;}int getDrawFrame(void){	//drawr++;
	//return ((drawr & PULAFRAME)==PULAFRAME);	return drawr;}void watchdog(long time){}void stopSound(void){}void resumeSound(void){}void resumeVideo(void){}void waitVBLANK(){}void saveVideo(void){}void tvOff(void){}	void tvOn(void){}int getRenderAudio(void){	return 1;  // habilita render audio}/*=================================================F: Get Root path ===================================================*/char * getRootPath(void){	return ROOTPATH;}char * getDEMOPath(void){	return DEMOPATH;}/*=================================================F: Get List directory ===================================================*/int lastSDX=-1;char *listSDX[]={"/dev/sda1","/dev/sdb1","/dev/sdc1","/dev/sdd1","/dev/sde1","/dev/sdf1","/dev/sdg1","/dev/sdh1","/dev/sdi1","/dev/sdj1","/dev/sdk1","/dev/sdl1","/dev/sdm1","/dev/sdn1","/dev/sdo1","/dev/sdp1","/dev/sdq1","/dev/sdr1","/dev/sds1","/dev/sdt1","/dev/sdu1","/dev/sdv1","/dev/sdw1","/dev/sdx1","/dev/sdy1","/dev/sdz1"};char lastDEV[500];int getListFileDir(char *path,listDir *list) { return 0;}/*============================================================ F: string compare to qsort	   ============================================================*/int comp(const void *s1, const void *s2){listDir *t1,*t2;	 t1=(listDir *)s1;	 t2=(listDir *)s2;	 if((t1->type==TYPE_BACKDIR))		 return -1;	 if((t2->type==TYPE_BACKDIR))		 return 1;		 return (strcmp(t1->nameFile,t2->nameFile) );	    }char *strtoupper(char *str){  char *string = str;  if(str){    for(; *str; ++str)      *str = toupper(*str);  }  return string;}/*============================================================ F: get Type file  ============================================================*/fileType getTypeFile(char *name){char *p; fileType ret = TFOTHER; char nameup[2048];	strcpy(nameup,name);	p = strtoupper( nameup );		while(ret==TFOTHER)	{		p=strchr(p,'.');	if (p != 0)	{		if ((strcmp(p, ".bin") == 0) || 			(strcmp(p, ".bin.gz") == 0) ||			(strcmp(p, ".BIN") == 0) ||			(strcmp(p, ".BIN.GZ") == 0)) ret = TFGEN;		else if ((strcmp(p, ".bms") == 0) || 			(strcmp(p, ".bms.gz") == 0) ||			(strcmp(p, ".sms") == 0) ||			(strcmp(p, ".SMS") == 0) ||			(strcmp(p, ".sms.gz") == 0) ||			(strcmp(p, ".SMS.GZ") == 0) ||			(strcmp(p, ".BMS") == 0) ||			(strcmp(p, ".gg") == 0) ||			(strcmp(p, ".GG") == 0) ||			(strcmp(p, ".gg.gz") == 0) ||			(strcmp(p, ".GG.GZ") == 0) ||			(strcmp(p, ".sg") == 0) ||			(strcmp(p, ".SG") == 0) ||			(strcmp(p, ".sg.gz") == 0) ||			(strcmp(p, ".SG.GZ") == 0) ||			(strcmp(p, ".sc") == 0) ||			(strcmp(p, ".SC") == 0) ||			(strcmp(p, ".sc.gz") == 0) ||			(strcmp(p, ".SC.GZ") == 0) ||			(strcmp(p, ".sm") == 0) ||			(strcmp(p, ".SM") == 0) ||			(strcmp(p, ".sm.gz") == 0) ||			(strcmp(p, ".SM.GZ") == 0) ||			(strcmp(p, ".sf") == 0) ||			(strcmp(p, ".SF") == 0) ||			(strcmp(p, ".sf.gz") == 0) ||			(strcmp(p, ".SF.GZ") == 0) ||			(strcmp(p, ".BMS.GZ") == 0)) ret = TFSMS;		else if ((strcmp(p, ".bmp") == 0) || 			(strcmp(p, ".bmp.gz") == 0) ||			(strcmp(p, ".BMP") == 0) ||			(strcmp(p, ".BMP.GZ") == 0)) ret = TFBMP;		else if ((strcmp(p, ".mp3") == 0) ||			(strcmp(p, ".MP3") == 0)) ret = TFMP3;		else if ((strcmp(p, ".avi") == 0) ||			(strcmp(p, ".AVI") == 0) ||			(strcmp(p, ".flv") == 0) ||			(strcmp(p, ".FLV") == 0) ||			(strcmp(p, ".mov") == 0) ||			(strcmp(p, ".MOV") == 0) ||			(strcmp(p, ".mp4") == 0) ||			(strcmp(p, ".wmv") == 0) ||			(strcmp(p, ".WMV") == 0) ||			(strcmp(p, ".MP4") == 0)) ret = TFMP4;		else if ((strcmp(p, ".jpg") == 0) ||			(strcmp(p, ".JPG") == 0)) ret = TFJPG;			}	else if (name[0] != 0) ret = TFARM;	p++;	}	return ret;	}int getIndexLastBar(char *name)
{int n;

	n=strlen(name);

	while(n>0)
	{
		if(name[n]=='/') break;
		n--;
	}

	return n++; 

}int m_z=1;
int m_w=1;

int myrand(void)
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;


}//----------------------------------------------------------
// Sound interface
//---------------------------------------------------------

int SRenderAudio(void)
{
//	return RenderAudio();
	return 0;

}

int SsetMusic(char *nameFile,int flgRepeat)
{
	setListSound(&nameFile[getIndexLastBar(nameFile)+1]);
//	return setMusic(nameFile,flgRepeat);
	return 0;
}

int SsetSoundChannel(char *nameFile,int channel,int flgRepeat)
{
//	return setSoundChannel(nameFile,channel,flgRepeat);
	return 0;
}

int SfreeAudio(void)
{
//	return freeAudio();
	return 0;
}

int SstartSound(tsound *snd)
{
//	return startSound(snd);
	return 0;
}

int SstartSoundBuffer(sample *buf,long size)
{
//	return startSoundBuffer(buf,size);
	return 0;
}void SstartIDMusic(int id,int flgRepeat)
{
	startIDSound(id,flgRepeat);
}