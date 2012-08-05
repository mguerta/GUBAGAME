
#include <stdio.h>          // Standard filesystem
#include "so.h"    // Windows specific functions and definitions
#include "sound.h"
//#include "game.h"




#define SND_BUFSIZE  256 
#define SND_HEADERS  8

#define ROOTPATH "/devel/visualstudio/DIRDATA/"


  void PlayBuffers(void);

 int comp(const void *s1, const void *s2);

// Main screen buffer
 pixel *ScrBuffer;
 // Joystick state
 unsigned int JoyState;

// Filename
 char Filename[256];

// Audio buffer
 sample SndBuffer[SND_BUFSIZE];
 sample *SndWPtr,*SndRPtr;

 int CurHdr;

// Timer variables
 int TimerID;
 int TimerReady;

 int fps;

 int ffps=0,fvps=0,rffps,rfvps;
 long timeFPS=0;

tmouse mouse;

long currentTime=0;

long long tempo,now,tempof,tempop;

long dif,diff=0;

int fpsc,cf,fpsg,cfg;

long memoBusy=0;

int stsPENDRIVE=0;
int stsJOY=0;

typedef struct _mapMalloc
{
	void *p;
	long size;
} mapMalloc;

int idxListMalloc;
mapMalloc listMalloc[100000];

int statusDraw;


int VVIDEO_WIDTH = VIDEO_WIDTH;  
int VVIDEO_HEIGHT = VIDEO_HEIGHT;


void setActualView(int w,int h)
{
	VVIDEO_WIDTH=w;
	VVIDEO_HEIGHT=h;
    
   
}

 
/** PlayBuffers() ********************************************/
/** This internal function plays ready audio buffers.       **/
/*************************************************************/
 void PlayBuffers(void)
{
 
}

/*============================================================
 F: Get mouse information
 =============================================================*/

tmouse * getMouseInf(void)
{
	return &mouse; 
}

/*============================================================
 F: Set mouse information
 =============================================================*/

void setMousePosition(tmouse * mo)
{float xf,yf;
 
    float wasp,hasp;
    
    wasp=VIDEO_WIDTH;
    hasp=VIDEO_HEIGHT;
    wasp/=VVIDEO_HEIGHT;
    hasp/=VVIDEO_WIDTH;
    
    xf=mo->y;
    yf=mo->x;
    xf*=wasp; //1.5;
    yf*=hasp; //1.5;
    xf=VIDEO_WIDTH-xf; //720.0-xf;
    
	mouse.x=(int)xf;
	mouse.y=(int)yf;
    mouse.status=mo->status;
}

extern unsigned char *fb;

//** GetVideo() *****************************************************
//** Get address of the video buffer that can be written by your   **
//** program, i.e. currently INACTIVE video buffer. The other two  **
//** functions return active (front) and inactive (back) buffers.  **
//*******************************************************************
pixel *GetVideo(void) { return(fb); }
pixel *GetFrontVideo(void) { return(fb); }
pixel *GetBackVideo(void) { return(fb); }

pixel *bufPre=0;
pixel *GetBufferPreview(void) 
{ 
	if(bufPre==0)
		bufPre=malloc(320*240*4);
	return bufPre; 
}

//** GetVideoWidth()/GetVideoHeight() *******************************
//** Get video buffer dimensions.                                  **
//*******************************************************************
int GetVideoWidth(void) { return(VIDEO_WIDTH); }
int GetVideoHeight(void) { return(VIDEO_HEIGHT); }

//** FlipVideo() ****************************************************
//** Switch displayed video buffer from VBUF0 to VBUF1 and back.   **
//** Returns the address of the INACTIVE video buffer that can be  **
//** modified by your program. The ACTIVE video buffer cannot be   **
//** written to.                                                   **
//*******************************************************************
pixel *FlipVideo(void)
{
 // Done
  //  myFlipVideo();
    iphoneFlipVideo();
  return(0);
}

//** RequestFlip() **************************************************
//** Request a video buffer flip. The FlipReady() function will    **
//** return 1 as soon as the video buffers have been flipped. Use  **
//** RequestFlip()/FlipRead() functions instead of the FlipVideo() **
//** function when you wish to do computations while waiting for   **
//** the buffers to be flipped.                                    **
//*******************************************************************
void RequestFlip(void) { FlipVideo(); }

//** FlipReady() ****************************************************
//** This function will return new buffer address as soon as the   **
//** video buffers are flipped after a call to RequestFlip(). It   **
//** returns 0 otherwise.                                          **
//*******************************************************************
pixel *FlipReady(void) { return(ScrBuffer); }

