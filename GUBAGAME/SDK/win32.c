#include <Windows.h>
#include <stdio.h>          // Standard filesystem
#include "so.h"    // Windows specific functions and definitions
#include "sound.h"
//#include "game.h"



#ifdef WIN32
#define SND_BUFSIZE  1024
#else
#define SND_BUFSIZE  256 
#endif

#define SND_HEADERS  8

#define ROOTPATH "/devel/visualstudio/DIRDATA/"

#define DEMOPATH "/devel/visualstudio/DIRDATA/games/JOGOSMEGA/"


 void HandleMouse(LPARAM lParam);

 LRESULT CALLBACK WndProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
 DWORD WINAPI ThreadFunc(void *Param);
 int HandleKeys(int Key,int Down);
 void PlayBuffers(void);

 int comp(const void *s1, const void *s2);

// Main screen buffer
 pixel *ScrBuffer;
 HDC hDC;
 HWND hWnd;
 HBITMAP hBMap;

// Joystick state
 unsigned int JoyState;

// Filename
 char Filename[256];

// Audio buffer
 sample SndBuffer[SND_BUFSIZE];
 sample *SndWPtr,*SndRPtr;
 WAVEHDR SndHeader[SND_HEADERS];
 HWAVEOUT hWave;
 volatile DWORD ThreadID;
 int CurHdr;

// Timer variables
 int TimerID;
 int TimerReady;

 int fps;

 int ffps=0,fvps=0,rffps,rfvps;
 long timeFPS=0;

tmouse mouse;

long currentTime=0;

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


int VVIDEO_WIDTH = VIDEO_WIDTH;  
int VVIDEO_HEIGHT = VIDEO_HEIGHT;

void setActualView(int w,int h)
{
	VVIDEO_WIDTH=w;
	VVIDEO_HEIGHT=h;
}


void printDebug(char *str)
{
  OutputDebugString(str);
}


/** TimerProc() **********************************************/
/** Synchronization timer callback function.                **/
/*************************************************************/
 void CALLBACK TimerProc(UINT TimerID,UINT Msg,DWORD dwUser,DWORD dw1,DWORD dw2)  
{ 
//  TimerReady=1;
	TimerReady++;
  currentTime++;
} 
 
/** PlayBuffers() ********************************************/
/** This internal function plays ready audio buffers.       **/
/*************************************************************/
 void PlayBuffers(void)
{
  sample *P;
  int J;

  // Check hWave
  if(!hWave) return;

  // Buffer size and start
  J=SND_BUFSIZE/SND_HEADERS;
  P=SndBuffer+CurHdr*J;

  // Until we hit write pointer...
  while(!(SndHeader[CurHdr].dwFlags&WHDR_INQUEUE)&&((SndWPtr<P)||(SndWPtr>=P+J)))
  {
    // Prepare new header

    memset(&SndHeader[CurHdr],0,sizeof(WAVEHDR));
    SndHeader[CurHdr].lpData         = (char *)P;
    SndHeader[CurHdr].dwBufferLength = J*sizeof(sample);
    SndHeader[CurHdr].dwUser         = CurHdr;
    SndHeader[CurHdr].dwFlags        = 0;
    SndHeader[CurHdr].dwLoops        = 0;
    // Enqueue buffer for playback
    if(waveOutPrepareHeader(hWave,&SndHeader[CurHdr],sizeof(WAVEHDR))!=MMSYSERR_NOERROR)
      break;
    if(waveOutWrite(hWave,&SndHeader[CurHdr],sizeof(WAVEHDR))!=MMSYSERR_NOERROR)
      break;
    // Next buffer
    CurHdr=CurHdr<SND_HEADERS-1? CurHdr+1:0;
    P=SndBuffer+CurHdr*J;
  }
}

/** ProcessMsgs() ********************************************/
/** Call this function periodically to process Windows      **/
/** messages. Returns number of messages processed.         **/
/*************************************************************/
int ProcessMsgs(void)
{
  MSG Msg;
  int J;

  // Process messages
  for(J=0;PeekMessage(&Msg,NULL,0,0,PM_REMOVE);J++)
    DispatchMessage(&Msg);

  // Done 
  return(J);
}