//** GetJoystick() **************************************************
//** Get the state of joypad buttons (1 means "pressed"). Refer to **
//** the BTN_* #defines for the button mapping information. Notice **
//** that on Windows this function automatically calls ProcessMsgs **
//** to process Windows messages.                                  **
//*******************************************************************
unsigned int GetJoystick(void)
{
    
   // JoyState=getJoyIphone();

  // Return joystick state
  return(JoyState);
}

void mainIphone(void)
{
    
    
    SndWPtr     = SndBuffer+1;
    
  	SndRPtr     = SndBuffer;
    
  	memset(SndBuffer,0,SND_BUFSIZE*sizeof(sample));
    
   //  Application(0);
}

//** WaitJoystick() *************************************************
//** Wait until one or more of the buttons given in the Mask have  **
//** been pressed. Returns the bitmask of pressed buttons. This    **
//** function will also eliminate jitter. Refer to BTN_* #defines  **
//** for the button mapping information.                           **
//*******************************************************************
unsigned int WaitJoystick(unsigned int Mask)
{
  unsigned int I;

  // Wait for all requested buttons to be released first
  while(GetJoystick()&Mask);
  // Wait for any of the buttons to become pressed 
  do I=GetJoystick()&Mask; while(!I);
  // Return pressed buttons
  return(I);
}

//** GetAudio() *****************************************************
//** Get base address of the audio buffer.                         **
//*******************************************************************
sample *GetAudio(void) { return(SndBuffer); }

//** GetTotalAudio() ************************************************
//** Get the total length of the audio buffer, in samples.         **
//*******************************************************************
unsigned int GetTotalAudio(void) { return(sizeof(SndBuffer)); }

//** GetFreeAudio() *************************************************
//** Get the amount of free space in the audio buffer, in samples. **
//*******************************************************************
unsigned int GetFreeAudio(void)
{
  int J;
  J=(int)(SndRPtr-SndWPtr);
  return(J>0? J:J+SND_BUFSIZE);
}

//** WriteAudio() ***************************************************
//** Write up to a given number of samples into the audio buffer.  **
//** Returns the number of samples written.                        **
//*******************************************************************
unsigned int WriteAudio(sample *Data,unsigned int Length)
{
  unsigned int J;

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
  PlayBuffers();

  // Done
  return(Length);
}


int drawr=0;
#define PULAFRAME 0
int getFPS(int * FPSG,int * FPS)
{

    now=getTimeMicro(); //direct_clock_get_millis();
    
	dif = now-tempo;
    
    
    
	if(dif>1000000) //1000)
        
	{
        
		fpsc=cf;
        
		fpsg=cfg;
        
        *FPS=fpsc;
        
		*FPSG=fpsg;
        
		cf=0;
        
		cfg=0;
        
		tempo=now;
        
	}
    
	cf++;
    
    
    
	dif=now-tempop;
    
    
    
	if(dif>5000000) //5000)
        
	{
        
		tempop=now;
        
		*FPS=fpsc;
        
		*FPSG=fpsg;
        
		return 1;
        
	}
    
	else	return 0;
    

}



int getDrawFrame(void)
{
	
	   
    now=getTimeMicro(); //direct_clock_get_millis();
    
	dif = now-tempof;

	tempof=now;
    
 //	dif+=diff;
    
	if(dif>18066) //dif>17) 
    //if(dif>16666) //dif>17) 
        
	{
    	statusDraw=0;
     
		return 0;
        
	}
    
	else	    
        
	{	
       
        cfg++;
        
		statusDraw=1;
     
		return 1;
        
	}
    
 
/*    
    now=getTimeMicro(); //direct_clock_get_millis();
    
	dif = now-tempof;
    
	tempof=now;
    
	dif+=diff;
    
	diff=dif-16393; //dif-17;
    
    
       printf("%d - %d\n",dif,diff);
	if(diff>16393*10) 
        diff=0; // para nao deixar atrasado demais
    
	if(dif>16393) //dif>17) 
        
	{
        
		statusDraw=0;
        
		return 0;
        
	}
    
	else	    
        
	{	cfg++;
        
        //	diff=0;
        
        
        
		statusDraw=1;
        
		return 1;
        
	}
 */   
    

    


}

void watchdog(long time)
{

}

void waitVBLANK()
{

}

void stopSound(void)
{

}

void tvOff(void)
{

}

void tvOn(void)
{

}

void resumeSound(void)
{

}

void saveVideo(void)
{

}

void resumeVideo(void)
{

}


void setLowCPUConsume(void)
{

}

void setNormalCPUConsume(void)
{

}

int getRenderAudio(void)
{
	return 0;
}

//void mute(void)
void mute(int i)
{
}
void resumeMute(void)
{
}