/** WinMain() ************************************************/
/** This is a main function from which Windows starts       **/
/** executing the program.                                  **/
/*************************************************************/
int PASCAL WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR CmdLine,int CmdShow)
{
  WNDCLASS WndClass;
  BITMAPINFO BMapInfo;
  WAVEFORMATEX Format;
  HANDLE hThread;
  RECT R;

  char *cmdL=0;


  JoyState    = 0;
  hWnd        = 0;
  hDC         = 0;
  hBMap       = 0;
  Filename[0] = '\0';
  SndWPtr     = SndBuffer+1;
  SndRPtr     = SndBuffer;
  CurHdr      = 0;
  ThreadID    = 0;
  hWave       = 0;
  TimerID     = 0;

  WndClass.style         = CS_BYTEALIGNCLIENT|CS_HREDRAW|CS_VREDRAW;
  WndClass.lpfnWndProc   = WndProc;
  WndClass.cbClsExtra    = 0;
  WndClass.cbWndExtra    = 0;
  WndClass.hInstance     = hInstance;
  WndClass.hIcon         = LoadIcon(hInstance,"Icon");
  WndClass.hCursor       = LoadCursor(NULL,IDC_ARROW);
  WndClass.hbrBackground = NULL;
  WndClass.lpszMenuName  = "Menu";
  WndClass.lpszClassName = "AtGames";
  if(!RegisterClass(&WndClass)) return(0);

  // Create window
  hWnd=CreateWindow(
    "AtGames","Game",
    WS_VISIBLE|WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,CW_USEDEFAULT,VVIDEO_WIDTH,VVIDEO_HEIGHT,
    0,0,hInstance,0
  );
  if(!hWnd) return(0);

  // Adjust window size
  R.top    = 0;
  R.left   = 0;
  R.right  = VVIDEO_WIDTH-1;
  R.bottom = VVIDEO_HEIGHT-1;
  if(AdjustWindowRect(&R,WS_OVERLAPPEDWINDOW,FALSE))
    SetWindowPos(
      hWnd,
      0,0,0,R.right-R.left+1,R.bottom-R.top+1,
      SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOMOVE
    );

  // Create DC compatible with the screen 
  hDC=CreateCompatibleDC(0);
  if(!hDC) { DestroyWindow(hWnd);return(0); }

  // Fill out BITMAPINFO structure
  memset(&BMapInfo,0,sizeof(BMapInfo));
  BMapInfo.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
  BMapInfo.bmiHeader.biWidth        = VIDEO_WIDTH;
  BMapInfo.bmiHeader.biHeight       = -VIDEO_HEIGHT;
  BMapInfo.bmiHeader.biBitCount     = sizeof(pixel)*8;
  BMapInfo.bmiHeader.biCompression  = BI_RGB;
  BMapInfo.bmiHeader.biClrUsed      = 0;
  BMapInfo.bmiHeader.biClrImportant = 0;
  BMapInfo.bmiHeader.biPlanes       = 1;
  BMapInfo.bmiHeader.biSizeImage    = 0;

  // Create DIB bitmap
  hBMap=CreateDIBSection(hDC,&BMapInfo,DIB_RGB_COLORS,&ScrBuffer,NULL,0);
  if(!hBMap) { DeleteDC(hDC);DestroyWindow(hWnd);return(0); }

  // Select bitmap into DC 
  if(!SelectObject(hDC,hBMap))
  { DeleteObject(hBMap);DeleteDC(hDC);DestroyWindow(hWnd);return(0); }

  // Create a thread
  hThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)ThreadFunc,0,0,(LPDWORD)&ThreadID);

  // If audio thread has been created...
  if(hThread)
  {
    // Sound update thread is time critical
    SetThreadPriority(hThread,THREAD_PRIORITY_TIME_CRITICAL);
    // No longer need the handle, thread will exit by itself 
    CloseHandle(hThread);
    // Let the thread go into message loop 
    Sleep(100);
    // Set required format: PCM-16bit-Mono 
    Format.wFormatTag      = WAVE_FORMAT_PCM;
    Format.nChannels       = 1;
    Format.nSamplesPerSec  = 11025;//11025; //16000; //11025; //22050;
    Format.wBitsPerSample  = sizeof(sample)*8;
    Format.nBlockAlign     = Format.nChannels*Format.wBitsPerSample/8;
    Format.nAvgBytesPerSec = Format.nSamplesPerSec*Format.nBlockAlign;
    Format.cbSize          = 0;
    // Open WaveOut device
    if(waveOutOpen(&hWave,WAVE_MAPPER,&Format,ThreadID,0,CALLBACK_THREAD))
      PostThreadMessage(ThreadID,MM_WOM_CLOSE,(WPARAM)hWave,0);
  }


  // Clear buffers
  memset(ScrBuffer,0,VIDEO_WIDTH*VIDEO_HEIGHT*sizeof(pixel));
  memset(SndBuffer,0,SND_BUFSIZE*sizeof(sample));