//** WaitSyncTimer() ************************************************
//** Wait for the timer to become ready.                           **
//*******************************************************************
void WaitSyncTimer(void)
{
 /*   if(dif<16393) //dif>17) 
        
	{
        do {
            now=getTimeMicro(); //direct_clock_get_millis();
            
            dif = now-tempof;
        
            
        } while (dif<16393);
        
    }    
  
 */   
   
	
  TimerReady=0;
}

//** SetSyncTimer() *************************************************
//** Set synchronization timer to a given frequency in Hz.         **
//*******************************************************************
int SetSyncTimer(int Hz)
{
  return(TimerID);
}

/*=================================================
F: Test file exist
===================================================*/
int testFileExist(char *nameFile) 
{FILE *arq;
 int ret=0;
 
  arq=fopen(nameFile,"rb");
  if(arq)
  {
	ret=1;
	fclose(arq);
  }
  return ret; 
}

/*=================================================
F: Read a file 
===================================================*/
long readFile(char *nameFile, char **buf) 
{
  FILE *arq;
  long size=-1;
  char *b;
    char nameF[8192];
    
  char *name=getFileName();
    
    strcpy(nameF,name);
   
    b=strstr(nameF,".app");
    if(b) b[4]=0;
   // nameF[strlen(nameF)-8]=0;  // eliminar .simf  -5 para um e -8 para outro 

    strcat(nameF,&nameFile[getIndexLastBar(nameFile)]);  //eliminar ./res/ (6)  // 18 para 3D
    
  //  printf("%s\n",nameF);
    

//  arq=fopen(nameFile,"rb");
  arq=fopen(nameF,"rb");
  if(arq)
  {
	fseek(arq, 0, SEEK_END); // seek to end of file
	size = ftell(arq); // get current file pointer
	fseek(arq, 0, SEEK_SET); 

	b=mymalloc(size);
	if(b)
	{
		size=(long)fread(b,1,size,arq);
	}
	*buf=b;
	fclose(arq);
  }

  return size ;
}


/*=================================================
F: Read a file limit size
===================================================*/
long readFileChunk(char *nameFile, char **buf,int seek,int size) 
{
  FILE *arq;
  char *b;
  int rsize=-1;

  arq=fopen(nameFile,"rb");
  if(arq)
  {
	fseek(arq, seek, SEEK_SET); 

	b=*buf;
	if(b==0) 
		b=mymalloc(size);
	if(b)
	{
		rsize=(long)fread(b,1,size,arq);
	}
	*buf=b;
	fclose(arq);
  }

  return rsize ;
}


/*=================================================
F: Get size malloc
===================================================*/

long  getSizeMalloc(void)
{
	return memoBusy;
}

/*=================================================
F: Malloc
===================================================*/

void * mymalloc(long size)
{int i;
 void *ret;

	memoBusy+=size;
	ret=malloc((size_t)size);

	for (i=0;i<100000;i++)
	{
		if(listMalloc[i].p==0)
		{
			listMalloc[i].p=ret;
			listMalloc[i].size=size;
			break;
		}
	}

	return ret;
}

/*=================================================
F: FreeMalloc
===================================================*/

void myfree(void *p)
{int i;	

	for (i=0;i<100000;i++)
	{
		if(listMalloc[i].p==p)
		{
			memoBusy-=listMalloc[i].size;

			listMalloc[i].p=0;
			listMalloc[i].size=0;
			break;
		}
	}

	if(p) free(p);
	p=0;
}

/*=================================================
F: Get current time mileseconds
===================================================*/

long getCurrentTime(void)
{
    currentTime=getTimeCurrent();
	return currentTime;
}

/*=================================================
F: Get Root path
 ===================================================*/

char * getRootPath(void)
{
	return ROOTPATH;
}

/*=================================================
F: Get List directory
 ===================================================*/

int getListFileDir(char *path,listDir *list) 
{
     return(0);
}

/*============================================================
 F: string compare to qsort	  
 ============================================================*/

int comp(const void *s1, const void *s2)
{listDir *t1,*t2;

	 t1=(listDir *)s1;
	 t2=(listDir *)s2;
	 if((t1->type==TYPE_BACKDIR))
		 return -1;
	 if((t2->type==TYPE_BACKDIR))
		 return 1;
	
	 return (strcmp(t1->nameFile,t2->nameFile) );	    
}

/*============================================================
 F: get Type file 
 ============================================================*/