#if ASKFILE
  P=AskFilename(
    "Select Cartridge",
    "All Files (*.*)\0*.*\0"
    "All Files (*.*)\0*.*\0"
  );
 cmdL=P;
#endif

  //gameMain();
  //menuMain();
  Application(cmdL);

  SetSyncTimer(0);
  if(hWave)
  {
    // Reset sound
     waveOutReset(hWave);
    // Close sound
     waveOutClose(hWave);
    // Wait for sound thread to exit
     while(ThreadID) Sleep(0);
  }
  // Destroy stuff
  if(hDC)   DeleteDC(hDC); 
  if(hBMap) DeleteObject(hBMap);
  // Done
  return(1);
}

/** WndProc() ************************************************/
/** Main window handler.                                    **/
/*************************************************************/
 LRESULT CALLBACK WndProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
  PAINTSTRUCT PS;

  switch(Msg)
  {
    case WM_CLOSE:
      DestroyWindow(hWnd);
      break;
    case WM_KEYUP:
      HandleKeys((int)wParam,0);
      break;
    case WM_KEYDOWN:
      if(!(HIWORD(lParam)&KF_REPEAT)) HandleKeys((int)wParam,1);
      break;

	case WM_NCMOUSEMOVE:   
		//mouse.status=0;
		break;

    case WM_MOUSEMOVE:   
            
      HandleMouse(lParam);

      break;    

    case WM_LBUTTONDOWN:
		 mouse.status|=MOUSE_DOWN;
		 HandleMouse(lParam);
   	  break;
    case WM_LBUTTONUP:
		 mouse.status^=MOUSE_DOWN;
         HandleMouse(lParam);
      break;
       	                 
    case WM_RBUTTONDOWN:
         mouse.status|=MOUSE_DOWNR; 
         HandleMouse(lParam);
      break;
    case WM_RBUTTONUP:
		mouse.status^=MOUSE_DOWNR; 
		HandleMouse(lParam);
	  break;

    case WM_PAINT:
      BeginPaint(hWnd,&PS);
      FlipVideo();
      EndPaint(hWnd,&PS);
      break;
    case WM_DESTROY:
	//	stopGame();
		stopApplication();
      break;
  }

  return(DefWindowProc(hWnd,Msg,wParam,lParam));
}

/*============================================================
 F: set mouse information
 =============================================================*/