fileType getTypeFile(char *name)
{char *p;
 fileType ret = TFOTHER;


	p = strrchr(name,'.');

	if(p==0) return ret;

	if((strcmp(p, ".gz")==0) || (strcmp(p, ".GZ")==0))
	{
		p = strchr(name,'.');
	}


	if (p != 0)
	{
		if ((strcmp(p, ".bin") == 0) || 
			(strcmp(p, ".bin.gz") == 0) ||
			(strcmp(p, ".BIN") == 0) ||
			(strcmp(p, ".BIN.GZ") == 0)) ret = TFGEN;
		else if ((strcmp(p, ".bms") == 0) || 
			(strcmp(p, ".bms.gz") == 0) ||
			(strcmp(p, ".sms") == 0) ||
			(strcmp(p, ".SMS") == 0) ||
			(strcmp(p, ".sms.gz") == 0) ||
			(strcmp(p, ".SMS.GZ") == 0) ||
			(strcmp(p, ".BMS") == 0) ||
			(strcmp(p, ".BMS.GZ") == 0)) ret = TFSMS;
		else if ((strcmp(p, ".bmp") == 0) || 
			(strcmp(p, ".bmp.gz") == 0) ||
			(strcmp(p, ".BMP") == 0) ||
			(strcmp(p, ".BMP.GZ") == 0)) ret = TFBMP;
		else if ((strcmp(p, ".mp3") == 0) ||
			(strcmp(p, ".MP3") == 0)) ret = TFMP3;
		else if ((strcmp(p, ".avi") == 0) ||
			(strcmp(p, ".AVI") == 0) ||
			(strcmp(p, ".flv") == 0) ||
			(strcmp(p, ".FLV") == 0) ||
			(strcmp(p, ".mov") == 0) ||
			(strcmp(p, ".MOV") == 0) ||
			(strcmp(p, ".mp4") == 0) ||
			(strcmp(p, ".wmv") == 0) ||
			(strcmp(p, ".WMV") == 0) ||
			(strcmp(p, ".MP4") == 0)) ret = TFMP4;
		else if ((strcmp(p, ".jpg") == 0) ||
			(strcmp(p, ".JPG") == 0)) ret = TFJPG;
		
	}
	else if (name[0] != 0) ret = TFARM;

	
	return ret;	
}


/*=================================================
F: exec File
 ===================================================*/
//extern Animate2D aniSega;

int execFile(char *nameExec, char *nameFile)
{//BMPimage view;

//	view.buf=GetVideo();
	//view.resx=GetVideoWidth();
	//view.resy=GetVideoHeight();
	//printf("%d %d %s %s \n",x,y,nameExec,nameFile);

//	printAnimate2D(x,y,&aniSega,&view);

}

int execFileNoKill(char *nameExec, char *nameFile)
{//BMPimage view;

//	view.buf=GetVideo();
	//view.resx=GetVideoWidth();
	//view.resy=GetVideoHeight();
	//printf("%d %d %s %s \n",x,y,nameExec,nameFile);

//	printAnimate2D(x,y,&aniSega,&view);

}

void RenderAudioPreview(void)
{

}

int checkStandBy(void)
{
	return 0;
}

int runStandBy(void)
{
	return 0;
}

int checkPENDRIVE(void)
{
	return stsPENDRIVE;
}

int checkJOY(void)
{
	return stsJOY;
}

int pplayer=0;
int stsPause;
int getPosMPlayer(void)
{
	if(stsPause) return pplayer;


	if(pplayer>1000) pplayer=0;
	return pplayer++;
}
int getLenMPlayer(void)
{
	return 1000;
}

int getEndMPlayer(void)
{int ret=0;

	if(pplayer>=1000)
			ret=1;
	
	return ret;
}

void setPause(int flag)
{
	stsPause=flag;
}

void resetVarMPLayer(void)
{

}

int getIndexLastBar(char *name)
{int n;
    
	n=strlen(name);
    
	while(n>0)
	{
		if(name[n]=='/') break;
		n--;
	}
    
	return n++;
}

int m_z=1;
int m_w=1;

int myrand(void)
{

    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;


}

//----------------------------------------------------------
// Sound interface
//---------------------------------------------------------
int idxSound=0;

int SRenderAudio(void)
{
	return 0;
}

int SsetMusic(char *nameFile,int flgRepeat)
{
    
    loadAVSound(&nameFile[getIndexLastBar(nameFile)+1],idxSound++,flgRepeat);
	return 0;
}

int SsetSoundChannel(char *nameFile,int channel,int flgRepeat)
{
	return 0;
}

int SfreeAudio(void)
{
	return 0;
}

int SstartSound(tsound *snd)
{
	return 0;
}

int SstartSoundBuffer(sample *buf,long size)
{
	return 0;
}

void SstartIDMusic(int id,int flgRepeat)
{
    if(flgRepeat) flgRepeat=-1;
    playAVSound(id,flgRepeat);
	
}

void nativeFadeColorScreen(float red,float green,float blue,float alpha)
{colorType color;
    
    color.r=red;
    color.g=green;
    color.b=blue;
    color.a=alpha;
    
    iphoneFadeColorScreen(&color);

}