void HandleMouse(LPARAM lParam)
{
	mouse.x=LOWORD(lParam);
	mouse.y=HIWORD(lParam); 
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
{
	//mouse.x=mo->x;
	//mouse.y=mo->y;
    
    float xf,yf;
    
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

/** HandleKeys() *********************************************/
/** Handle key presses/releases. Returns 1 if a key was     **/
/** recognized, 0 otherwise.                                **/
/*************************************************************/
 int HandleKeys(int Key,int Down)
{
  if(Down)
    switch(Key)
    {
      case VK_F12:     DestroyWindow(hWnd);return(1);
      case VK_LEFT:    JoyState|=BTN_LEFT;return(1);
      case VK_RIGHT:   JoyState|=BTN_RIGHT;return(1);
      case VK_UP:      JoyState|=BTN_UP;return(1);
      case VK_DOWN:    JoyState|=BTN_DOWN;return(1);
      case VK_SPACE:
      case 'Z':        JoyState|=BTN_A;return(1);
      case 'X':        JoyState|=BTN_B;return(1);
      case 'C':        JoyState|=BTN_C;return(1);
      case 'A':        JoyState|=BTN_X;return(1);
      case 'S':        JoyState|=BTN_Y;return(1);
      case 'D':        JoyState|=BTN_Z;return(1);
      case VK_RETURN:  JoyState|=BTN_START;return(1);
      case VK_ESCAPE:  JoyState|=BTN_EXIT;return(1);
      case 'I':        JoyState|=BTN_LEFT<<16;return(1);
      case 'P':        stsPENDRIVE=1-stsPENDRIVE;return(1);//JoyState|=BTN_RIGHT<<16;return(1);
      case '9':        JoyState|=BTN_UP<<16;return(1);
      case 'O':        JoyState|=BTN_DOWN<<16;return(1);
      case 'B':        JoyState|=BTN_A<<16;return(1);
      case 'N':        JoyState|=BTN_B<<16;return(1);
      case 'M':        JoyState|=BTN_C<<16;return(1);
      case 'H':        JoyState|=BTN_X<<16;return(1);
      case 'J':        stsJOY=1-stsJOY;return(1);//JoyState|=BTN_Y<<16;return(1);
      case 'K':        JoyState|=BTN_Z<<16;return(1);
      case VK_TAB:     JoyState|=BTN_START<<16;return(1);
    }
  else
    switch(Key)
    {
      case VK_F12:     DestroyWindow(hWnd);return(1);
      case VK_LEFT:    JoyState&=~BTN_LEFT;return(1);
      case VK_RIGHT:   JoyState&=~BTN_RIGHT;return(1);
      case VK_UP:      JoyState&=~BTN_UP;return(1);
      case VK_DOWN:    JoyState&=~BTN_DOWN;return(1);
      case VK_SPACE:
      case 'Z':        JoyState&=~BTN_A;return(1);
      case 'X':        JoyState&=~BTN_B;return(1);
      case 'C':        JoyState&=~BTN_C;return(1);
      case 'A':        JoyState&=~BTN_X;return(1);
      case 'S':        JoyState&=~BTN_Y;return(1);
      case 'D':        JoyState&=~BTN_Z;return(1);
      case VK_RETURN:  JoyState&=~BTN_START;return(1);
      case VK_ESCAPE:  JoyState&=~BTN_EXIT;return(1);
      case 'I':        JoyState&=~(BTN_LEFT<<16);return(1);
      case 'P':        JoyState&=~(BTN_RIGHT<<16);return(1);
      case '9':        JoyState&=~(BTN_UP<<16);return(1);
      case 'O':        JoyState&=~(BTN_DOWN<<16);return(1);
      case 'B':        JoyState&=~(BTN_A<<16);return(1);
      case 'N':        JoyState&=~(BTN_B<<16);return(1);
      case 'M':        JoyState&=~(BTN_C<<16);return(1);
      case 'H':        JoyState&=~(BTN_X<<16);return(1);
      case 'J':        JoyState&=~(BTN_Y<<16);return(1);
      case 'K':        JoyState&=~(BTN_Z<<16);return(1);
      case VK_TAB:     JoyState&=~(BTN_START<<16);return(1);
    }

  /* Done */
  return(0);
}

//** GetVideo() *****************************************************
//** Get address of the video buffer that can be written by your   **
//** program, i.e. currently INACTIVE video buffer. The other two  **
//** functions return active (front) and inactive (back) buffers.  **
//*******************************************************************
pixel *GetVideo(void) { return(ScrBuffer); }
pixel *GetFrontVideo(void) { return(ScrBuffer); }
pixel *GetBackVideo(void) { return(ScrBuffer); }

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
  HDC hDDC;
  RECT R;

  // Get device context and rectangle
  hDDC=GetDC(hWnd);
  if(hDDC&&GetClientRect(hWnd,&R))
  {
    // Blit
    StretchBlt(
      hDDC,0,0,R.right-R.left,R.bottom-R.top,
      hDC,0,0,VIDEO_WIDTH,VIDEO_HEIGHT,
      SRCCOPY
    );
    // Release context
    ReleaseDC(hWnd,hDDC);
  }


 // Done
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
  // Process Windows messages
  ProcessMsgs();
  // Return joystick state
  return(JoyState);
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
//  char str[1000];

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


  // sprintf(str,"lenSoud = %d\n",Length);

//	printDebug(str);
  // Done
  return(Length);
}

//** ThreadFunc() ***************************************************
//** This is a thread responsible for receiving messages from      **
//** waveOut.                                                      **
//*******************************************************************
 DWORD WINAPI ThreadFunc(void *Param)
{
  MSG Msg;
  int J;

  // Get packet-done messages 
  while(GetMessage(&Msg,0,0,0)>0)
    switch(Msg.message)
    {
      case MM_WOM_CLOSE:
        ThreadID=0;
        return(0);

      case MM_WOM_DONE:
        // Unprepare header
        waveOutUnprepareHeader(hWave,(WAVEHDR *)Msg.lParam,sizeof(WAVEHDR));
        // Move read pointer
        J=(int)(((WAVEHDR *)Msg.lParam-&SndHeader[0])+1);
        SndRPtr=SndBuffer+(J<SND_HEADERS? J:0)*(SND_BUFSIZE/SND_HEADERS);
        break;
    }

  return(0);
}

//** AskFilename() **************************************************
//** Shows an open-file dialog and makes the user select a file.   **
//** Returns 0 on failure. ATTENTION: This function returns a      **
//** pointer to the internal buffer!                               **
//*******************************************************************
const char *AskFilename(const char *Title,const char *Types)
{
  OPENFILENAME OFN;

  memset(&OFN,0,sizeof(OFN));
  OFN.lStructSize  = sizeof(OFN);
  OFN.hwndOwner    = hWnd;
  OFN.lpstrFilter  = Types? Types:"All Files (*.*)\0*.*\0";
  OFN.nFilterIndex = 1;
  OFN.lpstrFile    = Filename;
  OFN.nMaxFile     = sizeof(Filename)-1;
  OFN.lpstrTitle   = Title? Title:"Please Choose File";
  OFN.Flags        = OFN_PATHMUSTEXIST;
  return(GetOpenFileName(&OFN)? Filename:0);
}
int drawr=0;
#define PULAFRAME 1
int getFPS(int * FPSG,int * FPS)
{
//	*FPSG=60;
//	*FPS=60;

	if(currentTime<=timeFPS+1020)
	{
		ffps++;
		if((drawr & PULAFRAME)==PULAFRAME) 
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


	return 1;
}



int getDrawFrame(void)
{
	drawr++;
	return ((drawr & PULAFRAME)==PULAFRAME);
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
	return 1;
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
  //while(TimerID&&!TimerReady) 
	while(TimerReady<=(1000/fps))
		ProcessMsgs();
  TimerReady=0;
}

//** SetSyncTimer() *************************************************
//** Set synchronization timer to a given frequency in Hz.         **
//*******************************************************************
int SetSyncTimer(int Hz)
{
  /* Kill existing timer */
  if(TimerID) timeKillEvent(TimerID);

  /* Set new timer */
  if(Hz<1) TimerID=0;
  else
  {
    /* If timer was off, start timer period */
    if(!TimerID) timeBeginPeriod(5);

    /* Set timer */
   // TimerID=timeSetEvent(1000/Hz,333/Hz,TimerProc,0,TIME_PERIODIC);
	TimerID=timeSetEvent(1,1,TimerProc,0,TIME_PERIODIC);

	fps=Hz;
  }

  /* If timer is turned off, stop timer period */
  if(!TimerID) timeEndPeriod(5);

  /* Return allocated timer ID */
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

  arq=fopen(nameFile,"rb");
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
{//char str[1000];

	//sprintf(str,"curtime = %d\n",currentTime);

	//printDebug(str);

	return currentTime;
}

/*=================================================
F: Get Root path
 ===================================================*/

char * getRootPath(void)
{
	return ROOTPATH;
}

char * getDEMOPath(void)
{
	return DEMOPATH;
}

/*=================================================
F: Get List directory
 ===================================================*/

int getListFileDir(char *path,listDir *list) 
{
     int i=0;
     WIN32_FIND_DATA find_data;
     HANDLE hnd;
	 char patht[MAXPATH];
	 int flgRoot=0;


	 if(strcmp(path,ROOTPATH)==0) flgRoot=1;

	 strcpy(patht,path);
	 strcat(patht,"*");

     find_data.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
     if((hnd = FindFirstFile(patht, &find_data)) == INVALID_HANDLE_VALUE)
	 {
          return(-1); //no flies or error
     }
     
     do 
	 {
		  if((find_data.cFileName[0]=='.') && (find_data.cFileName[1]!='.')) continue;
		  list[i].type=0;
		  list[i].subType=0;  
		  if((find_data.cFileName[0]=='.') && (find_data.cFileName[1]=='.'))
		  {
		    if(strcmp(path,ROOTPATH)==0) continue;
			strcpy(find_data.cFileName,"Voltar");
			list[i].type=TYPE_BACKDIR;
			list[i].subType=TBACK;
		  }


		  if(flgRoot)
			  if(strcmp(find_data.cFileName,"usb")==0)
				if(!checkPENDRIVE()) continue;
				else
					list[i].subType=TFUSB;

          if(list[i].nameFile !=NULL) 
		  {
			//free(list[i].nameFile);
			list[i].nameFile=NULL;
		  }
		  list[i].nameFile=mymalloc((long)strlen(find_data.cFileName)); 
		  strcpy(list[i].nameFile,find_data.cFileName);
		  if(find_data.dwFileAttributes==16)
		  {
			  if(list[i].type!=TYPE_BACKDIR) 
			  {
					list[i].type=TYPE_DIR;
					if(list[i].subType==0)
					list[i].subType=TFOLD;
			  }
		  }
		  else
		  {
			  list[i].type=TYPE_FILE;
			  list[i].subType=getTypeFile(list[i].nameFile);
		  }
		  i++;

		  if(i>=NUMMAXLIST) break;
          
     } while(FindNextFile(hnd, &find_data));
     
     FindClose(hnd);

	qsort(list, i, sizeof(listDir), comp);

     return(i);
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


/*	p = strrchr(name,'.');

	if(p==0) return ret;

	if((strcmp(p, ".gz")==0) || (strcmp(p, ".GZ")==0))
	{
		p = strchr(name,'.');
	} */

	p=name;

	while(ret==TFOTHER)
	{

		p=strchr(p,'.');

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

	p++;

	}

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

	return 0;
}

int execFileNoKill(char *nameExec, char *nameFile)
{//BMPimage view;

//	view.buf=GetVideo();
	//view.resx=GetVideoWidth();
	//view.resy=GetVideoHeight();
	//printf("%d %d %s %s \n",x,y,nameExec,nameFile);

//	printAnimate2D(x,y,&aniSega,&view);
	return 0;
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
{/*int n;

	n=strlen(name);

	while(n>0)
	{
		if(name[n]=='/') break;
		n--;
	}

	return n++; */
	return 0;
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

int SRenderAudio(void)
{
	return RenderAudio();

}

int SsetMusic(char *nameFile,int flgRepeat)
{
	return setMusic(nameFile,flgRepeat);
}

int SsetSoundChannel(char *nameFile,int channel,int flgRepeat)
{
	return setSoundChannel(nameFile,channel,flgRepeat);
}

int SfreeAudio(void)
{
	return freeAudio();
}

int SstartSound(tsound *snd)
{
	return startSound(snd);
}

int SstartSoundBuffer(sample *buf,long size)
{
	return startSoundBuffer(buf,size);
}

void SstartIDMusic(int id,int flgRepeat)
{
	startIDMusic(id,flgRepeat);
}

